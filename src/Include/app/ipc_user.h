/*
 * @brief IPC user mode data structures
 */

#ifndef IPC_USER_H_
#define IPC_USER_H_

#include <app/rmi_types.h>

typedef struct {
	RMISERIAL destinationSerial;
	RMIFUNCTION* destinationFunctionNumbers;
} CABLE_USER;

typedef struct {
	UINT32 count;
	CABLE_USER* userCable;
} CABLE_USER_LIST;

typedef struct {
	UINT32 *entryPoint;
	MSG_PARAM* parameters;
	UINT32 paramCount;
	MSG_PARAM returnValue;
} PACKED MSG_CALL_USER;

typedef struct {
	UINT32 numRegisteredFuncs;
	MSG_CALL_USER* functions;
	PRIVLEVEL minPriv;
	BOOL partnership;
} PACKED CALLBACKS_USER;



typedef struct _IPSOCKET_USER{
	CString socketName;					/// Public name of the socket (must only be unique within the application)
	CALLBACKS_USER expectedRemoteFunctions;	/// List of functions the socket expects the other process to offer
	CALLBACKS_USER localCallbacks;			/// List of functions (and their parameters and return types) the socket offers
	PRIVLEVEL minNeeded;				/// The minimum privilege level the other process must have
	BOOL forcePairing;					/// Disable use of this socket for unauthenticated processes
} PACKED IPSOCKET_USER;

#endif /* IPC_USER_H_ */
