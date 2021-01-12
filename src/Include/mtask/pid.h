/**
 * @file pid.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date So Mär 30 2008
 * @see pid.c
 * @brief Summary
 */

#ifndef _pid_H
#define _pid_H

#include <mtask/pid_types.h>

#define NUM_PIDS 100000
#define NUM_PID_HANDLES 100000

/**
 * Initializes structures needed to manage PIDs
 */
void InitPIDs(UINT32 startingPID);

/**
 * @return A new, unused PID
 */
PID GetNewPID();

/**
 * Get a scheduler node by it's PID
 * @param pid
 * @return Scheduler node of PID 'pid'
 */
PROCESS_PTR GetProcessByPID(PID pid);

/**
 * Registers a scheduler node in the PID table
 * @param pid of the node
 * @param node
 * @return TRUE on success, FALSE on error
 */
void RegisterPID(PID pid, PROCESS_PTR node);

/**
 * Removes a scheduler node from the PID table
 * @param pid to remove
 * @return scheduler node on success, NULL otherwise
 */
PROCESS_PTR UnregisterPID(PID pid);

/**
 * Registers a PID handle with a thread/process
 * @param thread Thread to register with
 * @param handle Pointer to a PID_HANDLE to initialize
 */
void RegisterThreadHandle(PROCESS_PTR thread, PID_HANDLE* handle);

/**
 * Unregisters a process/thread handle
 * @param thread Thread/process to unregister
 */
BOOL UnregisterThreadHandle(PROCESS_PTR thread);

/**
 * Returns a thread/process by its handle
 * @param handle Handle to process/thread
 */
PROCESS_PTR GetThreadByHandle(PID_HANDLE* handle);

/**
 * Fills in the information in the PID_HANDLE struct according to the specified process/thread
 * @param thread Thread to gather info from
 * @param handle To fill info in
 */
void FillThreadHandle(PROCESS_PTR thread, PID_HANDLE* handle);

#endif
