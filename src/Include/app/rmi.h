#ifndef _RMI_H
#define _RMI_H

/**
 * @file rmi.h
 * @author Stefan Nuernberger
 * @date Fr Nov 17 2006
 * @see rmi.c
 * @brief Prototypes of export-management and remote method invocation
 */

// #include <memory/memfunc.h>
#include <mtask/privileges.h>
#include <mtask/task_types.h>
#include <memory/arguments.h>
#include <app/ipc_types.h>
#include<app/ipc_user.h>


/**
 * initializes the RMI subsysten
 * @note only called by the kernel itself
 */
void KernelInitRMI(void);

/**
 * Unregisters a serial from the given thread
 * @param thread to unregister for RMI
 */
BOOL RMIUnregister(PROCESS_PTR thread);

/**
 * Registers a RMI for a particular process
 * @param process that wants to register a RMI
 * @param exportUID unique ID of the function (must only be unique inside the process)
 * @param minPriv minimum privilege level the caller needs to be allowed to call that export
 * @param partnership defines if application partnership is needed
 * @param msgCall Expected parameter list and entry points including message type to receive
 * @return RAY_RMI
 * @see RMInvoke for opposite
 */
RAY_RMI RMIRegister(PROCESS_PTR process, PRIVLEVEL minPriv, BOOL partnership, MSG_CALL* msgCall) ;


RAY_RMI RMPassMessage(PROCESS_PTR thread, RMISERIAL remoteSerial, RMIFUNCTION funcExport, ARGUMENTS msgArgs, BOOL blocking, POINTER msgHandle);

/**
 * Sets up IPC between processes
 * @param thread to setup RMI for
 * @param threadSerial A unique THREADSERIAL we want to use to identify the process
 * @param count Maximum number of RMIs that will be registered
 * @return Status of registering (RAY_RMI)
 * @note Must be done before anything concerning RMI can be used
 */
RAY_RMI RMIExpand(PROCESS_PTR process, RMISERIAL threadSerial, UINT32 count);



RAY_RMI AllocateMessageBuffer(PROCESS_PTR process, UINT32 msgSize, MSG_TYPE type);


RAY_RMI FreeMessageBuffer(PROCESS_PTR process, UINT32 msgBuffer);



void RMIDequeueWorker(PROCESS_PTR worker);

RAY_RMI PostThreadMessage(PROCESS_PTR exitedThread, UINT32 message);

#endif
