/**
 * @file event_types.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Mär 15 2008
 * @see event_types.c
 * @brief Summary
 */

#ifndef _event_types_H
#define _event_types_H

#include <mtask/task_types.h>

typedef struct _EVENT_NODE {
	UINT32 ticksTillExpired;
	PROCESS_PTR thread;
	struct _EVENT_NODE *evNext;
	struct _EVENT_NODE *evPrev;
} EVENT_NODE;

#endif
