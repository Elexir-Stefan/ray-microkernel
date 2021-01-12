/**
 * @file scheduler.c
 * @author NURNware Technologies [Stefan Nürnberger - stefan@nurnware.de]
 * @date 02-26-2006
 * @brief Multitasking implementation and thread changing (including page table updating)
 */

#include <typedefs.h>
#include <hal/tss.h>
#include <hal/msr.h>
#include <memory/adt/hashmap.h>
#include <memory/paging.h>
#include <mtask/task_types.h>
#include <mtask/event_types.h>
#include <memory/adt/linkedlist.h>
#include <mtask/thread.h>
#include <mtask/scheduler.h>
#include <mtask/semaphore.h>
#include <memory/memory.h>
#include <hal/processor.h>
#include <hal/8259.h>
#include <hal/timer.h>
#include <memory/strings/strings.h>
#include <exceptions.h>
#include <profiler/measurement.h>
#include <hal/atom.h>

#include <debug.h>


extern TSS *kernelTSS;


PROCESS_PTR kernelThread;	/**< always points to the kernel's thread structure */
PROCESS_PTR currTask;		/**< always points to the thread structure of the thread thats currently active on the CPU */

UINT32 atomicWishes = 0;	/**< Counter for SchedulerDontInterrupt and SchedulerContinue - necessary for recursion and multithreading */

UINT32 timerTicks = 0;

IPLOCK M_ThreadList;

CreateList(THREAD_LIST, thread, struct schedNode);
CreateList(EVENT_LIST, event, EVENT_NODE);
static THREAD_LIST sleepingList;
static EVENT_LIST eventWaitingList;
typedef struct {
	PRIORITY highestPrio;
	PRIORITY lowestPrio;
	UINT32 elements;
	UINT32 ticks;
	UINT32 sumPrio;
	THREAD_LIST *runQueue[NUM_PRIO];
} PRIOQUEUES;
static PRIOQUEUES runningQueue;

static void InsertAtPrio(PROCESS_PTR newNode);
static BOOL SchedulerRemoveRunningNode(PROCESS_PTR thread);
static void HandleEventWaitingList();

void SchedulerInit(void) {
	UINT32 i;

	for (i = 0; i < NUM_PRIO; i++) {
		runningQueue.runQueue[i] = (THREAD_LIST*)KMalloc(sizeof(THREAD_LIST), MEMTYPE_DATA);
		memset(runningQueue.runQueue[i], 0, sizeof(THREAD_LIST));
	}
	runningQueue.elements = 0;
	runningQueue.ticks = 0;
	runningQueue.highestPrio = 0;
	runningQueue.lowestPrio = NUM_PRIO - 1;
	runningQueue.sumPrio = 0;

	sleepingList.threadRoot = NULL;
	sleepingList.threadTail = NULL;
	sleepingList.threadElements = 0;

	eventWaitingList.eventRoot = NULL;
	eventWaitingList.eventTail = NULL;
	eventWaitingList.eventElements = 0;

	SchedulerInsertNode(kernelThread);

	/* reset flags */
	atomicWishes = 0;

	M_ThreadList = MutexCreate("ThreadList", TRUE);
}

UINT32 SchedulerGetPendingProcesses() {
	return runningQueue.elements;
}

PROCESS_PTR GetRunningThreadList() {
	return runningQueue.runQueue[runningQueue.highestPrio]->threadRoot;
}

PROCESS_PTR GetSleepingThreadList() {
	return sleepingList.threadRoot;
}


void SchedulerThreadPause(PROCESS_PTR thread, UINT32 msecs) {
	UINT32 oneTickMSec = 1000 / SCHEDULER_TIMESLICE;
	UINT32 intervalTicks = msecs / oneTickMSec;

	ASSERT(intervalTicks > 0);

	if (intervalTicks) {

		AtomicSectionStart();

			SchedulerOverrideThreadState(thread, T_EVENT);
			EVENT_NODE *evNode = KMalloc(sizeof(EVENT_NODE), MEMTYPE_DATA);

			if (!evNode) {
				KernelThrowException("SchedulerThreadSleep: Out of memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
			}

			evNode->evNext = NULL;
			evNode->thread = thread;
			evNode->ticksTillExpired = intervalTicks;

			DLListInsertAtFront(eventWaitingList.event, ev, evNode);

		AtomicSectionEnd();
	}
}

static void InsertAtPrio(PROCESS_PTR newNode) {
	PRIORITY p = newNode->currPrio;
	THREAD_LIST *insertTo = runningQueue.runQueue[p];
	PROCESS_PTR nextPrio;
	SINT32 i;

	// set pointer to highest priority accordingly
	if (p > runningQueue.highestPrio) {
		runningQueue.highestPrio = p;
	}
	if (p < runningQueue.lowestPrio) {
		// look for pointers that might indicate the end of the list we will now append to
		if (runningQueue.highestPrio > p) {
			// there are elements higher than the current - correct their pointers
			ASSERT(runningQueue.lowestPrio < NUM_PRIO);
			runningQueue.runQueue[runningQueue.lowestPrio]->threadTail->next = newNode;
		}
		// now it's safe to update the index as we don't need the old value any longer
		runningQueue.lowestPrio = p;
	}

	/* if queue already exists, add to the end */
	if (insertTo->threadTail) {
		nextPrio = insertTo->threadTail->next;	/* points to the next element at the next priority level */
		insertTo->threadTail->next = newNode;		/* newNode now belongs to the current priority level */
	} else {
		/* else "create" queue first */
		insertTo->threadRoot = newNode;


		/* check if new element is in the middle of highest and lowest boundary
		 *
		 * If there are elements above (higher priority), their next-pointers will point to null (if there wasn't an
		 * elent with such a low priority like the new one), or to an element underneath the new one
		 * NOTE: It's not necessary to check for higher priorities, it's suffiecient if there are lower priorities, too
		 *       If there were no lower priorities (the next-pointer above would be NULL to indicate the end), it has already
		 *       been corrected by the code above ("if (p < runningQueue.lowestPrio)")
		 */
		if (runningQueue.lowestPrio < p && runningQueue.highestPrio > p) {
			/* there might be elements that skip the new element currently */
			for (i = p + 1; i <= runningQueue.highestPrio; i++) {
				if (runningQueue.runQueue[i]->threadTail) {
					runningQueue.runQueue[i]->threadTail->next = newNode;
					break;
				}
			}
		}

		/* there is no element pointing to the next priority level - find it */
		/* bottom level - mark end */
		nextPrio = NULL;
		if (p && p > runningQueue.lowestPrio) {
			for (i = p - 1; i >= runningQueue.lowestPrio; i--) {
				if (runningQueue.runQueue[i]->threadRoot) {
					nextPrio = runningQueue.runQueue[i]->threadRoot;
					break;
				}
			}
			/* if nothing found, it's also the end */
		} /* else end is marked by nextPrio == NULL */

	}
	newNode->next = nextPrio;	/* points to the next element at the next priority level */

	/* update end-pointer */
	insertTo->threadTail = newNode;
}

void SchedulerInsertNode(PROCESS_PTR newNode) {
	InsertAtPrio(newNode);
	runningQueue.sumPrio += newNode->priority;
	runningQueue.elements++;
}

BOOL CancelWaiting(PROCESS_PTR thread) {
	ASSERT(thread->state == T_EVENT);

	// TODO: Change this to a hash-map or something with O(1) access

	// we have to iterator through the list of waiting threads...
	EVENT_NODE* current = eventWaitingList.eventRoot;
	while(current) {
		if (current->thread == thread) {
			DLListRemoveNode(eventWaitingList.event, ev, current);
			KFree(current, MEMTYPE_DATA, TRUE);
			return TRUE;
		}
		current = current->evNext;
	}

	return FALSE;
}

BOOL SchedulerRemoveNode(PROCESS_PTR thread) {
	if (thread->state == T_RUNNING) {
		return SchedulerRemoveRunningNode(thread);
	} else {
		if (thread->state == T_EVENT) {
				BOOL waitRes = CancelWaiting(thread);
				if(!waitRes) {
					KernelThrowException("Could not remove event from waiting list", EXC_ILLEGAL_STATE, thread->pid, FALSE, NULL);
				}
		}

		DLListRemoveNode(sleepingList.thread, waiting, thread);
		thread->waitingNext = NULL;
		thread->waitingPrev = NULL;
		return TRUE;
	}
}

static BOOL SchedulerRemoveRunningNode(PROCESS_PTR thread) {
	PROCESS_PTR currElement = NULL;
	PROCESS_PTR parent = NULL;
	PRIORITY p =thread->currPrio;
	SINT8 i;

	for(i = runningQueue.highestPrio; i >= thread->currPrio; i--) {
		currElement = runningQueue.runQueue[runningQueue.highestPrio]->threadRoot;
		if (currElement) {
			break;
		}
	}

	ASSERT(currElement);

	while (currElement) {

		if (currElement == thread) {
			// found it
			if (parent) {
				parent->next = thread->next;
			}
			// if it's the first element of a queue, update the root pointer
			if (runningQueue.runQueue[p]->threadRoot == thread) {
				// if element was last one, update tail pointer
				if (runningQueue.runQueue[p]->threadTail == thread) {
					runningQueue.runQueue[p]->threadRoot = NULL;
					runningQueue.runQueue[p]->threadTail = NULL;

					// update global priorities
					if (p == runningQueue.highestPrio) {
						// update to the next lower priority
						ASSERT(thread->next);
						runningQueue.highestPrio = thread->next->currPrio;
					}
					if (p == runningQueue.lowestPrio) {
						for (i = p + 1; i < NUM_PRIO; i++) {
							if (runningQueue.runQueue[i]->threadRoot) {
								runningQueue.lowestPrio = i;
								break;
							}
						}
						ASSERT(FALSE);
					}
				} else {
					runningQueue.runQueue[p]->threadRoot = thread->next;
				}
			} else if (runningQueue.runQueue[p]->threadTail == thread) {
				runningQueue.runQueue[p]->threadTail = parent;
			}
			runningQueue.elements--;
			runningQueue.sumPrio -= thread->priority;
			return TRUE;
		}

		parent = currElement;
		currElement = currElement->next;
	}

	return FALSE;

}

UINT32 GetTicks() {
	return runningQueue.ticks;
}

UINT32 GetTimerTicks() {
	return timerTicks;
}

PROCESS_PTR SchedulerNextTask() {
	PRIORITY highest;
	PROCESS_PTR curr;
	PROCESS_PTR end;
	PROCESS_PTR nextPrio;
	PROCESS_PTR result;

	/* increase global tick */
	runningQueue.ticks++;


	highest = runningQueue.highestPrio;
	curr = runningQueue.runQueue[highest]->threadRoot;
	end = runningQueue.runQueue[highest]->threadTail;


	/* check for end of current priority queue */
	if (curr == end) {
		/* was the only one at the level */
		runningQueue.runQueue[highest]->threadRoot = NULL;
		runningQueue.runQueue[highest]->threadTail = NULL;

		/* decrease new highest priority (check if possible to prevent underflow) */
		if (runningQueue.highestPrio) {
			runningQueue.highestPrio--;
		}
	} else {
		/* select next entry for start (root) of queue */
		runningQueue.runQueue[highest]->threadRoot = curr->next;
	}

	/* update priority of current process */
	if (curr->currPrio) {
		curr->currPrio--;

		/* add to end of lower priority-queue */
		if (runningQueue.runQueue[curr->currPrio]->threadTail != NULL) {
			nextPrio = runningQueue.runQueue[curr->currPrio]->threadTail->next;
			runningQueue.runQueue[curr->currPrio]->threadTail->next = curr;
			runningQueue.runQueue[curr->currPrio]->threadTail = curr;
			curr->next = nextPrio;
		} else {
			/* row to insert is empty, so get the next-pointer which points to the next filled row from the current row */
			nextPrio = end->next;
			end->next = curr;	/* dont omit the newly created row */
			curr->next = nextPrio;

			runningQueue.runQueue[curr->currPrio]->threadTail = curr;
			runningQueue.runQueue[curr->currPrio]->threadRoot = curr;

			if (curr->currPrio < runningQueue.lowestPrio) {
				runningQueue.lowestPrio = curr->currPrio;
			}

		}
	} else {
		/* priority "discharged" */
		curr->currPrio = curr->priority;

		if (curr->currPrio) {
			/* as the element will be inserted at the top of the priority-list, update the highest-priority-index */
			runningQueue.highestPrio = curr->currPrio;

			/* if last element of lowest priority was removed, update the lowest-priority-index */
			if (runningQueue.runQueue[highest]->threadRoot == NULL) {
				runningQueue.lowestPrio = curr->currPrio;
			}

			/* add to end of higher priority-queue - next-pointer is already set accordingly, because curr is the highest element */
			runningQueue.runQueue[curr->currPrio]->threadTail = curr;
			runningQueue.runQueue[curr->currPrio]->threadRoot = curr;
		} else {
			if (runningQueue.runQueue[0]->threadRoot) {
				runningQueue.runQueue[0]->threadTail->next = curr;
				runningQueue.runQueue[0]->threadTail = curr;
			} else {
				runningQueue.runQueue[0]->threadRoot = curr;
				runningQueue.runQueue[0]->threadTail = curr;
			}
			curr->next = NULL;
		}
	}
	result = runningQueue.runQueue[runningQueue.highestPrio]->threadRoot;

	ASSERT(result);
	if (result->priority == PRIO_IDLE) {
		result->currPrio = result->priority;
	}

	return result;
}

static void HandleEventWaitingList() {
	AtomicSectionStart();
	EVENT_NODE *current = eventWaitingList.eventRoot, *next;

	while(current) {
		current->ticksTillExpired--;
		next = current->evNext;
		if (current->ticksTillExpired == 0) {
			// ring
			SchedulerOverrideThreadState(current->thread, T_RUNNING);
			// TODO: Decide whether to call SchedulerSelectThread or not
			// Pro:
			//		Threads that have been selected here are usually not CPU bound
			//		(because they use to sleep) and therefore earn a timely wake-up.
			// Contra:
			//		There may be two ore more processes. Who will be next?
			//		Would it be possible to gain a somewhat higher priority
			//		by intentionally pausing and therefore getting to run
			//		after the timer rang?
			DLListRemoveNode(eventWaitingList.event, ev, current);
			KFree(current, MEMTYPE_DATA, TRUE);
		}
		current = next;
	}

	AtomicSectionEnd();
}

void Schedule() {
	// check for waiting events
	/**
	 * TODO: Event list should be handled in O(1)
	 * FIXME Use priority-queue to sort waiting thready by their remaining time till expiration.
	 * Use a counter (c) that always counts down till the next event of the list.
	 *
	 * Example:
	 * Thread X will be fired in 6 ticks, thread Y in 9 Ticks and thread Z in 4 ticks from the time they are inserted.
	 *
	 * Insert X: Set c = 6 (next event)
	 * Insert Y: Leave c = 6 (is sooner than Y)
	 *
	 * Timer: c=c-1 -> c = 5	(X will be fired in 5 ticks, Y in 8)
	 * Timer: c=c-1 -> c = 4	(X will be fired in 4 ticks, Y in 7)
	 *
	 * Insert Z: set c = 4 (it is already, yes..)
	 *
	 * Timer, Timer, Timer -> c = 1 (X and Z will be fired in 1 tick!)
	 * Timer : -> get first bucket of elements (X and Z) -> fire them
	 * -> set c = 2 (sonnest remaining thread [Y])
	 * -> and so on
	 */
	if (eventWaitingList.eventElements) {
		HandleEventWaitingList();
	}

	SchedulerSelectThread(SchedulerNextTask());
	timerTicks++;
}

void SchedulerSelectThread(PROCESS_PTR thread) {
#ifdef RAYDEBUG
	ThreadSchedule();
	if (thread != currTask) {
		ThreadAdvance();
	}
#endif

	// A few sanity checks
	ASSERT(thread->semaphores.semaphore == 0);
	ASSERT(thread->semaphores.semaphoreNext == NULL);
	ASSERT(thread->waitingNext == NULL);
	ASSERT(thread->waitingPrev == NULL);

	// update run time of old task
	UINT64 stopTime = ReadTSC();
	currTask->runTimeCycles += stopTime - currTask->runTimeStart;
	currTask->privilegedCycles += stopTime - currTask->privilegedTimeStart;

	/* change current running thread */
	currTask = thread;


	// setup for new entry via INT
	kernelTSS->esp_ring0 = (UINT32)(&thread->regs + 1);
#ifdef ENABLE_FASTCALL
	WriteMSR(MSR_SYSENTER_ESP, kernelTSS->esp_ring0, 0);
#endif


	currTask->contextSwitches++;
	currTask->runTimeStart = ReadTSC();
	// If we switch the thread, we have to switch the privileged
	// cycle counter as well
	currTask->privilegedTimeStart = ReadTSC();

	// update the TID structure
	ThreadInformationBlock* TIB = currTask->threadParent->TIB;
	TIB->tid = currTask->tid;

}

BOOL SchedulerOverrideThreadState(PROCESS_PTR thread, THREAD_STATE newState) {
	BOOL awakeBefore = thread->state == T_RUNNING;
	BOOL awakeAfter = newState == T_RUNNING;

	ASSERT(thread->semaphores.semaphore == 0);
	ASSERT(thread->semaphores.semaphoreNext == NULL);
	ASSERT(thread->locked == FALSE);

	/* update thread's  state */
	thread->state = newState;

	if (!awakeBefore && awakeAfter) {
		// first of all, check whether this thread is waiting for a timer
		if (thread->state == T_EVENT) {
			CancelWaiting(thread);
		}

		// wake up
		DLListRemoveNode(sleepingList.thread, waiting, thread);
		thread->waitingNext = NULL;
		thread->waitingPrev = NULL;

		/* reset elapsed time to 0 */
		thread->currPrio = thread->priority;

		SchedulerInsertNode(thread);

	} else if (awakeBefore && !awakeAfter) {
		/* be dormant */
		if (!SchedulerRemoveRunningNode(thread)) {
			KernelThrowException("SchedulerOverrideThreadState:: Tried to get thread from running-list, but it isn't there :(",0x34f, thread->state,0,0);
		}

		ASSERT(thread->waitingNext == NULL);
		ASSERT(thread->waitingPrev == NULL);
		DLListInsertAtEnd(sleepingList.thread, waiting, thread);
		return TRUE;
	} else {
		KernelThrowException("Thread state change called despite no state changed!", EXC_ILLEGAL_STATE, thread->state, FALSE, NULL);
	}
	/* no sleep-state change */
	return FALSE;

}

void SchedulerRelinquish(void) {
	SchedulerSelectThread(SchedulerNextTask());
}


UINT32 SchedulerThreadCount() {
	return runningQueue.elements + sleepingList.threadElements;
}
