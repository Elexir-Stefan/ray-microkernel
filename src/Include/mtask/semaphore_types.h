/**
 * @file semaphore_types.h
 * @brief Internal data types for semaphores and barriers
 */


#ifndef SEMAPHORE_TYPES_H_
#define SEMAPHORE_TYPES_H_

#include <mtask/task_types.h>
#include <memory/adt/linkedlist.h>



typedef struct {
	SINT32 counter;
	SINT32 initialValue;
	PROCESS_PTR waitingRoot;
	PROCESS_PTR waitingTail;
	UINT32 waitingElements;
	PROCESS_PTR creator;
	BOOL othersAllowed;
} SEMAPHORE;

typedef struct {
	UINT32 hash;
	UINT32 totalEverWaited;
	BOOL open;
	BOOL othersAllowed;
	PROCESS_PTR owner;

	PROCESS_PTR waitingRoot;
	PROCESS_PTR waitingTail;
	UINT32 waitingElements;
} BARRIER;

#endif /* SEMAPHORE_TYPES_H_ */
