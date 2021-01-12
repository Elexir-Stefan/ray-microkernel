#ifndef _SCHEDULER_H
#define _SCHEDULER_H

/**
 * @file scheduler.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Multitasking
 * Scheduler functions used for multitasking
 */

#include "mtask/task_types.h"
#include "hal/syscall.h"
#include <memory/adt/linkedlist.h>

#define SCHEDULER_TIMESLICE	20

/**
 * Returns the number of processes that are in the run-queue
 * @return number of processes to be scheduled (non-idle)
 */
UINT32 SchedulerGetPendingProcesses();

/**
 * selects the next thread that is ready from the queue
 * @return the next task
 * @note Race conditions if: a) no task in queue b) all tasks are not running
 */
PROCESS_PTR SchedulerNextTask(void);

/**
 * @brief selects the next thread and calls SchedulerSelectThread
 * @note called by the asm-function of the irq 0 ISR
 */
void Schedule();

/**
 * Prepares all variables etc. for a thread-switch when return to IRQ 0 oder SysCall
 * @param thread the thread, that will be scheduled next
 */
void SchedulerSelectThread(PROCESS_PTR thread);


/**
 * Adds a thread in the run-queue
 * @param newTask the thread to add
 */
void SchedulerInsertNode(PROCESS_PTR newTask);

/**
 * Must be called once before any function can be used
 */
void SchedulerInit(void);


/**
 * Encapsulation for settings the thread's state to 'idle'
 * @param thread Thread to alter
 */
void SchedulerThreadSleep(PROCESS_PTR thread);


/**
 * Encapsulation for setting the thread's state to 'running'
 * @param thread
 */
void SchedulerRunThread(PROCESS_PTR thread);


/**
 * Gives up the remaining time-slice of the current thread
 */
void SchedulerRelinquish(void);

/**
 * Immediately leaves the current thread by calling a timer
 * interrupt so that the scheduler will pick up.
 * @note This can be dangerous as all the functions (including
 * SchedulerRelinquishNow) won't return before calling
 * the scheduler
 */
void SchedulerRelinquishNow(void);

/**
 * Overrides the current state without saving the old one
 * @param thread Thread to alter
 * @param newState new state to apply
 */
inline BOOL SchedulerOverrideThreadState(PROCESS_PTR thread, THREAD_STATE newState);


/**
 * Number of threads currently running
 * @return number of threads currently running
 */
UINT32 SchedulerThreadCount();

/**
 * Returns a pointer to a thread at wich all the running processes are concatenated (via .next)
 * @return First thread of running-queue
 */
PROCESS_PTR GetRunningThreadList();

/**
 * Returns a pointer to a thread at which all the sleeping processes are concatenated (via .waitingNext)
 * Note: intersection of running thread list is empty!)
 */
PROCESS_PTR GetSleepingThreadList();


/**
 * Removes a thread from thread-list
 * @param thread Thread to remove
 * @return true on success (was there), false if not in list
 */
BOOL SchedulerRemoveNode(PROCESS_PTR thread);

/**
 * Returns how often SchedulerNextTask was called
 * @return How often SchedulerNextTask was called
 */
UINT32 GetTicks();

/**
 * Returns the number of times the timer fired since startup (modulo 2^32 due to data-type limitation)
 * @return Number of timer ticks
 */
UINT32 GetTimerTicks();

/**
 * Moves a thread to the sleeping list for msecs milli-seconds
 * @param thread wich will be moved to the waiting list
 * @param msecs amount of time to wait (will be rounded to multiples of timer ticks)
 * @note this function is very inaccurate as it relies on timer ticks
 */
void SchedulerThreadPause(PROCESS_PTR thread, UINT32 msecs);


/**
 * Searches for the specified thread and removes is from the event list
 * @param thread to remove from the list
 */
BOOL CancelWaiting(PROCESS_PTR thread);
#endif
