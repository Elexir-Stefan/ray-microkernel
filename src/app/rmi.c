/**
 * @file rmi.c
 * @author Stefan Nuernberger
 * @date Do Nov 16 2006
 * @brief Remote method invocation and inter-process-communication
 */

#include <typedefs.h>
#include <memory/adt/hashmap.h>
#include <app/ipc_types.h>
#include <app/rmi.h>
#include <app/rmi_internals.h>
#include <memory/arguments.h>
#include <memory/memory.h>
#include <memory/usermem.h>
#include <memory/mmu.h>
#include <memory/strings/strings.h>
#include <memory/threadmemory.h>
#include <memory/addresses.h>
#include <memory/protection.h>
#include <mtask/thread.h>
#include <mtask/scheduler.h>
#include <exceptions.h>
#include <debug.h>

HASH *serialHash;

extern PROCESS_PTR kernelThread;
static RAY_RMI RMIStartThread(PROCESS_PTR sender, PROCESS_PTR remoteThread, RMIFUNCTION funcNumber, BOOL blocking, ARGUMENTS msgArgs, POINTER msgHandle);

void KernelInitRMI(void) {
	// initialize the hash for serials to identify threads
	serialHash = HashCreateAndInit(NUM_SERIALS, RMI_PROCESS);
}

/**
 * Removes a worker thread (started due to an RMI request) from the
 * queue of waiting threads
 * @param worker Thread to be removed
 */
void RMIDequeueWorker(PROCESS_PTR worker) {
	RMETHOD* request = &worker->threadParent->RMIs[worker->currRMI.issuedRMIndex.Value];

	ASSERT(request->totalWorkers > 0);
	UnProtectKernelStructure(worker->threadParent->RMIs);
	request->totalWorkers--;


	if (request->waitQueue.waitingElements > 0) {
		PROCESS_PTR nextWaiting = request->waitQueue.waitingTail;
		DLListRemoveTail(request->waitQueue.waiting, waiting);
		ProtectKernelStructure(worker->threadParent->RMIs);
		nextWaiting->waitingNext = NULL;
		nextWaiting->waitingPrev = NULL;
		nextWaiting->threadParent->rmiQueuedWorkers--;

		SchedulerInsertNode(nextWaiting);
	} else {
		ProtectKernelStructure(worker->threadParent->RMIs);
	}
}

/**
 * Returns the message calculated by the RMI worker thread to the thread
 * which requested the RMI
 * @param exitedThread Just exited thread
 * @param message Its result
 * @return
 */
RAY_RMI PostThreadMessage(PROCESS_PTR exitedThread, UINT32 message) {
	UINT32 messageRemoteAddress;

	RMIWORK* work = &exitedThread->currRMI;

	// check if message exists
	if (LIKELY(work->msgSenderHandle && work->expectedReturnType && work->sender)) {
		PROCESS_PTR remoteThread = work->sender;

		ASSERT(remoteThread->state = T_MSG_RECV);

		// check conditions
		if (SENTBYVALUE(work->expectedReturnType)) {
			messageRemoteAddress = message;
		} else {
			// by reference
			// check type
			GIFT_BUFFER* gift = HashRetrieve(exitedThread->threadParent->msgHash, message);

			if (!gift) {
				ASSERT(FALSE);
				return RMSG_INVALID_HANDLE;
			}

			if (gift->type != work->expectedReturnType) {
				ASSERT(FALSE);
				return RMI_ARGUMENT_TYPE_MISMATCH;
			}
			if (gift->maxSize > work->expectedReturnSize) {
				ASSERT(FALSE);
				return RMI_ARGUMENT_SIZE_MISMATCH;
			}

			messageRemoteAddress = MemoryGift(exitedThread, remoteThread, message, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_USER | PAGE_WRITE);
			if (UNLIKELY(!messageRemoteAddress)) {
				ASSERT(FALSE);
				return RMSG_INVALID_BUFFER;
			}
			// Update hash to contain modified address
			HashInsert(remoteThread->threadParent->msgHash, messageRemoteAddress, gift);
			HashDelete(exitedThread->threadParent->msgHash, message);
		}

		// overwrite variable pointing to message on fallback
		ProcessWriteAddress(remoteThread, work->msgSenderHandle, messageRemoteAddress);

		// run the receiver-thread next time it is scheduled
		SchedulerOverrideThreadState(remoteThread, T_RUNNING);

		return RMI_SUCCESS;
	} else {
		ASSERT(FALSE);
		return RMI_WRONG_SETUP;
	}
}

/**
 * Starts a new thread that will handle the RMI work
 * @param sender The initiating thread
 * @param remoteThread The remote process in which the thread will be created
 * @param funcNumber Remote callback index
 * @param blocking Does the sender wait for an answer?
 * @param msgArgs Arguments to that function
 * @param msgHandle If block, handle for the returned value
 * @return
 */
static RAY_RMI RMIStartThread(PROCESS_PTR sender, PROCESS_PTR remoteThread, RMIFUNCTION funcNumber, BOOL blocking, ARGUMENTS msgArgs, POINTER msgHandle) {
	PROCESS_PTR newThread;
	ARGUMENTS threadArgs;
	RMETHOD* remoteCall = &remoteThread->RMIs[funcNumber.Value];
	char newName[THREAD_IDENT_LENGTH];
	UINT32 args[msgArgs.count];
	threadArgs.count = msgArgs.count;
	threadArgs.values = args;

	// First, look for the sender's cable
	CABLE** cablePtr = HashRetrieve(sender->threadParent->cablesByRemoteSerial, remoteThread->appSerial);
	if (!cablePtr) {
		ASSERT(FALSE);
		return RMI_NO_SETUP;
	}
	CABLE* cable = *cablePtr;

	// Fill the arguments
	//args[0] = (UINT32)senderCable;
	int i;
	MSG_CALL* remoteFunction = remoteCall->functionCall;
	// do we want to get an answer?
	if (LIKELY(blocking)) {

		// Check, whether function can actually return something
		if (remoteFunction->content.returnValue.type == RTYPE_VOID) {
			ASSERT(FALSE);
			return RMI_WRONG_SETUP;
		}
	}
	// Convert the arguments according to their type (by reference or by value)
	for(i = 0; i < msgArgs.count; i++) {
		UINT32 value = msgArgs.values[i];

		// For each argument: We have to check the type and potentially move memory
		if(SENTBYVALUE(remoteFunction->content.parameters[i].type)) {
				// by value
				args[i] = value;
		} else {
				// by reference
				GIFT_BUFFER* gift = HashRetrieve(sender->threadParent->msgHash, value);
				if (!gift) {
					return RMSG_INVALID_HANDLE;
				}

				// Assure type
				if (gift->type != remoteFunction->content.parameters[i].type) {
					ASSERT(FALSE);
					return RMI_ARGUMENT_TYPE_MISMATCH;
				}
				if (gift->maxSize > remoteFunction->content.parameters[i].maxSize) {
					ASSERT(FALSE);
					return RMI_ARGUMENT_SIZE_MISMATCH;
				}

				UINT32 messageRemoteAddress = MemoryGift(sender, remoteThread, value, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_USER | PAGE_WRITE);
				if (!messageRemoteAddress)  {
					// Could not be move the remote thread
					ASSERT(FALSE);
					return RMI_ARGUMENT_NO_GIFT;
				}

				// Save new message hash
				HashInsert(remoteThread->threadParent->msgHash, messageRemoteAddress, gift);
				HashDelete(sender->threadParent->msgHash, value);

				// Set new address
				args[i] = messageRemoteAddress;
		}

	}

	// create new thread name
	strcpy(newName, "RMI:");
	strcat(newName, sender->ident);
	strcat(newName, "->");
	strcat(newName, remoteThread->ident);

	newThread = CreateThreadFromExisting(remoteThread, remoteCall->functionCall->content.entryPoint, remoteThread->standardReturnPoint, remoteThread->priority, threadArgs, newName);
	if (LIKELY(newThread != NULL)) {

		if (blocking) {
			// change sender's thread state
			if (!SchedulerOverrideThreadState(sender, T_MSG_RECV)) {
				ASSERT(FALSE);
			}
		}

#ifdef RAYDEBUG
		UINT32 i;
		for(i = 0; i < remoteThread->numRMIs; i++) {
			if (remoteThread->RMIs[i].totalWorkers > 0) {
				ASSERT(remoteThread->RMIs[i].totalWorkers == remoteThread->RMIs[i].waitQueue.waitingElements + 1)
			}
		}
#endif

		UnProtectKernelStructure(remoteThread->RMIs);
		// only insert thread and run it if no others are currently queued
		if (remoteThread->RMIs[funcNumber.Value].totalWorkers == 0) {
			SchedulerInsertNode(newThread);

			if (blocking) {
				//SchedulerSelectThread(newThread);
				// be more fair:
				Schedule();
			}
		} else {
			// enqueue it
			DLListInsertAtFront(remoteThread->RMIs[funcNumber.Value].waitQueue.waiting, waiting, newThread);
			remoteThread->rmiQueuedWorkers++;
			if (blocking) {
				SchedulerRelinquish();
			}
		}

		// either way, increase number of current total workers
		remoteThread->RMIs[funcNumber.Value].totalWorkers++;
		ProtectKernelStructure(remoteThread->RMIs);

		// mark the fact, that this is an RMI initiated thread
		RMIWORK* work = &newThread->currRMI;
		work->issuedRMIndex = funcNumber;
		work->sendersCable = cable;
		work->sender = sender;
		work->expectsReturn = blocking;
		if (blocking) {
			work->msgSenderHandle = msgHandle;
			work->expectedReturnSize = remoteFunction->content.returnValue.maxSize;
			work->expectedReturnType = remoteFunction->content.returnValue.type;
		}

		return RMI_SUCCESS;
	} else {
		ASSERT(FALSE);
		return RMI_GEN_ERROR;
	}
}

BOOL RMIUnregister(PROCESS_PTR thread) {
	RMI_PROCESS* result;
	RMISERIAL serial = thread->appSerial;

	result = (RMI_PROCESS*)HashRetrieve(serialHash, serial);
	if (LIKELY(result->thread == thread)) {
		HashDelete(serialHash, serial);
		KFree(thread->RMIs, MEMTYPE_DATA, TRUE);
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * Sets up IPC between processes
 * @param thread to setup RMI for
 * @param threadSerial A unique THREADSERIAL we want to use to identify the process
 * @param count Maximum number of RMIs that will be registered
 * @return Status of registering (RAY_RMI)
 * @note Must be done before anything concerning RMI can be used
 */
RAY_RMI RMIExpand(PROCESS_PTR process, RMISERIAL threadSerial, UINT32 count) {
	RMI_PROCESS* exists;

	/* allocate space for array of exported symbols */
	if (threadSerial && count) {

		exists = (RMI_PROCESS*)HashRetrieve(serialHash, threadSerial);

		// Check, if the serial does not exist yet - then create it
		// otherwise, don't care.

		if (!exists) {
			RMI_PROCESS newRMI;
			newRMI.thread = process;
			HashInsert(serialHash, threadSerial, &newRMI);

			// save serial number in thread info and hash table
			process->appSerial = threadSerial;
		} else {
			// It exists, just be sure that it is identical
			ASSERT(exists->thread == process);
		}

		RMETHOD* oldRMIs = process->RMIs;
		process->RMIs = (RMETHOD*)KMalloc((count + process->numRMIs) * sizeof(RMETHOD), MEMTYPE_DATA);
		if (process->RMIs) {
			memset(process->RMIs + process->numRMIs, 0, count * sizeof(RMETHOD));

			// Expand the struct as necessary
			if (oldRMIs) {
				// by copying the old array
				memcpy(process->RMIs, oldRMIs, process->usedRMIs * sizeof(RMETHOD));
				KFree(oldRMIs, MEMTYPE_DATA, TRUE);
			}

			process->numRMIs += count;
			ProtectKernelStructure(process->RMIs);

			if (!process->threadParent->msgHash) {
				process->threadParent->msgHash = HashCreateAndInit(MSG_HASH_SIZE_PER_PROCESS, GIFT_BUFFER);
			}

			return RMI_SUCCESS;
		} else {
			return RMI_OUT_OF_MEMORY;
		}
	} else {
		return RMI_WRONG_SETUP;
	}
}


RAY_RMI RMIRegister(PROCESS_PTR process, PRIVLEVEL minPriv, BOOL partnership, MSG_CALL* msgCall) {
	RMETHOD *currRMI;

	if (process->appSerial) {
		if (process->RMIs) {

			// Assign a new entry
			currRMI = &process->RMIs[process->usedRMIs];
			if (!currRMI->functionCall) {

				UnProtectKernelStructure(process->RMIs);
				// set up RMI information
				currRMI->privNeeded = minPriv;
				currRMI->partnership = partnership;
				currRMI->functionCall = msgCall;
				CheckUserModePointersCode(process, (UINT32)msgCall->content.entryPoint);
				ASSERT((UINT32)currRMI->functionCall->content.entryPoint > USER_TEXT_SEG_START);

				ProtectKernelStructure(process->RMIs);

				// tell assigned entry and increase number of used entries
				msgCall->assignedIndex.Value = process->usedRMIs;
				process->usedRMIs++;

				return RMI_SUCCESS;
			} else {
				ASSERT(FALSE);
				return RMI_OCCUPIED;
			}
		} else {
			// seems to be not set up yet
			ASSERT(FALSE);
			return RMI_NO_SETUP;
		}
	} else {
		// serial not set up yet
		ASSERT(FALSE);
		return RMI_NO_SERIAL;
	}
}


/**
 * Used to call a certain method in a remote thread and pass a value or message buffer
 * @param thread that invoked the procedure call
 * @param remoteSerial THREADSERIAL of the thread we want to call
 * @param funcExport the unique function number we want to call
 * @param msgArgs Arguments to pass to the function
 * @param blocking if TRUE execution stops until remote function returns
 * @param msgHandle Handle to the variable that will be overwritten on return
 * @return Status of call (is RAY_RMI)
 * @see RMIRegister how to register a function that can be invoked
 */
RAY_RMI RMPassMessage(PROCESS_PTR thread, RMISERIAL remoteSerial, RMIFUNCTION funcExport, ARGUMENTS msgArgs, BOOL blocking, POINTER msgHandle) {
	RMI_PROCESS* remote;
	RMETHOD *remoteMethod;

	// TODO: SemaphoreEnter(...)

	/* check if sender set an ID */
	if (LIKELY(thread->threadParent->appSerial)) {

		/* look for requested thread */
		remote = (RMI_PROCESS*)HashRetrieve(serialHash, remoteSerial);
		if (LIKELY(remote != NULL)) {
			/* index out of bounds? */
			if (LIKELY(funcExport.Value < remote->thread->numRMIs)) {

				// get method associated with the index
				remoteMethod = &remote->thread->RMIs[funcExport.Value];

				if (LIKELY(remoteMethod->functionCall->content.entryPoint)) {
					// sufficient rights?
					if (LIKELY(thread->privLevel <= remoteMethod->privNeeded)) {
						// Check argument count
						if (msgArgs.count == remoteMethod->functionCall->content.paramCount) {
							return RMIStartThread(thread, remote->thread, funcExport, blocking, msgArgs, msgHandle);
						} else {
							ASSERT(FALSE);
							return RMI_ARGUMENT_COUNT_MISMATCH;
						}

					} else {
						/* no */
						ASSERT(FALSE);
						return RMI_INSUFF_RIGHTS;
					}
				} else {
					ASSERT(FALSE);
					return RMI_EXPORT_NOT_FOUND;
				}
			} else {
				/* no */
				ASSERT(FALSE);
				return RMI_EXPORT_NOT_FOUND;
			}
		} else {
			ASSERT(FALSE);
			return RMI_NOT_SUPPORTED;
		}
	} else {
		ASSERT(FALSE);
		return RMI_NO_SERIAL;
	}
}

/**
 * Allocates a buffer of memory that can later be used to be passed between processes (malloc won't work!)
 * @param process that wants to allocate message memory
 * @param msgSize Size to allocate
 * @param type Specifies the type of the message
 * @return RAY_MESSAGE handle, NULL if failed
 * @see FreeMessageBuffer for opposite
 */
RAY_RMI AllocateMessageBuffer(PROCESS_PTR process, UINT32 msgSize, MSG_TYPE type) {
	UINT32 freeBuffer;

	freeBuffer = (UINT32)UserMalloc(process->userMemory, msgSize, TRUE, PAGE_USER | PAGE_WRITE, MEMTYPE_DATA);

	if (LIKELY(freeBuffer)) {
		GIFT_BUFFER gift;
		ASSERT(msgSize <= 1024000000);	// test purposes only. No need for size limit
		ASSERT(type <= 11);
		gift.maxSize = msgSize;
		gift.type = type;
		HashInsert(process->threadParent->msgHash, freeBuffer, &gift);
		return freeBuffer;
	} else {
		KernelThrowException("AllocateMessageBuffer:: Out of memory", EXC_OUT_OF_MEMORY,0,0,0);
		return RMI_OUT_OF_MEMORY;
	}
}

/**
 * Deallocates a previously allocated message buffer
 * @param process that wants to free memory
 * @param msgBuffer Handle to the buffer
 * @see AllocateMessageBuffer for opposite
 */
RAY_RMI FreeMessageBuffer(PROCESS_PTR process, UINT32 msgBuffer) {
	GIFT_BUFFER* gift;

	gift = (GIFT_BUFFER*)HashRetrieve(process->threadParent->msgHash, msgBuffer);

	if (LIKELY(gift)) {
		HashDelete(process->threadParent->msgHash, msgBuffer);
		POINTER result = UserFree(process->userMemory, (POINTER)msgBuffer, MEMTYPE_DATA, TRUE);
		if (LIKELY(result)) {
			return RMI_SUCCESS;
		} else {
			return RMSG_INVALID_BUFFER;
		}
	} else {
		KernelThrowException("FreeMessageBuffer:: Invalid handle!", EXC_OUT_OF_MEMORY,msgBuffer,0,0);
		return RMSG_INVALID_HANDLE;
	}
}
