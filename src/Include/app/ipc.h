/**
 * @file ipc.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Apr 7 2008
 * @see ipc.c
 * @brief Summary
 */

#ifndef _ipc_H
#define _ipc_H

#include <app/ipc_types.h>
#include <mtask/task_types.h>
#include <app/rmi.h>

#define MAX_IPC_PLUGS	16
#define MAX_IPC_CABLES	32

RAY_RMI CreateWall(PROCESS_PTR process, CString wallName, CString description);
RAY_RMI CreateSocket(PROCESS_PTR thread, IPSOCKET_USER* socket);
RAY_RMI RemoveSocket(PROCESS_PTR thread, CString socketName);
RAY_RMI PlugPlugByThread(PROCESS_PTR startThread, PROCESS_PTR destinationThread, CString plugStart, CString plugDestination, CABLE_USER** outCable);
RAY_RMI PlugPlugByPID(PROCESS_PTR source, CString plugStart, CString plugDest, UINT32 pid, CABLE_USER** outCable);
RAY_RMI PlugPlugByName(PROCESS_PTR source, CString plugStart, CString plugDest, CString descriptiveName, CABLE_USER** outCable);
CABLE_USER* GetRMICable(PROCESS_PTR thread);
UINT32 GetAllRMICables(PROCESS_PTR thread, CString socketName, CABLE_USER** list);

#endif
