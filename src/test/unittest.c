/**
 * @file unittest.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Apr 4, 2009
 * @brief ...
 */

#include <typedefs.h>
#include <test/unittest.h>
#include <mtask/task_types.h>
#include <mtask/scheduler.h>
#include <debug.h>

static void TestRMIQueues() {

}

static void TestThreadConditions(PROCESS_PTR thread) {
	if (thread->isParent) {
		ASSERT(thread->threadParent == thread);
		ASSERT(thread->numThreads == thread->threads.threadElements);
	}
}

static void TestThreadCounts() {
	PROCESS_PTR current, last;
	last = NULL;

	current = GetSleepingThreadList();

	UINT32 count = 0;
	while (current) {

		ASSERT(current->waitingPrev == last);

		TestThreadConditions(current);

		last = current;
		current = current->waitingNext;
		count++;
	}

	current = GetRunningThreadList();
	while (current) {
		current = current->next;
		count++;
	}

	ASSERT(count == SchedulerThreadCount());
}

static void Invariants() {
	TestThreadCounts();
	TestRMIQueues();
}

void PeriodicalTests() {
	Invariants();

}
