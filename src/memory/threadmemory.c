/**
 * @file threadmemory.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Aug 25 2007
 * @see threadmemory.h
 * @brief Summary
 */

#include "typedefs.h"

#include <memory/threadmemory.h>
#include <memory/memory.h>
#include <memory/mmu.h>
#include <mtask/semaphore.h>
#include <mtask/scheduler.h>
#include <exceptions.h>

#include <debug.h>

extern IPLOCK M_PhysicalAccess;

RAY_RMI SignalThread(PROCESS_PTR remoteThread, UINT32* functionAddress, UINT32 sender, UINT32 argument, UINT32 msgID) {
	if (remoteThread->regs.userESP > (UINT32)remoteThread->userStack + 100) {
		UINT32 oldReturnAddress = remoteThread->regs.eip;
		UINT32 *currentStack = (UINT32*)remoteThread->regs.userESP;

		// push values on stack
		ProcessWriteAddress(remoteThread, --currentStack, msgID);
		ProcessWriteAddress(remoteThread, --currentStack, argument);
		ProcessWriteAddress(remoteThread, --currentStack, sender);
		ProcessWriteAddress(remoteThread, --currentStack, oldReturnAddress);

		remoteThread->regs.userESP = (UINT32)currentStack;
		remoteThread->regs.eip = (UINT32)functionAddress;

		SchedulerOverrideThreadState(remoteThread, T_RUNNING);

		// WARNING: The caller should take care of this!
		// SchedulerSelectThread(remoteThread);
		// Actually, be calling Schedule() not a specific thread (more fair)

		return RMI_SUCCESS;
	} else {
		ASSERT(FALSE);
		return RMI_OVERLOAD;
	}
}

/**
 * Writes a value at the given virtual address in the process' memory
 * @param process process to write to
 * @param pointer address to write at
 * @param value to write
 */
void ProcessWriteAddress(PROCESS_PTR process, UINT32 *pointer, UINT32 value) {
	UINT32 physicalPageAddress;
	UINT32 *virtualPageAddress;
	UINT32 offset;

	physicalPageAddress = GetPhysicalAddressOf(process->userMemory->pageDirVirtAddress, pointer) & PAGE_NO_OFFSET;
	if (physicalPageAddress) {
		offset = (UINT32)pointer % PAGE_SIZE;

		SemaphoreEnter(M_PhysicalAccess); {
			// open whole page
			virtualPageAddress = OpenPhysMemory((UINT32*)physicalPageAddress, TRUE);
			// add offset (ATTENTION! virtualPageAddress is UINT32 (32 bits), while offset (and pointer) is in bytes (8 bits))
			virtualPageAddress = (UINT32*)((UINT32)virtualPageAddress + offset);
			*virtualPageAddress = value;
		}
		SemaphoreLeave(M_PhysicalAccess);
	} else {
		KernelThrowException("ProcessWriteAddress:: Cannot write to unknown address!", EXC_OUT_OF_MEMORY, (UINT32)pointer, 0, 0);
	}
}
