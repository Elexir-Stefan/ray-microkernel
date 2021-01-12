/**
 * @file ipc.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date 2008-04-07 (Yes, i worked on my birthday...)
 * @see ipc.h
 * @brief Manages process-name/pid relations and communication based on PIDs
 */

#include <typedefs.h>
#include <app/ipc.h>
#include <app/processnames.h>
#include <mtask/pid.h>
#include <memory/strings/strings.h>
#include <debug.h>
#include <memory/memfunc.h>
#include <memory/memory.h>
#include <memory/usermem.h>
#include <memory/adt/hashing.h>

extern PROCESS_PTR currTask;

static UINT32 lastSerial = 1;

static UINT32 CreateUniqueRMISerial();
static UINT32 GetFuncSize(CALLBACKS_USER* callbacks);
static MSG_CALL* CopyFunctions(CALLBACKS_USER* source, CALLBACKS* destination, MSG_CALL* start);
static BOOL IsCompatible(MSG_CALL* source, MSG_CALL* dest);
static BOOL CheckRemoteCompatability(CALLBACKS* local, CALLBACKS* remote);
static BOOL CheckCompatibility(IPSOCKET* source, IPSOCKET* dest);
static BOOL Connect(IPSOCKET* source, IPSOCKET* dest, CABLE_USER** cable);
static CABLE* Connect2(IPSOCKET* source, IPSOCKET* dest);
static void UnplugCable(CABLE* cable);
static void Unplug(IPSOCKET* socket);

static UINT32 CreateUniqueRMISerial() {
	return lastSerial++;
}

/**
 * Calculates the total size in bytes used by the most compact
 * form of the CALLBACKS structure
 * @param callbacks
 * @return
 */
static UINT32 GetFuncSize(CALLBACKS_USER* callbacks) {
	UINT32 i;

	UINT32 totalSize = 0;
	totalSize += callbacks->numRegisteredFuncs * sizeof(MSG_CALL);

	for(i = 0; i < callbacks->numRegisteredFuncs; i++) {
		totalSize += callbacks->functions[i].paramCount * sizeof(MSG_PARAM);
	}

	return totalSize;
}

/**
 * Copies the contents of destination to source using the
 * pointer start as address for additional memory that
 * is need to save the variable amount of functions
 * and their parameters
 * @param source The CALLBACK structure to copy to
 * @param destination The CALLBACK structure to copy from
 * @param start Memory address to use for additional space
 * @return
 */
static MSG_CALL* CopyFunctions(CALLBACKS_USER* source, CALLBACKS* destination, MSG_CALL* start) {
	UINT32 i;

	destination->numRegisteredFuncs = source->numRegisteredFuncs;
	destination->functions = start;
	destination->minPriv = source->minPriv;
	destination->partnership = source->partnership;

	MSG_PARAM* msgStart = (MSG_PARAM*)(start + source->numRegisteredFuncs);
	for(i = 0; i < source->numRegisteredFuncs; i++) {
		memcpy(&destination->functions[i].content, &source->functions[i], sizeof(MSG_CALL_USER));

		destination->functions[i].content.parameters = msgStart;
		memcpy(destination->functions[i].content.parameters, source->functions[i].parameters, source->functions[i].paramCount * sizeof(MSG_PARAM));
		msgStart+= source->functions[i].paramCount;
	}

	return (MSG_CALL*)msgStart;

}

static BOOL IsCompatible(MSG_CALL* source, MSG_CALL* dest) {
	if (source->content.paramCount != dest->content.paramCount) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (source->content.returnValue.type != dest->content.returnValue.type) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (source->content.returnValue.maxSize > dest->content.returnValue.maxSize) {
		ASSERT(FALSE);
		return FALSE;
	}

	// Now check the parameters themselves
	UINT32 i;
	for(i = 0; i < source->content.paramCount; i++) {
		if (source->content.parameters[i].type != dest->content.parameters[i].type) {
			ASSERT(FALSE);
			return FALSE;
		}
		if (source->content.parameters[i].maxSize > dest->content.parameters[i].maxSize) {
			ASSERT(FALSE);
			return FALSE;
		}
	}

	// Everything is ok - compatible
	return TRUE;
}

static BOOL CheckRemoteCompatability(CALLBACKS* local, CALLBACKS* remote) {
	if(local->numRegisteredFuncs != remote->numRegisteredFuncs) {
		ASSERT(FALSE);
		return FALSE;
	}
	// for every source export check destination import
	UINT32 i;
	for(i = 0; i < local->numRegisteredFuncs; i++) {
		MSG_CALL* function = &local->functions[i];

		MSG_CALL* match = &remote->functions[i];
		// If one function cannot be found, it is not compatible
		if(!IsCompatible(function, match)) {
			ASSERT(FALSE);
			return FALSE;
		}
	}
	return TRUE;
}

static BOOL CheckCompatibility(IPSOCKET* source, IPSOCKET* dest) {
	return (CheckRemoteCompatability(&source->localCallbacks, &dest->expectedRemoteFunctions)
			&& CheckRemoteCompatability(&dest->localCallbacks, &source->expectedRemoteFunctions));
}

static BOOL Connect(IPSOCKET* source, IPSOCKET* dest, CABLE_USER** cable) {
	CABLE* c = Connect2(source, dest);
	if (!c) {
		ASSERT(FALSE);
		return FALSE;
	}
	// NOTE: As UserMalloc is called on the source socket, the conjugate
	//       will live in the destination address space!
	c->conjugate = Connect2(dest, source);
	if(!c->conjugate) {
		ASSERT(FALSE);
		return FALSE;
	}
	c->conjugate->conjugate = c;

	// Assign cable to source and dest
	// Enqueue
	c->next = source->cables;
	source->cables = c;
	c->conjugate->next = dest->cables;
	dest->cables = c->conjugate;

	// Protect content
	ProtectKernelStructure(c->conjugate);
	ProtectKernelStructure(c);

	*cable = c->userModeCable;

	// Insert cable in process' hash tables for fast retrieval
	PROCESS_PTR startThread = source->parentThread;
	PROCESS_PTR destinationThread = dest->parentThread;
	CABLE** sourceExists = HashRetrieve(startThread->cablesByRemoteSerial, destinationThread->appSerial);
	CABLE** destExists = HashRetrieve(destinationThread->cablesByRemoteSerial, startThread->appSerial);
	if (sourceExists || destExists) {
		ASSERT(FALSE);
		return FALSE;
	} else {
		HashInsert(startThread->cablesByRemoteSerial, destinationThread->appSerial, &c);
		HashInsert(destinationThread->cablesByRemoteSerial, startThread->appSerial, &c->conjugate);
	}



	return TRUE;
}

/**
 * Connects two IP sockets.
 * Essentially saves the remote RMIFUNCTION numbers assigned to the remote
 * callbacks registered by that process
 * @param source
 * @param dest
 * @return A new CABLE structure
 */
static CABLE* Connect2(IPSOCKET* source, IPSOCKET* dest) {
	UINT32 assignmentSize = sizeof(RMISERIAL) * source->expectedRemoteFunctions.numRegisteredFuncs;
	UINT32 cableSize = sizeof(CABLE) +  assignmentSize;

	CABLE* c = KMalloc(cableSize, MEMTYPE_DATA);
	CABLE_USER* cU = UserMalloc(currTask->userMemory, sizeof(CABLE_USER), TRUE, PAGE_WRITE | PAGE_USER, MEMTYPE_DATA);
	memset(c, 0, cableSize);
	memset(cU, 0, sizeof(CABLE_USER));

	cU->destinationSerial = dest->parentThread->appSerial;
	if (assignmentSize > 0) {
		cU->destinationFunctionNumbers = UserMalloc(currTask->userMemory, sizeof(RMIFUNCTION) * assignmentSize, TRUE, PAGE_WRITE | PAGE_USER, MEMTYPE_DATA);
	}

	// Fill in assigned functions
	UINT32 i;
	for(i = 0; i < dest->localCallbacks.numRegisteredFuncs; i++) {
		cU->destinationFunctionNumbers[i] = dest->localCallbacks.functions[i].assignedIndex;
	}

	// copy to kernel mode structure
	memcpy(&c->content, cU, sizeof(CABLE_USER));

	// Donate it to the owner, as it may have been created
	// in another thread than the current one
	if (currTask == source->parentThread) {
		MemoryChangeAllocAttributes(currTask, cU, PAGE_WRITE, MEM_ATTR_DISABLE);
		c->userModeCable = cU;
	} else {
		// Only move the data to the process when local callback functions do exist.
		if (assignmentSize > 0) {
			cU->destinationFunctionNumbers = (RMIFUNCTION*)MemoryGift(currTask, source->parentThread, (UINT32)cU->destinationFunctionNumbers, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_READ | PAGE_USER);
		}

		c->userModeCable = (CABLE_USER*)MemoryGift(currTask, source->parentThread, (UINT32)cU, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_READ | PAGE_USER);

		if (!c->userModeCable) {
			ASSERT(FALSE);
			return NULL;
		}
	}
	return c;
}

static void UnplugCable(CABLE* cable) {
	CABLE* conjCable = cable->conjugate;

	ASSERT(conjCable->socket->cables == conjCable);

	UnProtectKernelStructure(conjCable->socket);
	// Dequeue
	conjCable->socket->cables = conjCable->next;
	ProtectKernelStructure(conjCable->socket);

	// and delete both connections from the threads
	PROCESS_PTR sourceThread = cable->socket->parentThread;
	PROCESS_PTR destThread = cable->conjugate->socket->parentThread;
	BOOL deleted = HashDelete(sourceThread->cablesByRemoteSerial, destThread->appSerial);
	deleted &= HashDelete(destThread->cablesByRemoteSerial, sourceThread->appSerial);

	ASSERT(deleted);

	KFree(conjCable, MEMTYPE_DATA, TRUE);
}

static void Unplug(IPSOCKET* socket) {
	// Check, if we actually have something to do
	// If there is no cable, it hasn't been plugged
	while (socket->cables) {
		UnplugCable(socket->cables);
		// Dequeue
		CABLE* temp = socket->cables;
		socket->cables = socket->cables->next;
		KFree(temp, MEMTYPE_DATA, TRUE);
	}
}

/**
 * Registers every function provided in callbacks as a remotely callable
 * function via RMI
 * @param thread Process owner of these functions
 * @param callbacks The functions to register for remote calling
 */
static void RegisterRMIFunctions(PROCESS_PTR thread, CALLBACKS* callbacks) {
	UINT32 i;

	// Register serial for the first time
	RMIExpand(thread, thread->appSerial, callbacks->numRegisteredFuncs);

	for(i = 0; i < callbacks->numRegisteredFuncs; i++) {
		RMIRegister(thread, callbacks->minPriv, callbacks->partnership, &callbacks->functions[i]);
	}
}

/**
 * Registers the process with a descriptive name, so that it can be
 * reached by another process (using plugs).
 * Additionally this function assigns a SERIAL to communicate
 * via RMI for later use
 * @see PlugPlugByName
 * @param process Process that should be registered
 * @param wallName The unique name of the process
 * @param description Additional description (no further
 * meaning)
 * @return
 */
RAY_RMI CreateWall(PROCESS_PTR process, CString wallName, CString description) {
	ASSERT(process->isParent);

	// First check, if the current process is already registered
	if (GetDescriptionByPID(process->pid) == NULL) {
		// it's still free
		// So check, if the name is already registered
		PID pid = GetPIDByDescriptiveName(wallName);
		if (pid == PID_DOES_NOT_EXIST) {
			RegisterDescriptiveName(process->pid, wallName, description);
			process->appSerial = CreateUniqueRMISerial();
			return RMI_SUCCESS;
		} else {
			ASSERT(FALSE);
			return RMI_OCCUPIED;
		}
	} else {
		ASSERT(FALSE);
		return RMI_WRONG_SETUP;
	}
}


/**
 * Creates an inter process socket in the specified thread (must be parent)
 * @param thread Parent thread (process) to create socket in
 * @param socket Socket definition to create for this process
 * @return SUCCESS or error type
 */
RAY_RMI CreateSocket(PROCESS_PTR thread, IPSOCKET_USER* socket) {
    UINT32 nameLength = strlen(socket->socketName) + 1;
    UINT32 totalSize = sizeof (IPSOCKET) + nameLength + GetFuncSize(&socket->localCallbacks) + GetFuncSize(&socket->expectedRemoteFunctions);

	IPSOCKET* newSocket = KMalloc(totalSize, MEMTYPE_DATA);

	if (newSocket) {
		memset(newSocket, 0, sizeof(IPSOCKET));
		newSocket->parentThread = thread;
		newSocket->forcePairing = socket->forcePairing;
		newSocket->minNeeded = socket->minNeeded;
		newSocket->socketName = (String)(newSocket + 1);
		strcpy(newSocket->socketName, socket->socketName);
		MSG_CALL* beginning = (MSG_CALL*)(newSocket->socketName + nameLength);
		MSG_CALL* start = CopyFunctions(&socket->localCallbacks, &newSocket->localCallbacks, beginning);
		UNUSED MSG_CALL* end = CopyFunctions(&socket->expectedRemoteFunctions, &newSocket->expectedRemoteFunctions, start);
		ASSERT((UINT32)end == (UINT32)newSocket + totalSize);

		// If the hash doesn't exist yet - create it
		if (!thread->plugs) {
			thread->plugs = HashCreateAndInit(MAX_IPC_PLUGS, IPSOCKET*);
			thread->cablesByRemoteSerial = HashCreateAndInit(MAX_IPC_CABLES, CABLE*);
			if (UNLIKELY(!thread->plugs || !thread->cablesByRemoteSerial)) {
				ASSERT(FALSE);
				return RMI_OUT_OF_MEMORY;
			}
		}

		UINT32 hash = SimpleStringHash(socket->socketName);
		if (HashRetrieve(thread->plugs, hash) == NULL) {
			HashInsert(thread->plugs, hash, &newSocket);

			RegisterRMIFunctions(thread, &newSocket->localCallbacks);

			ProtectKernelStructure(newSocket);

			return RMI_SUCCESS;
		} else {
			KFree(newSocket, MEMTYPE_DATA, TRUE);
			ASSERT(FALSE);
			return RMI_OCCUPIED;
		}

	} else {
		ASSERT(FALSE);
		return RMI_OUT_OF_MEMORY;
	}
}

/**
 * Removes a specified socket by its name
 * @param thread Owner of the socket
 * @param socketName Name of the previously created socket
 * @return
 */
RAY_RMI RemoveSocket(PROCESS_PTR thread, CString socketName) {
	if (thread->plugs) {
		UINT32 hash = SimpleStringHash(socketName);
		IPSOCKET** foundPtr = HashRetrieve(thread->plugs, hash);
		if (foundPtr) {
			IPSOCKET* found = *foundPtr;
			RMIUnregister(found->parentThread);
			// de-register receiver thread as well
			Unplug(found);

			HashDelete(thread->plugs, hash);
			return RMI_SUCCESS;
		} else {
			ASSERT(FALSE);
			return RMI_EXPORT_NOT_FOUND;
		}
	} else {
		ASSERT(FALSE);
		return RMI_NO_SETUP;
	}
}


RAY_RMI PlugPlugByThread(PROCESS_PTR startThread, PROCESS_PTR destinationThread, CString plugStart, CString plugDestination, CABLE_USER** outCable) {
	if (startThread->plugs && destinationThread->plugs) {
		UINT32 sourceHash = SimpleStringHash(plugStart);
		UINT32 destHash = SimpleStringHash(plugDestination);
		IPSOCKET** sourceSocketPtr = HashRetrieve(startThread->plugs, sourceHash);
		IPSOCKET** destSocketPtr = HashRetrieve(destinationThread->plugs, destHash);

		if (sourceSocketPtr && destSocketPtr) {
			IPSOCKET* sourceSocket = *sourceSocketPtr;
			IPSOCKET* destSocket = *destSocketPtr;
			if (CheckCompatibility(sourceSocket, destSocket)) {
				UnProtectKernelStructure(sourceSocket);
				UnProtectKernelStructure(destSocket);
					sourceSocket->expectedRemoteFunctions.remoteProcess = destSocket->parentThread;
					Connect(sourceSocket, destSocket, outCable);
				ProtectKernelStructure(destSocket);
				ProtectKernelStructure(sourceSocket);


				return RMI_SUCCESS;
			} else {
				ASSERT(FALSE);
				return RMI_INCOMPATIBLE;
			}
		} else {
			ASSERT(FALSE);
			return RMI_EXPORT_NOT_FOUND;
		}
	} else {
		ASSERT(FALSE);
		return RMI_WRONG_SETUP;
	}
}

/**
 * Plugs in a cord between two specified plugs according to the pid as destination plug.
 * @param plugStart
 * @param plugDest
 * @param pid
 * @return
 */
RAY_RMI PlugPlugByPID(PROCESS_PTR source, CString plugStart, CString plugDest, UINT32 pid, CABLE_USER** outCable) {
	return PlugPlugByThread(source, GetProcessByPID(pid), plugStart, plugDest, outCable);
}

/**
 * Plugs in a cable between two specified plugs, given a remote socket's name
 * @param source
 * @param plugStart
 * @param plugDest
 * @param descriptiveName
 * @param outCable
 * @return
 */
RAY_RMI PlugPlugByName(PROCESS_PTR source, CString plugStart, CString plugDest, CString descriptiveName, CABLE_USER** outCable) {
	PID pid = GetPIDByDescriptiveName(descriptiveName);

	if (pid != PID_DOES_NOT_EXIST) {
		return PlugPlugByPID(source, plugStart, plugDest, pid, outCable);
	} else {
		ASSERT(FALSE);
		return RMI_INVALID_PROCESS;
	}
}

/**
 * Returns the current RMI cable of the thread (not process!)
 * Only iff the supplied thread was started via RMI.
 * @param thread
 * @return
 */
CABLE_USER* GetRMICable(PROCESS_PTR thread) {
	if (thread->currRMI.sendersCable) {
		return thread->currRMI.sendersCable->conjugate->userModeCable;
	} else {
		return NULL;
	}
}

UINT32 GetAllRMICables(PROCESS_PTR thread, CString socketName, CABLE_USER** list) {
	UINT32 hash = SimpleStringHash(socketName);

	IPSOCKET** socketPtr = HashRetrieve(thread->threadParent->plugs, hash);

	if (!socketPtr) {
		ASSERT(FALSE);
		return 0;
	}

	IPSOCKET* socket = *socketPtr;
	CABLE* current = socket->cables;
	UINT32 count = 0;
	while(current) {
		list[count++] = current->userModeCable;
		current = current->next;
	}

	return count;
}
