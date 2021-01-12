/**
 * @file ipc_types.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Apr 7 2008
 * @see ipc_types.c
 * @brief Summary
 */

#ifndef _ipc_types_H
#define _ipc_types_H

typedef struct _CABLE CABLE;

#include <mtask/privileges.h>
#include <app/rmi_types.h>
#include <app/ipc_user.h>
#include <mtask/task_types.h>

typedef struct {
	UINT32 numRegisteredFuncs;
	MSG_CALL* functions;
	PRIVLEVEL minPriv;
	PROCESS_PTR remoteProcess;
	BOOL partnership;
} PACKED CALLBACKS;

struct _CABLE{
	CABLE_USER content;
	struct _CABLE* conjugate;
	struct _CABLE* next;
	struct _IPSOCKET* socket;
	CABLE_USER* userModeCable;
};

typedef struct _IPSOCKET{
	CString socketName;					/// Public name of the socket (must only be unique within the application)
	CALLBACKS expectedRemoteFunctions;	/// List of functions the socket expects the other process to offer
	CALLBACKS localCallbacks;			/// List of functions (and their parameters and return types) the socket offers
	PRIVLEVEL minNeeded;				/// The minimum privilege level the other process must have
	BOOL forcePairing;					/// Disable use of this socket for unauthenticated processes
	PROCESS_PTR parentThread;
	CABLE* cables;
} PACKED IPSOCKET;


#endif
