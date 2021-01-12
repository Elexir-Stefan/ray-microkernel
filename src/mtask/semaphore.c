/**
 * @file semaphore.c
 * @brief Manages semaphores and barriers to synchronize threads
 */

#include <typedefs.h>
#include <exceptions.h>
#include <memory/memory.h>
#include <memory/adt/hashmap.h>
#include <memory/adt/hashing.h>
#include <memory/paging.h>
#include <mtask/task_types.h>
#include <mtask/semaphore_types.h>
#include <mtask/scheduler.h>
#include <hal/atom.h>
#include <memory/strings/strings.h>
#include <debug.h>

static HASH* semaphoreHash;
static HASH* barrierHash;
extern PROCESS_PTR currTask;

void SemaphoresInit(HASH* externInit) {
	// initialize the hash for the messages
	semaphoreHash = externInit;
}

void BarrierInit() {
	barrierHash = HashCreateAndInit(NUM_IP_BARRIERS_GLOBAL, BARRIER);
}

/**
 * Returns a barrier associated with the name <code>barrierName</code> or
 * a creates a new one if not found
 * @note A new barrier is automatically created, when the barrier was not found.
 * This is done in order to cope with <code>BarrierArrive</code> in other processes
 * that have been scheduler, before <code>BarrierCreate</code> has been called
 * in the owning process.
 * These non-existent barriers are assumed to be global (local ones would have
 * been created before!) and closed (because that's the reason for a barrier at all)
 * @param process The process of locally bound barriers
 * @param barrierName Name of the barrier to find or to be created
 * @param newLocal If it wasn't found - create it locally (true) or globally (false)
 * @return Always barrier, existing one or newly created one
 */
static BARRIER* GetBarrier(PROCESS_PTR process, CString barrierName, BOOL newLocal) {
	UINT32 bHash = SimpleStringHash(barrierName);

	// prioritize local ones
	BARRIER* temp = NULL;
	if (process->barriers) {
		temp = HashRetrieve(process->barriers, bHash);
	}

	// if not, it may be global
	if(!temp) {
		temp = HashRetrieve(barrierHash, bHash);
	}

	if (!temp) {
		// Neither local, nor global - create a new one
		BARRIER new;
		memset(&new, 0, sizeof(BARRIER));
		new.hash = bHash;
		new.open = FALSE;
		new.othersAllowed = TRUE;

		if (newLocal) {
			// local
			if (!process->barriers) {
				process->barriers = HashCreateAndInit(NUM_IP_BARRIERS_LOCAL, BARRIER);
			}
			HashInsert(process->barriers, bHash, &new);
			// NOTE: Cannot return &new because it was created on stack
			temp = HashRetrieve(process->barriers, bHash);
		} else {
			// global
			HashInsert(barrierHash, bHash, &new);
			// NOTE: Cannot return &new because it was created on stack
			temp = HashRetrieve(barrierHash, bHash);
		}

	}

	return temp;
}

BOOL BarrierArrive(PROCESS_PTR thread, CString barrierName) {
	BARRIER* barrier = GetBarrier(thread->threadParent, barrierName, FALSE);

	barrier->totalEverWaited++;
	if (barrier->open) {
		return TRUE;
	} else {
		// process must wait...
		ASSERT(thread->semaphores.semaphore == 0);
		ASSERT(thread->semaphores.semaphoreNext == NULL);

		ASSERT(thread->state == T_RUNNING)
		SchedulerOverrideThreadState(thread, T_WAITING);
		currTask->locked = TRUE;

		// Enqueue in list
		DLListInsertAtFront(barrier->waiting, semaphores.semaphore, thread);

		// set IPLOCK
		thread->semaphores.semaphore = barrier->hash;

		SchedulerRelinquish();

		return FALSE;

	}
}

BOOL BarrierCreate(PROCESS_PTR owner, CString barrierName, BOOL othersAllowed) {
	BARRIER* barrier = GetBarrier(owner->threadParent, barrierName, !othersAllowed);

	if (!barrier->owner) {
		barrier->othersAllowed = othersAllowed;
		barrier->owner = owner->threadParent;
		return TRUE;
	} else {
		// Barrier already created...
		ASSERT(FALSE);
		return FALSE;
	}
}

BOOL BarrierGo(PROCESS_PTR thread, CString barrierName, BOOL wakeForeignProcesses) {
	BARRIER* barrier = GetBarrier(thread->threadParent, barrierName, FALSE);

	ASSERT(!barrier->open);


	barrier->open = TRUE;
	// Barrier must have an owner!
	if (!barrier->owner) {
			ASSERT(FALSE);
			return FALSE;
	}

	// Barrier can only be let gone if owner or others are allowed to do so
	if ((barrier->owner == thread->threadParent) || barrier->othersAllowed) {

		BOOL oneIsWaiting = FALSE;
		PROCESS_PTR waitingThread = barrier->waitingRoot;
		while(waitingThread) {
			PROCESS_PTR next = waitingThread->semaphores.semaphoreNext;

			if ((waitingThread->threadParent == barrier->owner) || wakeForeignProcesses) {
				oneIsWaiting = TRUE;
				waitingThread->locked = FALSE;
				waitingThread->semaphores.semaphore = 0;
				waitingThread->semaphores.semaphoreNext = NULL;
				DLListRemoveNode(barrier->waiting, semaphores.semaphore, waitingThread);

				SchedulerOverrideThreadState(waitingThread, T_RUNNING);

			}

			waitingThread = next;
		}

		return oneIsWaiting;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}

BOOL BarrierClose(PROCESS_PTR thread, CString barrierName) {
	BARRIER* barrier = GetBarrier(thread->threadParent, barrierName, FALSE);

	// Barrier must have an owner!
	if (!barrier->owner) {
			ASSERT(FALSE);
			return FALSE;
	}

	if ((barrier->owner == thread->threadParent) || barrier->othersAllowed) {
		barrier->open = FALSE;
		return (barrier->totalEverWaited > 0);
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}

IPLOCK SemaphoreGet(CString lockName) {
	IPLOCK lockUID = SimpleStringHash(lockName);

	SEMAPHORE* exists = (SEMAPHORE*)HashRetrieve(semaphoreHash, lockUID);

	if (exists) {

		// only return, if others are allowed
		if (exists->othersAllowed || (exists->creator == currTask->threadParent)) {
			return lockUID;
		} else {
			return SEMAPHORE_DENIED;
		}

	} else {
		return SEMAPHORE_ILLEGAL;
	}

}

IPLOCK SemaphoreCreate(CString lockName, SINT32 initialValue, BOOL othersAllowed) {

	IPLOCK lockUID = SimpleStringHash(lockName);

	SEMAPHORE* exists = (SEMAPHORE*)HashRetrieve(semaphoreHash, lockUID);

	if (exists) {
		return SEMAPHORE_IN_USE;
	}

	// create a new lock
	SEMAPHORE semaphore;
	semaphore.counter = initialValue;
	semaphore.initialValue = initialValue;
	semaphore.creator = currTask->threadParent;
	semaphore.othersAllowed = othersAllowed;
	semaphore.waitingRoot = NULL;
	semaphore.waitingTail = NULL;
	semaphore.waitingElements = 0;
	HashInsert(semaphoreHash, lockUID, &semaphore);
	return lockUID;
}

SEMAPHORE_STATUS SemaphoreDestroy(IPLOCK lockUID) {

	AtomicSectionStart();
	SEMAPHORE* semaphore = HashRetrieve(semaphoreHash, lockUID);

	if (semaphore == NULL) {
		// it doesn't exist!
		AtomicSectionEnd();
		ASSERT(FALSE);
		return SEMAPHORE_ILLEGAL;
	}

	// is the thread allowed to destroy it?
	ASSERT((semaphore->creator == currTask) || semaphore->othersAllowed);

	// when destroying, it should be at the state of creation (no more threads inside!)
	if ((semaphore->waitingRoot != NULL) || (semaphore->counter != semaphore->initialValue)) {
		ASSERT(FALSE);
		AtomicSectionEnd();
		return SEMAPHORE_MISUSE;
	}
	ASSERT(semaphore->counter == semaphore->initialValue);

	// else, destroy it
	UNUSED BOOL delRes = HashDelete(semaphoreHash, lockUID);
	ASSERT(delRes);

	AtomicSectionEnd();
	return SEMAPHORE_READY;

}

MEMPROFILE SEMAPHORE_STATUS SemaphoreEnter(IPLOCK lockUID) {
	AtomicSectionStart();
	SEMAPHORE* semaphore = (SEMAPHORE*)HashRetrieve(semaphoreHash, lockUID);

	if (semaphore == NULL) {
		// it doesn't exist!
		AtomicSectionEnd();
		ASSERT(FALSE);
		return SEMAPHORE_ILLEGAL;
	}

	ASSERT(semaphore);

	// is the thread allowed to destroy it?
	ASSERT((semaphore->creator == currTask->threadParent) || semaphore->othersAllowed);


	if (semaphore->counter > 0) {
		// update lock
		semaphore->counter--;
		AtomicSectionEnd();
		return SEMAPHORE_READY;
	} else {
		// not allowed. Wait...
		// occupied
		ASSERT(currTask->semaphores.semaphore == 0);
		ASSERT(currTask->semaphores.semaphoreNext == NULL);
		SchedulerOverrideThreadState(currTask, T_WAITING);
		currTask->locked = TRUE;

		// Enqueue in list
		DLListInsertAtFront(semaphore->waiting, semaphores.semaphore, currTask);

		// set IPLOCK
		currTask->semaphores.semaphore = lockUID;

		SchedulerRelinquish();

		// will apparently not be returned until the process gets the semaphore
		AtomicSectionEnd();
		return SEMAPHORE_IN_USE;
	}
}


SEMAPHORE_STATUS SemaphoreStatus(IPLOCK lockUID) {
	AtomicSectionStart();
	SEMAPHORE* semaphore = (SEMAPHORE*)HashRetrieve(semaphoreHash, lockUID);
	AtomicSectionEnd();
	if (semaphore) {
		// check, if process is allowed to read status
		if (semaphore->othersAllowed || (semaphore->creator == currTask->threadParent)) {
			if (semaphore->counter > 0) {
				return SEMAPHORE_READY;
			} else {
				return SEMAPHORE_IN_USE;
			}
		} else {
			ASSERT(FALSE);
			return SEMAPHORE_DENIED;
		}
	} else {
		return SEMAPHORE_ILLEGAL;
	}
}


MEMPROFILE SEMAPHORE_STATUS SemaphoreLeave(UINT32 lockUID) {
	PROCESS_PTR nextWaiting;
	AtomicSectionStart();

	SEMAPHORE* semaphore = (SEMAPHORE*)HashRetrieve(semaphoreHash, lockUID);
	if (semaphore == NULL) {
		// it doesn't exist!
		AtomicSectionEnd();
		ASSERT(FALSE);
		return SEMAPHORE_ILLEGAL;
	}
	ASSERT(semaphore);

	if (semaphore->othersAllowed || (semaphore->creator == currTask->threadParent)) {

		if (semaphore->waitingElements == 0) {
			// nobody is waiting. Increase counter
			semaphore->counter++;
			AtomicSectionEnd();
			return SEMAPHORE_READY;
		} else {
			// at least one process is waiting
			nextWaiting = semaphore->waitingTail;
			// Dequeue from list
			DLListRemoveTail(semaphore->waiting, semaphores.semaphore);

			// reset semaphore status
			nextWaiting->semaphores.semaphore = 0;
			nextWaiting->semaphores.semaphoreNext = NULL;

			ASSERT(nextWaiting->locked);
			nextWaiting->locked = FALSE;
			SchedulerOverrideThreadState(nextWaiting, T_RUNNING);

			// TODO: Decide whether to scheduler next waiting or let current
			// process finish...
			//SchedulerSelectThread(nextWaiting);


			AtomicSectionEnd();
			return SEMAPHORE_IN_USE;
		}
	} else {
		ASSERT(FALSE);
		return SEMAPHORE_DENIED;
	}
}
