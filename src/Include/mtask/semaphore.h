/*
 * semaphore.h
 *
 *  Created on: 07.03.2009
 *      Author: stefan
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#define NUM_IP_SEMAPHORES			2048
#define NUM_IP_BARRIERS_GLOBAL		2048
#define NUM_IP_BARRIERS_LOCAL		(PAGE_SIZE/sizeof(BARRIER))

typedef UINT32 IPLOCK;

typedef struct {
	struct schedNode* semaphoreNext;
	struct schedNode* semaphorePrev;
	IPLOCK semaphore;
} SEMAPHORELIST;

typedef enum {
	SEMAPHORE_IN_USE = 0,
	SEMAPHORE_ILLEGAL = 1,
	SEMAPHORE_MISUSE = 2,
	SEMAPHORE_DENIED = 3,
	SEMAPHORE_READY = 4
} SEMAPHORE_STATUS;

#include <memory/adt/hashmap.h>

/**
 * Initializes the data structures needed for semaphore handling. As semaphores
 * are needed before proper memory management works, this function takes an
 * argument to an already initialized HASH structure that will be used.
 * @param externInit Already initialized hash to use
 */
void SemaphoresInit(HASH* externInit);

/**
 * Called once by the kernel startup routines to set up proper data structures
 */
void BarrierInit();

/**
 * Lets the thread <code>thread</code> wait before a barrier, if it was not
 * released. Otherwise this functions returns immediately.
 * @param thread Thread to wait for a barrier
 * @param barrierName Name associated with a (not necessarily existing) barrier
 * @return TRUE if actually waited, FALSE if already open
 */
BOOL BarrierArrive(struct schedNode* thread, CString barrierName);

/**
 * Can only be called once: Lets all the waiting processes run again and
 * will let all future process that will wait for this barrier run immediately
 * @param thread Thread/process which tries to unlock this barrier
 * @param barrierName Name of barrier to let go
 * @param wakeForeignProcesses Decides whether other processes should to be woken
 * up by this barrier.
 * @return TRUE if already at least one thread is waiting, FALSE otherwise
 */
BOOL BarrierGo(struct schedNode* thread, CString barrierName, BOOL wakeForeignProcesses);

/**
 * Closes a barrier again so that no new processes can enter it.
 * All the process that may have entered it already may still
 * remain in a critical section
 * @param barrierName
 * @return TRUE if nobody every entered the barrier, FALSE otherwise
 */
BOOL BarrierClose(struct schedNode* thread, CString barrierName);

/**
 * Creates a barrier according to the given name
 * @param owner Owner of the barrier
 * @param barrierName
 * @param othersAllowed Decides whether other processes are allowed to let go
 * this barrier or close this barrier
 * @return TRUE on success, FALSE if barrier with given name already exists
 */
BOOL BarrierCreate(struct schedNode* owner, CString barrierName, BOOL othersAllowed);

IPLOCK SemaphoreGet(CString lockName);
IPLOCK SemaphoreCreate(CString lockName, SINT32 initialValue, BOOL othersAllowed);
SEMAPHORE_STATUS SemaphoreDestroy(IPLOCK lockUID);
SEMAPHORE_STATUS SemaphoreEnter(IPLOCK lockUID);
SEMAPHORE_STATUS SemaphoreLeave(UINT32 lockUID);
SEMAPHORE_STATUS SemaphoreStatus(IPLOCK lockUID);

#define MutexCreate(title, othersAllowed) SemaphoreCreate(title, 1, othersAllowed)

#endif /* SEMAPHORE_H_ */
