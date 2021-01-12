#ifndef _WRAPPERS_H
#define _WRAPPERS_H

/**
 * @file wrappers.h
 * @author Stefan Nuernberger
 * @date 09-26-2006
 * @brief prototypes for the wrappers of 'real' kernel functions
 */

#include "video/video_char.h"
#include "memory/usermem.h"

#define DefWrapper(name) UINT32 WRAPPER_ ## name (UINT32 *args)
#define DefWrapperN(name) void WRAPPER_ ## name (UINT32 *args)

DefWrapper(HardReset);

DefWrapperN(WriteVideoAttr);
DefWrapperN(printf);
DefWrapper(malloc);
DefWrapper(free);
DefWrapper(GetMemSize);

DefWrapperN(Relinquish);
DefWrapper(RequestIOPort);
DefWrapper(CallPrivileged);
DefWrapper(UserGetPrivLevel);
DefWrapper(RegisterIRQ);
DefWrapperN(Sleep);
DefWrapper(AcknowledgeAndWaitForIRQ);

DefWrapper(CreateWall);
DefWrapper(CreateSocket);
DefWrapper(RemoveSocket);
DefWrapper(PlugByName);
DefWrapper(PlugByPID);
DefWrapper(AllocateMB);
DefWrapper(FreeMB);
DefWrapper(RemoteMethod);
DefWrapper(GetRMICable);
DefWrapper(GetAllCables);

DefWrapperN(StandardReturnPoint);
DefWrapper(ThreadCreate);
DefWrapperN(ThreadGet);
DefWrapper(ThreadAbort);
DefWrapper(ThreadNotify);
DefWrapper(ThreadAlive);
DefWrapperN(ThreadExit);
DefWrapper(ThreadJoin);
DefWrapperN(ProcessExit);
DefWrapper(ThreadInfo);
DefWrapper(SemaphoreCreate);
DefWrapper(SemaphoreGet);
DefWrapper(SemaphoreEnter);
DefWrapper(SemaphoreLeave);
DefWrapper(SemaphoreStatus);
DefWrapper(SemaphoreDestroy);
DefWrapper(BarrierArrive);
DefWrapper(BarrierGo);
DefWrapper(BarrierClose);
DefWrapper(BarrierCreate);
DefWrapperN(SleepInterval);
DefWrapper(LoadProcess);
DefWrapper(GetPID);

// Debugging and profiling
DefWrapperN(Breakpoint);
DefWrapperN(WriteDebugMsg);
DefWrapperN(ProfileFlush);
DefWrapperN(ProfileSetup);
DefWrapperN(ProfileStart);
DefWrapperN(ProfileStop);
DefWrapperN(ProfileReset);
DefWrapper(ProfileSize);
DefWrapper(ProfileRecordCount);
DefWrapper(ProfileGetBuffer);

#endif
