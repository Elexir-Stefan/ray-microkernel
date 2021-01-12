/**
 * @file threadmemory.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Aug 25 2007
 * @see threadmemory.c
 * @brief Summary
 */

#ifndef _threadmemory_H
#define _threadmemory_H

#include <mtask/task_types.h>


/**
 * Pretends a function call a the remote thread with two arguments: Sender and argument
 * @param remoteThread Thread on wich the call should be pretended
 * @param functionAddress Memory address where a function starts
 * @param sender RMISERIAL of the initiator
 * @param argument Abitrary value
 * @param msgID unique communication ID stored in hash map for returning result of callee to caller
 * @return RMI_SUCCESS on success, RMI_OVERLOAD if not enough stack size on remote thread left
 */
RAY_RMI SignalThread(PROCESS_PTR remoteThread, UINT32* functionAddress, UINT32 sender, UINT32 argument, UINT32 msgID);


void ProcessWriteAddress(PROCESS_PTR process, UINT32 *pointer, UINT32 value);

#endif
