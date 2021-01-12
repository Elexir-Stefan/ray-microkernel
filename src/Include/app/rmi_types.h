/**
 * @file rmi_types.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Aug 18 2007
 * @see rmi.c
 * @brief Types used by the RMI system
 */

#ifndef _rmi_types_H
#define _rmi_types_H

#include <typedefs.h>
#include <mtask/privileges.h>
#include <mtask/threadqueue.h>
#include <app/ipc_types.h>


/** Static type for thread identification - Don't mess up with PID which is dynamically set up */
typedef UINT32 RMISERIAL;
/** Unique (per process) number for functions */
//typedef UINT32 RMIFUNCTION;
NewType(UINT32, RMIFUNCTION);

typedef enum {
	RTYPE_UINT8 = 1,
	RTYPE_SINT8 = 2,
	RTYPE_UINT16 = 3,
	RTYPE_SINT16 = 4,
	RTYPE_UINT32 = 5,
	RTYPE_SINT32 = 6,
	RTYPE_FLOAT = 7,
	RTYPE_CHAR = 8,
	RTYPE_STRING = 9,
	RTYPE_VOID = 10,
	RTYPE_MEM = 11,
} PACKED MSG_TYPE;

#define SENTBYREF(type) ((type == RTYPE_MEM) || (type == RTYPE_STRING))
#define SENTBYVALUE(type) (!(SENTBYREF(type)))



typedef struct {
	MSG_TYPE type;
	UINT32 maxSize;
} PACKED MSG_PARAM;

#include <app/ipc_user.h>

typedef struct {
	MSG_CALL_USER content;
	RMIFUNCTION assignedIndex;
	RMIFUNCTION remoteIndex;
} PACKED MSG_CALL;

typedef struct{
	PRIVLEVEL privNeeded;
	MSG_CALL* functionCall;
	BOOL partnership;
	ThreadQueue waitQueue;
	UINT32 totalWorkers;
} RMETHOD;

typedef struct {
	struct schedNode* sender;
	RMIFUNCTION issuedRMIndex;
	POINTER msgSenderHandle;
	MSG_TYPE expectedReturnType;
	UINT32 expectedReturnSize;
	BOOL expectsReturn;
	CABLE* sendersCable;
} RMIWORK;

/**
 * Possible return values for Remote Method Invokation
 */
typedef enum {
	RMI_SUCCESS = 0,			/**< everything is OK */
	RMI_EXPORT_NOT_FOUND = 1,	/**< There is no function with that number registered at the receiver's thread */
	RMI_TRANSMIT_ERROR = 2,		/**< General error concerning method invocation */
	RMI_GEN_ERROR = 3,			/**< An unknown error occurred */
	RMI_WRONG_SETUP = 4,		/**< Too many functions exported for current RMISetup */
	RMI_NO_SETUP = 5,			/**< Must be initialized with RMISetup first! */
	RMI_NO_SERIAL = 6,			/**< Sender has not set up RMI! */
	RMI_INSUFF_RIGHTS = 7,		/**< Insufficient rights to send a message to that thread */
	RMI_NOT_SUPPORTED = 8,		/**< Remote thread has not set up RMI */
	RMI_OCCUPIED = 9,			/**< Exported function number already in use */
	RMI_OVERLOAD = 10,			/**< Calling remote thread would cause stack problems at remote thread */
	RMI_OUT_OF_MEMORY = 11,		/**< Out of memory */
	RMI_ARGUMENT_COUNT_MISMATCH = 12,
	RMI_ARGUMENT_TYPE_MISMATCH = 13,
	RMI_ARGUMENT_SIZE_MISMATCH = 14,
	RMI_INCOMPATIBLE = 15,
	RMI_ARGUMENT_NO_GIFT = 16,
	RMI_INVALID_PROCESS = 17,
	RMI_DOUBLE_CABLE = 18,
	RMSG_INVALID_BUFFER = 20,
	RMSG_INVALID_HANDLE = 21,
	RMSG_OWNERSHIP_ERROR = 22,
	RMSG_SAME_TYPE_NEEDS_MEM = 23

} RAY_RMI;

#define NUM_SERIALS	4096
#define MSG_HASH_SIZE_PER_PROCESS	128

#endif
