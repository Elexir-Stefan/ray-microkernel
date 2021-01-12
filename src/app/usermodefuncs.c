/**
 * @file usermodefuncs.c
 * @author Stefan Nuernberger
 * @date Di Sep 26 2006
 * @brief Wrappers for functions that are called by the SCT from user-mode
 */

#include <typedefs.h>
#include <memory/memory.h>
#include <memory/mmu.h>
#include <hal/syscall.h>
#include <app/sct.h>
#include <app/rmi.h>
#include <mtask/scheduler.h>
#include <mtask/thread.h>
#include <mtask/semaphore.h>
#include <mtask/pid.h>
// #include <modules/loadexecutable.h>
#include <app/usermodefuncs.h>
#include <app/wrappers.h>
#include <app/ioperm.h>
#include <app/ipc.h>
#include <memory/protection.h>
#include <app/callbacks.h>
#include <hal/irq_han.h>
#include <hal/8259.h>
#include <kdebug/i386-stub.h>
#include <kdebug/debugbuffer.h>
#include <profiler/useprofile.h>
#include <hal/system.h>

#include <debug.h>

extern PROCESS_PTR currTask;

void SCTRegisterKernelFunctions(void) {
	// System
	RegisterSCTFunction(SCT_HARD_RESET, 255, WRAPPER_HardReset, TRUE);

	// Video
	RegisterSCTFunction(SCT_VIDEO_ATTR, 255, WRAPPER_WriteVideoAttr, FALSE);
	RegisterSCTFunction(SCT_PRINTF, 255, WRAPPER_printf, FALSE);

	// Memory
	RegisterSCTFunction(SCT_MALLOC, 255, WRAPPER_malloc, TRUE);
	RegisterSCTFunction(SCT_GET_MEM_SIZE, 255, WRAPPER_GetMemSize, TRUE);
	RegisterSCTFunction(SCT_FREE, 255, WRAPPER_free, TRUE);

	// Threading
	RegisterSCTFunction(SCT_STANDARDRET, 255, WRAPPER_StandardReturnPoint, FALSE);
	RegisterSCTFunction(SCT_RELINQUISH, 255, WRAPPER_Relinquish, FALSE);
	RegisterSCTFunction(SCT_SLEEP, 255, WRAPPER_Sleep, FALSE);
	RegisterSCTFunction(SCT_THREAD_CREATE, 255, WRAPPER_ThreadCreate, TRUE);
	RegisterSCTFunction(SCT_THREAD_EXIT, 255, WRAPPER_ThreadExit, FALSE);
	RegisterSCTFunction(SCT_THREAD_ABORT, 255, WRAPPER_ThreadAbort, TRUE);
	RegisterSCTFunction(SCT_THREAD_GET, 255, WRAPPER_ThreadGet, FALSE);
	RegisterSCTFunction(SCT_THREAD_ALIVE, 255, WRAPPER_ThreadAlive, TRUE);
	RegisterSCTFunction(SCT_THREAD_NOTIFY, 255, WRAPPER_ThreadNotify, TRUE);
	RegisterSCTFunction(SCT_THREAD_INFO, 255, WRAPPER_ThreadInfo, TRUE);
	RegisterSCTFunction(SCT_THREAD_JOIN, 255, WRAPPER_ThreadJoin, TRUE);
	RegisterSCTFunction(SCT_EXIT_PROCESS, 255, WRAPPER_ProcessExit, FALSE);

	// Processes
	RegisterSCTFunction(SCT_SLEEP_INTV, 255, WRAPPER_SleepInterval, FALSE);
	RegisterSCTFunction(SCT_LOAD_PROCESS, 255, WRAPPER_LoadProcess, TRUE);
	RegisterSCTFunction(SCT_GET_PID, 255, WRAPPER_GetPID, TRUE);
	RegisterSCTFunction(SCT_GET_PRIV_LEVEL, 255, WRAPPER_UserGetPrivLevel, TRUE);

	// Synchronization
	RegisterSCTFunction(SCT_SEMAPHORE_CREATE, 255, WRAPPER_SemaphoreCreate, TRUE);
	RegisterSCTFunction(SCT_SEMAPHORE_ENTER, 255, WRAPPER_SemaphoreEnter, TRUE);
	RegisterSCTFunction(SCT_SEMAPHORE_GET, 255, WRAPPER_SemaphoreGet, TRUE);
	RegisterSCTFunction(SCT_SEMAPHORE_LEAVE, 255, WRAPPER_SemaphoreLeave, TRUE);
	RegisterSCTFunction(SCT_SEMAPHORE_STATUS, 255, WRAPPER_SemaphoreStatus, TRUE);
	RegisterSCTFunction(SCT_SEMAPHORE_DESTROY, 255, WRAPPER_SemaphoreDestroy, TRUE);
	RegisterSCTFunction(SCT_BARRIER_ARRIVE, 255, WRAPPER_BarrierArrive, TRUE);
	RegisterSCTFunction(SCT_BARRIER_GO, 255, WRAPPER_BarrierGo, TRUE);
	RegisterSCTFunction(SCT_BARRIER_CLOSE, 255, WRAPPER_BarrierClose, TRUE);
	RegisterSCTFunction(SCT_BARRIER_CREATE, 255, WRAPPER_BarrierCreate, TRUE);


	// Hardware
	RegisterSCTFunction(SCT_REQUEST_IO_PORT, 255, WRAPPER_RequestIOPort, TRUE);
	RegisterSCTFunction(SCT_REGISTER_IRQ, 255, WRAPPER_RegisterIRQ, TRUE);
	RegisterSCTFunction(SCT_INT_WAIT, 255, WRAPPER_AcknowledgeAndWaitForIRQ, TRUE);


	// IPC
	RegisterSCTFunction(SCT_CREATE_WALL, 255, WRAPPER_CreateWall, TRUE);
	RegisterSCTFunction(SCT_CREATE_SOCKET, 255, WRAPPER_CreateSocket, TRUE);
	RegisterSCTFunction(SCT_REMOVE_SOCKET, 255, WRAPPER_RemoveSocket, TRUE);
	RegisterSCTFunction(SCT_PLUG, 255, WRAPPER_PlugByName, TRUE);
	RegisterSCTFunction(SCT_PLUG_PID, 255, WRAPPER_PlugByPID, TRUE);
	RegisterSCTFunction(SCT_ALLOC_MB, 255, WRAPPER_AllocateMB, TRUE);
	RegisterSCTFunction(SCT_FREE_MB, 255, WRAPPER_FreeMB, TRUE);
	RegisterSCTFunction(SCT_REMOTE_METHOD, 255, WRAPPER_RemoteMethod, TRUE);
	RegisterSCTFunction(SCT_GET_RMI_CABLE, 255, WRAPPER_GetRMICable, TRUE);
	RegisterSCTFunction(SCT_GET_ALL_CABLES, 255, WRAPPER_GetAllCables, TRUE);

	// Debugging and profiling
	RegisterSCTFunction(SCT_BREAKPOINT, 255, WRAPPER_Breakpoint, FALSE);
	RegisterSCTFunction(SCT_WRITE_DEBUG_MSG, 255, WRAPPER_WriteDebugMsg, FALSE);
#ifdef RAYPROFILING
	RegisterSCTFunction(SCT_PROFILE_SETUP, 255, WRAPPER_ProfileSetup, FALSE);
	RegisterSCTFunction(SCT_PROFILE_START, 255, WRAPPER_ProfileStart, FALSE);
	RegisterSCTFunction(SCT_PROFILE_STOP, 255, WRAPPER_ProfileStop, FALSE);
	RegisterSCTFunction(SCT_PROFILE_RESET, 255, WRAPPER_ProfileReset, FALSE);
	RegisterSCTFunction(SCT_PROFILE_SIZE, 255, WRAPPER_ProfileSize, TRUE);
	RegisterSCTFunction(SCT_PROFILE_RCOUNT, 255, WRAPPER_ProfileRecordCount, TRUE);
	RegisterSCTFunction(SCT_PROFILE_FLUSH, 255, WRAPPER_ProfileFlush, FALSE);
	RegisterSCTFunction(SCT_PROFILE_GETBUF, 255, WRAPPER_ProfileGetBuffer, FALSE);
#endif

	SealSCTFunctions();

	/**
	 * FIXME: Mutexes
	 * @see underneath -> Mutexes
	*/
}

UINT32 WRAPPER_HardReset(UINT32 *args) {
	return (UINT32)SystemHardResetUser(currTask, args[0]);
}

void WRAPPER_WriteVideoAttr(UINT32 *args) {
	VideoWriteAttribute((UINT8)args[0], (UINT8)args[1], (UINT16)args[2]);
}

void WRAPPER_printf(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	print((String)args[0]);
}

UINT32 WRAPPER_malloc(UINT32 *args) {
	return (UINT32)UserMalloc(currTask->userMemory, args[0], TRUE, PAGE_USER | PAGE_WRITE, MEMTYPE_DATA);
}

UINT32 WRAPPER_free(UINT32 *args) {
	return (UINT32)UserFree(currTask->userMemory, (POINTER)args[0], MEMTYPE_DATA, TRUE);
}

UINT32 WRAPPER_GetMemSize(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return GetMemorySize(currTask->userMemory, (CPOINTER)args[0]);
}

void WRAPPER_Relinquish(UNUSED UINT32 *args) {
	SchedulerRelinquish();
}

UINT32 WRAPPER_RequestIOPort(UINT32 *args) {
	return FlipIOPort((UINT16)args[0]);
}

UINT32 WRAPPER_UserGetPrivLevel(UNUSED UINT32 *args) {
	return (UINT32)currTask->privLevel;
}

UINT32 WRAPPER_RegisterIRQ(UINT32 *args) {
	return IRQRegisterHandler((UINT8)args[0], currTask);
}

UINT32 WRAPPER_AcknowledgeAndWaitForIRQ(UINT32 *args) {
	return IRQAcknowledgeAndWait((UINT8)args[0], currTask);
}

void WRAPPER_Sleep(UNUSED UINT32 *args) {
	/* sleep thread */
	SchedulerOverrideThreadState(currTask, T_IDLE);
	SchedulerRelinquish();

}

UINT32 WRAPPER_AllocateMB(UINT32 *args) {
	return AllocateMessageBuffer(currTask->threadParent, args[0], (MSG_TYPE)args[1]);
}

UINT32 WRAPPER_FreeMB(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return FreeMessageBuffer(currTask->threadParent, args[0]);
}

UINT32 WRAPPER_CreateWall(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	CheckUserModePointers(currTask, args[1]);
	return CreateWall(currTask->threadParent, (CString)args[0], (CString)args[1]);
}

UINT32 WRAPPER_CreateSocket(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return CreateSocket(currTask->threadParent, (IPSOCKET_USER*)args[0]);
}

UINT32 WRAPPER_RemoveSocket(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return RemoveSocket(currTask->threadParent, (CString)args[0]);
}

UINT32 WRAPPER_PlugByName(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	CheckUserModePointers(currTask, args[1]);
	CheckUserModePointers(currTask, args[2]);
	CheckUserModePointers(currTask, args[3]);
	return PlugPlugByName(currTask->threadParent, (CString)args[0], (CString)args[1], (CString)args[2], (CABLE_USER**)args[3]);
}

UINT32 WRAPPER_PlugByPID(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	CheckUserModePointers(currTask, args[1]);
	CheckUserModePointers(currTask, args[2]);
	CheckUserModePointers(currTask, args[3]);
	return PlugPlugByPID(currTask->threadParent, (CString)args[0], (CString)args[1], (PID)args[2], (CABLE_USER**)args[3]);
}

UINT32 WRAPPER_RemoteMethod(UINT32 *args) {
	ARGUMENTS arguments;
	arguments.count = args[2];
	arguments.values = (UINT32*)args[3];
	CheckUserModePointers(currTask, args[3]);
	CheckUserModePointers(currTask, args[5]);
	return RMPassMessage(currTask, (RMISERIAL)args[0], (RMIFUNCTION)args[1], arguments, (BOOL)args[4], (POINTER)args[5]);
}

UINT32 WRAPPER_GetRMICable(UNUSED UINT32 *args) {
	return (UINT32)GetRMICable(currTask);
}

UINT32 WRAPPER_GetAllCables(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	CheckUserModePointers(currTask, args[1]);
	return GetAllRMICables(currTask, (CString)args[0], (CABLE_USER**)args[1]);
}

void WRAPPER_StandardReturnPoint(UINT32 *args) {
	CheckUserModePointersCode(currTask, args[0]);
	currTask->standardReturnPoint = (void*)args[0];
}

UINT32 WRAPPER_ThreadAbort(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	PROCESS_PTR thread = GetThreadByHandle((PID_HANDLE*)args[0]);
	if (thread) {
		ThreadExitUserMode(thread, EXIT_CODE_ABORT);
		return TRUE;
	} else {
		return FALSE;
	}
}

void WRAPPER_ThreadGet(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	PID_HANDLE* handle = (PID_HANDLE*)args[0];
	if (LIKELY(currTask->currRMI.sender)) {
		if (UNLIKELY(currTask->pidHandle)) {
			FillThreadHandle(currTask, handle);
		} else {
			RegisterThreadHandle(currTask, handle);
		}
	} else {
		FillThreadHandle(currTask, handle);
	}
}

UINT32 WRAPPER_ThreadAlive(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return IsThreadAlive((PID_HANDLE*)args[0]);
}

UINT32 WRAPPER_ThreadNotify(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	PROCESS_PTR thread = GetThreadByHandle((PID_HANDLE*)args[0]);
	if (thread) {
		// only allowed on own threads and when thread voluntarily put to sleep
		if ((thread->state == T_IDLE) && (thread->threadParent == currTask->threadParent)) {
			SchedulerOverrideThreadState(thread, T_RUNNING);
			return TRUE;
		} else {
			ASSERT(FALSE);
			return FALSE;
		}
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}

UINT32 WRAPPER_ThreadCreate(UINT32 *args) {
	PROCESS_PTR tempThread;
	CheckUserModePointersCode(currTask, args[0]);
	CheckUserModePointersCode(currTask, args[1]);
	CheckUserModePointers(currTask, args[4]);
	CheckUserModePointers(currTask, args[5]);
	ARGUMENTS arguments;
	arguments.count = args[3];
	arguments.values = (UINT32*)args[4];
	tempThread = CreateThreadFromExisting(currTask, (POINTER)args[0], (POINTER)args[1], (PRIORITY)args[2], arguments, (String)args[5]);
	if (tempThread) {
		SchedulerInsertNode(tempThread);
		PID_HANDLE* handle = (PID_HANDLE*)args[6];
		RegisterThreadHandle(tempThread, handle);
		return TRUE;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}

UINT32 WRAPPER_ThreadJoin(UINT32* args) {
	PROCESS_PTR thread = GetThreadByHandle((PID_HANDLE*)args[0]);
	if (thread) {
		ThreadAddJoinList(thread, currTask);
		SchedulerRelinquish();
		return 0;
	} else {
		ASSERT(FALSE);
		return EXIT_CODE_NO_THREAD;
	}
}

void WRAPPER_ThreadExit(UNUSED UINT32 *args) {
	ThreadExitUserMode(currTask, (SINT32)args[0]);
}

void WRAPPER_ProcessExit(UINT32 *args) {
	ProcessExit(currTask->threadParent, (SINT32)args[0]);
}

UINT32 WRAPPER_ThreadInfo(UINT32 *args) {
	return (UINT32)GetThreadInfo(args[0]);
}

	/**
	 TODO: Mutexes are not safe against priority inversion!
	 @see http://en.wikipedia.org/wiki/Priority_inversion

	 solution:	MutexAcquire(MUTEX mutex, BOOL priorityInheritance)
			While accessing a shared resource the process locking it inherits
			the priority from a higher one trying to access the same data and
			therefore gets locked (prevents a third middle-priority-process from
			blocking the lower (and therefore the higher) process from running)

	*/

UINT32 WRAPPER_SemaphoreCreate(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return SemaphoreCreate((String)args[0], args[1], (BOOL)args[2]);
}

UINT32 WRAPPER_SemaphoreGet(UINT32 *args) {
	CheckUserModePointers(currTask, args[0]);
	return SemaphoreGet((String)args[0]);
}

UINT32 WRAPPER_SemaphoreEnter(UINT32 *args) {
	return SemaphoreEnter((IPLOCK)args[0]);
}

UINT32 WRAPPER_SemaphoreLeave(UINT32 *args) {
	return SemaphoreLeave((IPLOCK)args[0]);
}

UINT32 WRAPPER_SemaphoreDestroy(UINT32 *args) {
	return SemaphoreDestroy((IPLOCK)args[0]);
}


UINT32 WRAPPER_BarrierArrive(UINT32 *args) {
	return (UINT32)BarrierArrive(currTask, (CString)args[0]);
}

UINT32 WRAPPER_BarrierGo(UINT32 *args) {
	return (UINT32)BarrierGo(currTask, (CString)args[0], (BOOL)args[1]);
}

UINT32 WRAPPER_BarrierClose(UINT32 *args) {
	return (UINT32)BarrierClose(currTask, (CString)args[0]);
}

UINT32 WRAPPER_BarrierCreate(UINT32 *args) {
	return (UINT32)BarrierCreate(currTask, (CString)args[0], (BOOL)args[1]);
}

UINT32 WRAPPER_SemaphoreStatus(UINT32 *args) {
	return SemaphoreStatus((IPLOCK)args[0]);
}


void WRAPPER_SleepInterval(UINT32 *args) {
	SchedulerThreadPause(currTask, (UINT32)args[0]);
	SchedulerRelinquish();
}

UINT32 WRAPPER_LoadProcess(UNUSED UINT32 *args) {
	/*PROCESS_PTR newProcess = LoadProcess((LOAD_PROCESS*)args[0]);
	ASSERT(newProcess);
	if (newProcess) {
		return TRUE;
	} else {
		return FALSE;
	}*/
	return 0;
}

UINT32 WRAPPER_GetPID(UNUSED UINT32 *args) {
	return currTask->pid;
}

void WRAPPER_Breakpoint(UNUSED UINT32 *args) {
	BREAKPOINT();
}

void WRAPPER_WriteDebugMsg(UINT32* args)
{
	DebugWriteMessage((String)args[0]);
}

#ifdef RAYPROFILING

void WRAPPER_ProfileSetup(UINT32 *args) {
	ProfilingEnable(args[0]);
}

void WRAPPER_ProfileStart(UNUSED UINT32 *args) {
	ProfilingStart();
}

void WRAPPER_ProfileStop(UNUSED UINT32 *args) {
	ProfilingStop();
}

void WRAPPER_ProfileReset(UNUSED UINT32 *args) {
	ProfilingReset();
}

void WRAPPER_ProfileFlush(UNUSED UINT32 *args) {
	ProfilerFlushCollection();
}

UINT32 WRAPPER_ProfileSize(UNUSED UINT32 *args) {
	return ProfilingGetSize();
}
UINT32 WRAPPER_ProfileRecordCount(UNUSED UINT32 *args) {
	return ProfilingGetUsed();
}

UINT32 WRAPPER_ProfileGetBuffer(UNUSED UINT32* args) {
	return (UINT32)ProfilingCreateUserModeBuffer(currTask);
}

#endif
