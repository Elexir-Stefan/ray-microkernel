/**
 * @file pid_types.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Fr Aug 1 2008
 * @see pid_types.c
 * @brief Summary
 */

#ifndef _pid_types_H
#define _pid_types_H

typedef UINT32 PID;
typedef UINT32 TID;

typedef struct {
	PID pid;
	TID tid;
	BOOL isProcessParent;
	UINT32 handle;
} PID_HANDLE;

#define PID_DOES_NOT_EXIST	0

#endif
