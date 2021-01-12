#ifndef _TASK_TYPES_H
#define _TASK_TYPES_H

/**
 * @file task_types.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Type declaration of threads and tasks
 * Types and tructures used for the scheduler
 */

typedef struct schedNode* PROCESS_PTR;

#include <memory/paging.h>
#include <mtask/semaphore.h>
#include <mtask/privileges.h>
#include <hal/x86regs.h>
#include <mtask/pid_types.h>
#include <memory/adt/linkedlist.h>
#include <app/ipc_types.h>

#define THREAD_IDENT_LENGTH	64

typedef enum {
	T_RUNNING = 0,
	T_IDLE = 1,
	T_WAITING = 2,
	T_MSG_RECV = 3,
	T_EVENT = 4
} __attribute__((packed)) THREAD_STATE;

typedef enum {
	RING0 = 0,
	RING3 = 1
} CPURING;

typedef struct _JoinElement {
	struct schedNode* thread;
	struct _JoinElement* waitingNext;
	struct _JoinElement* waitingPrev;
} JoinElement;

CreateList(ThreadList, thread, struct schedNode);
CreateListItem(ThreadListItem, child, struct schedNode);

CreateList(JoinList, thread, JoinElement);

/**
 * User mode mapped TIB with some information
 * NOTE: Warning: This structure will be writable for user
 * more programs in order to write in kernel mode
 * (as the kernel has to be obey user mode write-protection
 * it's not possible otherwise)
 */
typedef struct _ThreadInformationBlock
{
	PID pid;
	TID tid;
}ThreadInformationBlock;

struct schedNode{
	CPURING CPUring;

	UINT32 *userStack;
	UINT32 *codeStart;
	UINT32 *bssSection;
	INT_REG regs;
	MEM_MANAGE *userMemory;
	INT_REG *regPointer;

	/** IMPORTANT - DO NOT CHANGE TILL HERE! HARDCODED POSITIONS IN IRQ.S!!! */

	ThreadInformationBlock* TIB;

	THREAD_STATE state;
	PRIORITY priority;
	PRIORITY currPrio;
	PRIVLEVEL privLevel;

	UINT64 runTimeStart;
	UINT64 privilegedTimeStart;
	UINT64 runTimeCycles;
	UINT64 privilegedCycles;
	UINT64 contextSwitches;

	UINT8 certificate;

	PID pid;
	TID tid;
	UINT32 pidHandle;
	char ident[THREAD_IDENT_LENGTH];

	SEMAPHORELIST semaphores;

	RMISERIAL appSerial;
	HASH* plugs;
	HASH* cablesByRemoteSerial;
	HASH* msgHash;
	HASH* barriers;
	RMETHOD *RMIs;
	UINT32 numRMIs;
	UINT32 usedRMIs;
	UINT32 rmiQueuedWorkers;
	RMIWORK currRMI;
	void* standardReturnPoint;

	BOOL isParent;
	UINT32 numThreads;
	UINT32 argCount;
	struct schedNode* threadParent;
	ThreadList threads;
	ThreadListItem threadItem;

	JoinList joined;

	BOOL locked;

	struct schedNode* next;
	struct schedNode* waitingNext;
	struct schedNode* waitingPrev;
};



#endif
