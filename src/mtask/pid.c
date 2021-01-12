/**
 * @file pid.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date So Mär 30 2008
 * @see pid.h
 * @brief Manages PIDs of processes
 */

#include <typedefs.h>
#include <mtask/task_types.h>
#include <memory/adt/hashmap.h>
#include <debug.h>
#include <mtask/pid.h>

static HASH* pidHash;
static HASH* handles;

static PID nextPID;
static UINT32 uniqueHandle;

void InitPIDs(UINT32 startingPID) {
	pidHash = HashCreateAndInit(NUM_PIDS, PROCESS_PTR);
	handles = HashCreateAndInit(NUM_PID_HANDLES, PROCESS_PTR);
	nextPID = startingPID;
	uniqueHandle = 1;
}

static UINT32 GetNewHandle() {
	return uniqueHandle++;
}


PID GetNewPID() {
	return nextPID++;
}

void RegisterThreadHandle(PROCESS_PTR thread, PID_HANDLE* handle) {
	ASSERT(thread->pidHandle == 0);
	thread->pidHandle = GetNewHandle();
	HashInsert(handles, thread->pidHandle, &thread);

	FillThreadHandle(thread, handle);
}

void FillThreadHandle(PROCESS_PTR thread, PID_HANDLE* handle) {
	ASSERT(thread->pidHandle);
	handle->handle = thread->pidHandle;
	handle->isProcessParent = thread->isParent;
	handle->pid = thread->pid;
	handle->tid = thread->isParent ? 0 : thread->tid;
}

BOOL UnregisterThreadHandle(PROCESS_PTR toFind) {
	if (toFind->pidHandle) {
		PROCESS_PTR* found = HashRetrieve(handles, toFind->pidHandle);
		if (LIKELY(found)) {
			PROCESS_PTR thread = *found;
			// check for validity
			if (thread == toFind) {
				HashDelete(handles, toFind->pidHandle);
				toFind->pidHandle = 0;
				return TRUE;
			} else {
				ASSERT(FALSE);
				return FALSE;
			}
		} else {
			// This situation is impossible! Thread registered properly
			// but no handle found?!
			ASSERT(FALSE);
			return FALSE;
		}
	} else {
		// if no handle found, this only applies for threads created
		// by RMI functions
		ASSERT(toFind->currRMI.sender != NULL);
		return FALSE;
	}

}

PROCESS_PTR GetThreadByHandle(PID_HANDLE* handle) {
	PROCESS_PTR* found = HashRetrieve(handles, handle->handle);
	if (found) {
		PROCESS_PTR thread = *found;
		// check for validity
		if ((handle->pid == thread->pid) && (thread->isParent == handle->isProcessParent || thread->tid == handle->tid)) {
			return thread;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
}

void RegisterPID(PID pid, PROCESS_PTR node) {
	ASSERT(HashRetrieve(pidHash, pid) == NULL);
	HashInsert(pidHash, pid, &node);
}

PROCESS_PTR UnregisterPID(PID pid) {
	PROCESS_PTR* found = HashRetrieve(pidHash, pid);

	if (found) {
		HashDelete(pidHash, pid);
		return *found;
	} else {
		ASSERT(FALSE);
		return NULL;
	}
}

PROCESS_PTR GetProcessByPID(PID pid) {
	PROCESS_PTR* found= HashRetrieve(pidHash, pid);

		if (found) {
			return *found;
		} else {
			ASSERT(FALSE);
			return NULL;
		}

}
