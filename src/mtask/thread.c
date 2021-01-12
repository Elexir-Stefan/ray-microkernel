/**
 * @file thread.c
 * @author Stefan Nürnberber [NURNware Technologies - stefan@nurnware.de]
 * @brief Functions for starting and managing threads and processes
 */
#include <typedefs.h>
#include <memory/memory.h>
#include <memory/adt/hashmap.h>
#include <memory/mmu.h>
#include <memory/arguments.h>
#include <memory/threadmemory.h>
#include <mtask/task_types.h>
#include <mtask/scheduler.h>
#include <mtask/thread.h>
#include <mtask/pid.h>
#include <app/rmi.h>
#include <memory/usermem.h>
#include <hal/gdt.h>
#include <hal/atom.h>
#include <hal/x86flags.h>
#include <hal/abi.h>
#include <exceptions.h>
#include <memory/strings/strings.h>
#include <debug.h>

extern PROCESS_PTR currTask, kernelThread;



void InitThreading() {
	InitPIDs(kernelThread->pid + 1);
	RegisterPID(kernelThread->pid, kernelThread);
}

/**
 * Fills the THREAD_INFO structure with information from a thread/process
 * provided in the PROCESS_PTR struct.
 * @param thread
 * @param tInfo
 */
static void FillThreadInfo(PROCESS_PTR thread, THREAD_INFO *tInfo) {
	tInfo->pid = thread->pid;
	tInfo->serial = thread->appSerial;
	tInfo->numExports = thread->numRMIs;

	tInfo->usedMemory = thread->userMemory->threadMemUsage;
	tInfo->usedPages = thread->userMemory->threadMemPages;
	tInfo->memAllocs= thread->userMemory->memAllocs->numEntries;
	tInfo->lastFitPointerCode = thread->userMemory->bitmaps.space[MEMTYPE_CODE].lastFit;
	tInfo->lastFitPointerData = thread->userMemory->bitmaps.space[MEMTYPE_DATA].lastFit;

	tInfo->cpuCycles = thread->runTimeCycles;
	tInfo->privCycles = thread->privilegedCycles;
	tInfo->contextSwitches = thread->contextSwitches;
	tInfo->state = thread->state;

	tInfo->threadNum = thread->isParent ? 0 : thread->tid;
	tInfo->isParent = thread->isParent;

	strncpy(tInfo->ident, thread->ident, THREAD_IDENT_LENGTH - 1);


}

/**
 * Adds another process to the thread's waiting list. These processes
 * will be scheduled when the thread terminates
 * @param thread Thread to watch for
 * @param waitingThread Thread that will be queued
 * @param writeBackAddress Pointer to address in waitingThread's memory
 * where the result will be written to
 */
void ThreadAddJoinList(PROCESS_PTR thread, PROCESS_PTR waitingThread) {
	ASSERT(waitingThread->state == T_RUNNING);

	// stop thread and seal it
	SchedulerOverrideThreadState(waitingThread, T_WAITING);
	waitingThread->locked = TRUE;

	JoinElement* join = KMalloc(sizeof(JoinElement), MEMTYPE_DATA);

	join->thread = waitingThread;
	DLListInsertAtFront(thread->joined.thread, waiting, join);

	// make structure write-protected
	ProtectKernelStructure(join);
}

static void ThreadEngageWaitingJoins(PROCESS_PTR thread, SINT32 exitCode) {
	while(thread->joined.threadElements) {
		JoinElement* join = thread->joined.threadRoot;
		DLListRemoveRoot(thread->joined.thread, waiting);
		// remove locking...
		ASSERT(join->thread->locked);
		join->thread->locked = FALSE;
		SchedulerOverrideThreadState(join->thread, T_RUNNING);
		ProcessWriteResult(join->thread, (UINT32)exitCode);
		KFree(join, MEMTYPE_DATA, TRUE);
	}
}

/**
 * Get process/thread information
 * @param pid of the process you want to get information of, 0 for all processes
 * @return list of process information
 */
THREAD_INFO_LIST* GetThreadInfo(UINT32 pid) {
	THREAD_INFO_LIST *info;
	THREAD_INFO *threadList;

	AtomicSectionStart();


	if (pid) {
		// specific for one process
		info = (THREAD_INFO_LIST*)UserMalloc(currTask->userMemory, sizeof(THREAD_INFO_LIST) +  sizeof(THREAD_INFO), TRUE, PAGE_USER | PAGE_WRITE, MEMTYPE_DATA);
		threadList = (THREAD_INFO*)(info + 1);

		info->numThreads = 1;
		PROCESS_PTR process = GetProcessByPID(pid);
		ASSERT(process);
		FillThreadInfo(process, threadList);
	} else {
		// list of all processes
		THREAD_INFO *currThreadInfo;
		PROCESS_PTR current;
		PROCESS_PTR kernelThreadRunningList = GetRunningThreadList();
		PROCESS_PTR kernelThreadSleepingList = GetSleepingThreadList();
		UINT32 numThreads = SchedulerThreadCount();
		UINT32 countedThreads;

		info = (THREAD_INFO_LIST*)UserMalloc(currTask->userMemory, sizeof(THREAD_INFO_LIST) + numThreads * sizeof(THREAD_INFO), TRUE, PAGE_USER | PAGE_WRITE, MEMTYPE_DATA);
		threadList = (THREAD_INFO*)(info + 1);
		info->numThreads = numThreads;

		// fill list with both - running and sleeping list
		current = kernelThreadRunningList;
		countedThreads = 0;
		currThreadInfo = threadList;
		while(current) {
			FillThreadInfo(current, currThreadInfo);
			currThreadInfo++;
			countedThreads++;
			current = current->next;
		}
		current = kernelThreadSleepingList;
		while (current) {
			FillThreadInfo(current, currThreadInfo);
			currThreadInfo++;
			countedThreads++;
			current = current->waitingNext;
		}
		if (countedThreads != numThreads) {
			KernelThrowException("GetThreadInfo: inconsistent thread count!", EXC_ILLEGAL_STATE, 0,0,0);
		}
	}

	// general info
	info->thread = threadList;
	info->memUsageTotal = SystemMemoryUsed();
	info->kernelMemory = kernelThread->userMemory->threadMemUsage;
	info->memFree = SystemMemoryFree();

	// Before returning, we should seal the data...
	MemoryChangeAllocAttributes(currTask, info, PAGE_WRITE, MEM_ATTR_DISABLE);

	AtomicSectionEnd();

	return info;
}

/**
 * Determines whether a process/thread to the specified handle still exists
 * @param handle Handle to the process/thread
 */
BOOL IsThreadAlive(PID_HANDLE* handle) {
	return GetThreadByHandle(handle) != NULL;
}

/**
 * Creates a thread from a currently running process.
 * Sets up new stack (user for data + kernel for registers) and alters instruction pointer
 * @param oldThread Thread to spawn from
 * @param functionAddress entry point of thread
 * @param priority new priority of thread
 * @param args argument list that will be passed to the called function
 * @param threadName Name of the new thread
 * @return new Thread-node
 */
PROCESS_PTR CreateThreadFromExisting(PROCESS_PTR oldThread, POINTER functionAddress, POINTER returnAddress, PRIORITY priority, ARGUMENTS args, CString threadName) {
	PROCESS_PTR newThread, parentThread;

	ASSERT((UINT32)functionAddress >= 0x1000);

	newThread = (PROCESS_PTR)KMalloc(sizeof(struct schedNode), MEMTYPE_DATA);
	memset(newThread, 0, sizeof(struct schedNode));

	// maintain parameters of its creator
	newThread->userMemory = oldThread->userMemory;
	newThread->CPUring = oldThread->CPUring;
	newThread->bssSection = oldThread->bssSection;
	newThread->codeStart = oldThread->codeStart;
	newThread->isParent = FALSE;

	strcpy(newThread->ident, threadName);
	newThread->pid = oldThread->pid;
	newThread->privLevel = oldThread->privLevel;


	ASSERT(priority < NUM_PRIO);

	PRIORITY newPrio;
	if (oldThread->privLevel <= PRIV_PRIORITY) {
		newPrio = priority;
	} else {
		newPrio = oldThread->priority;
	}



	// maintain linked thread list
	newThread->isParent = FALSE;		// its not the main thread, a child only
	parentThread = oldThread->threadParent;	// get main thread that was initially spawned (the process itself)
	newThread->threadParent = parentThread;	// set roots

	// count how often parent created threads
	parentThread->numThreads++;
	parentThread->tid++;

	// and set current thread#
	newThread->tid = parentThread->tid;

	PrepareThread(newThread, functionAddress, returnAddress, newPrio, args);

	// enqueue in parent
	DLListInsertAtEnd(parentThread->threads.thread, threadItem.child, newThread);
	newThread->runTimeCycles = 0;

	return newThread;
}

void PrepareThread(PROCESS_PTR newThread, POINTER functionAddress, POINTER returnAddress, PRIORITY priority, ARGUMENTS args) {
	UINT32 *userAppStack;
	UINT32 *currUserStack;
	UINT32 appStackRemoteAddress, userStackRemoteAddress;
	UINT32 currArg;
	INT_REG *threadRegs;

	userAppStack = (UINT32*)UserMalloc(currTask->userMemory, APP_USER_STACK_SIZE * sizeof(UINT32), TRUE, PAGE_USER | PAGE_WRITE, MEMTYPE_DATA);
	currUserStack = userAppStack;
	currUserStack += APP_USER_STACK_SIZE;

	newThread->argCount = args.count;
	/* push arguments (in reverse order!) */
	currArg = args.count;
	while(currArg--) {
		*(--currUserStack) = args.values[currArg];
	}

	// push return address
	*(--currUserStack) = (UINT32)returnAddress;

	// since data on stack will not be altered any longer, pass it to the thread it actually belongs to
	appStackRemoteAddress = MemoryGift(currTask, newThread, (UINT32)userAppStack, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_USER | PAGE_WRITE);
	ASSERT(appStackRemoteAddress);

	// correct currUserStack
	userStackRemoteAddress = appStackRemoteAddress + ((UINT32)currUserStack - (UINT32)userAppStack);

	threadRegs = &newThread->regs;

	threadRegs->ss = RING3 == newThread->CPUring ? USER_SEG_DATA | DPL3 : KERNEL_SEG_DATA | DPL0;
	threadRegs->userESP = userStackRemoteAddress;
	threadRegs->eflags = X86FLAG_STANDARD | X86FLAG_IO_RING0 | X86FLAG_INTERRUPT;
	threadRegs->cs = RING3 == newThread->CPUring ? USER_SEG_CODE | DPL3 : KERNEL_SEG_CODE | DPL0;
	threadRegs->eip = (UINT32)functionAddress;
	threadRegs->intNum = 0;
	threadRegs->eax = 0;
	threadRegs->ecx = 0;
	threadRegs->edx = 0;
	threadRegs->ebx = 0;
	threadRegs->esp = userStackRemoteAddress;
	threadRegs->ebp = userStackRemoteAddress;
	threadRegs->esi = 0;
	threadRegs->edi = 0;
	threadRegs->ds = (RING3 == newThread->CPUring) ? USER_SEG_DATA : KERNEL_SEG_DATA;
	threadRegs->es = (RING3 == newThread->CPUring) ? USER_SEG_DATA : KERNEL_SEG_DATA;
	threadRegs->fs = (RING3 == newThread->CPUring) ? USER_SEG_DATA : KERNEL_SEG_DATA;
	threadRegs->gs = (RING3 == newThread->CPUring) ? USER_SEG_TIB : KERNEL_SEG_TIB;

	newThread->regPointer = threadRegs;

	newThread->userStack = (UINT32*)appStackRemoteAddress;
	newThread->priority = priority;
	newThread->currPrio = priority;

}

/**
 * creates a new thread of a function
 * @param function_address The function to spawn as a new thread
 * @param priority the priority of the new function (-127 to 128)
 * @param args Arguments to push on user's stack
 */
PROCESS_PTR SpawnProcess() {
	PROCESS_PTR newProcess = KMalloc(sizeof(struct schedNode), MEMTYPE_DATA);
	memset(newProcess, 0, sizeof(struct schedNode));

	// all threads run in Ring 3
	newProcess->CPUring = RING3;
	newProcess->state = T_RUNNING;
	newProcess->isParent = TRUE;
	newProcess->threadParent = newProcess;
	newProcess->tid = 0;
	newProcess->pid = GetNewPID();
	RegisterPID(newProcess->pid, newProcess);

	// initialize thread's memory management
	PrepareUserPaging(newProcess);
	return newProcess;
}

/**
 * Same as ThreadExit but also cares about pid/tid handles
 * @param thread Thread to kill
 * @param exitCode exit code
 */
void ThreadExitUserMode(PROCESS_PTR thread, SINT32 exitCode) {
	UnregisterThreadHandle(thread);
	ThreadExit(thread, exitCode);
}

/**
 * Frees all allocated memory and removes thread from scheduler.
 * Stops all children threads as well.
 * @param process Process to exit
 * @param exitCode The process' exit code it posted (currently unused)
 */
void ProcessExit(PROCESS_PTR process, UNUSED SINT32 exitCode) {
	PROCESS_PTR child;

	AtomicSectionStart();


		// it must not be scheduled again, before it can be removed
		if (!SchedulerRemoveNode(process)) {
			KernelThrowException("ExitProcess:: Tried to remove a node which doesn't exist!!",EXC_ILLEGAL_STATE,0,0,0);
		}

		// Kill all children (please, do NOT take this literally!)
		while (process->threads.threadElements) {
			child = process->threads.threadRoot;
			// NOTE: We don't need to remove it from the list, as
			// the ThreadExit-function takes care of this
			ThreadExitUserMode(child, EXIT_CODE_CHILD);
		}

		// stop asynchronous messages
		RMIUnregister(process);

		IRQUnregisterThread(process);

		PROCESS_PTR unregistered = UnregisterPID(process->pid);
		if (unregistered != process) {
			KernelThrowException("ProcessExit: PID mismatch", EXC_ILLEGAL_STATE, process->pid, FALSE, NULL);
		}

		UserFree(process->userMemory, process->userStack, MEMTYPE_DATA, TRUE);		// free user's own stack - harmless
		UserFree(process->userMemory, process->codeStart, MEMTYPE_CODE, TRUE);
		KFree(process->TIB, MEMTYPE_DATA, TRUE);
		FreeThreadMemory(process);

		if (currTask == process) {
			SchedulerRelinquish();
		}

		KFree(process, MEMTYPE_DATA, FALSE);
	AtomicSectionEnd();
}

/**
 * Frees stack and deletes thread from scheduler's list
 * @see ProcessExit
 * @param thread thread to exit
 * @param exitCode exit code
 */
void ThreadExit(PROCESS_PTR thread, SINT32 exitCode) {
	PROCESS_PTR parent;

	ASSERT(thread->semaphores.semaphore == 0);
	ASSERT(thread->semaphores.semaphoreNext == NULL);

	ThreadEngageWaitingJoins(thread, exitCode);

	if (!thread->isParent) {
		AtomicSectionStart();

			if (thread->currRMI.sender) {
				// thread was started due to an RMI request
				ASSERT(thread != thread->threadParent);
				if (thread->currRMI.expectsReturn) {
					PostThreadMessage(thread, exitCode);
				}
				RMIDequeueWorker(thread);
			}

			if (!SchedulerRemoveNode(thread)) {
				KernelThrowException("ExitThread:: Tried to remove a node which doesn't exist!!",EXC_ILLEGAL_STATE,0,0,0);
			}
			parent = thread->threadParent;
			parent->numThreads--;

			// add the CPU usage consumed by the thread to it's parent
			parent->runTimeCycles += thread->runTimeCycles;

			// dequeue this thread from it's parent
			DLListRemoveNode(parent->threads.thread, threadItem.child, thread);


			// Free user's stack - it's harmless
			UserFree(thread->userMemory, thread->userStack, MEMTYPE_DATA, TRUE);
			if (currTask == thread) {
				SchedulerRelinquish();
			}
			KFree(thread, MEMTYPE_DATA, TRUE);

		AtomicSectionEnd();

	} else {
		// it's the parent (the process itself)
		ProcessExit(thread, exitCode);
	}
}
