#ifndef _THREAD_H
#define _THREAD_H

#include "memory/arguments.h"
#include "mtask/task_types.h"
#include <memory/adt/linkedlist.h>

#define EXIT_CODE_ABORT		-32760
#define EXIT_CODE_CHILD		-32761
#define EXIT_CODE_RMIRESULT	-32762
#define EXIT_CODE_NO_THREAD	-32763

typedef struct {
	UINT32 pid;
	RMISERIAL serial;
	UINT32 numExports;

	UINT32 usedMemory;
	UINT32 usedPages;
	UINT32 memAllocs;
	UINT32 lastFitPointerCode;
	UINT32 lastFitPointerData;

	UINT64 contextSwitches;
	UINT64 cpuCycles;
	UINT64 privCycles;
	THREAD_STATE state;

	UINT32 threadNum;
	BOOL isParent;
	char ident[THREAD_IDENT_LENGTH];
} THREAD_INFO;

typedef struct {
	UINT32 numThreads;
	UINT32 kernelMemory;
	UINT32 memUsageTotal;
	UINT32 memFree;
	THREAD_INFO *thread;
} THREAD_INFO_LIST;

/**
 * @file thread.h
 * @date 09-13-2006
 * @author Stefan Nuernberger
 * @brief Thread management
 */

/**
 * Initializes threading
 */
void InitThreading();

PROCESS_PTR SpawnProcess();
void PrepareThread(PROCESS_PTR newThread, POINTER functionAddress, POINTER returnAddress, PRIORITY priority, ARGUMENTS args);
PROCESS_PTR CreateThreadFromExisting(PROCESS_PTR oldThread, POINTER functionAddress, POINTER returnAddress, PRIORITY priority, ARGUMENTS args, CString threadName);
void ThreadExit(PROCESS_PTR thread, SINT32 exitCode);
void ThreadExitUserMode(PROCESS_PTR thread, SINT32 exitCode);
void ProcessExit(PROCESS_PTR thread, SINT32 exitCode);
void ThreadAddJoinList(PROCESS_PTR thread, PROCESS_PTR waitingThread);
THREAD_INFO_LIST* GetThreadInfo(UINT32 pid);
BOOL IsThreadAlive(PID_HANDLE* handle);

/**
 * Stack size for kernel and user mode (in UINT32s!)
 */
#define APP_USER_STACK_SIZE 1024


#endif
