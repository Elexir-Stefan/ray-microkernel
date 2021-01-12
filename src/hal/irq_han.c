/**
 * @file irq_han.c
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief Generic IRQ handler for IRQs 0 - 15 and associated
 * serive routines for a certain IRQ
 */

#include <typedefs.h>
#include <hal/8259.h>
#include <memory/paging.h>
#include <mtask/task_types.h>
#include <mtask/scheduler.h>
#include <app/callbacks.h>
#include <hal/irq_han.h>
#include <hal/x86regs.h>
#include <memory/threadmemory.h>
#include <memory/memory.h>
#include <exceptions.h>
#include <debug.h>

#define USEGLOBALCONFIG
#include <globalconfig.h>

#define NUM_IRQS	16

#define MAX_PENDING_IRQS	1

extern PROCESS_PTR currTask;

// include debugger support
#ifdef RAY_KERNEL_DEBUGGER
#include <hal/uart.h>
#include <kdebug/kdebug.h>
#include <kdebug/i386-stub.h>
#include <video/video_char.h>
extern COM_PORT debuggerPort;
#endif

REGISTERED_HANDLER *userHandlers;

void IRQHandlerInit(void) {
    UINT16 irqNum;

	userHandlers = (REGISTERED_HANDLER*)KMalloc(NUM_IRQS * sizeof(REGISTERED_HANDLER), MEMTYPE_DATA);
	for (irqNum = 0; irqNum < NUM_IRQS; irqNum++) {
		userHandlers[irqNum].thread = NULL;
		userHandlers[irqNum].numPending = 0;
	}

}

void IRQHandler() {
	INT_REG *regs = currTask->regPointer;
	UINT32 irqNum = regs->intNum;

	userHandlers[irqNum].numPending++;

	if (userHandlers[irqNum].numPending > MAX_PENDING_IRQS) {
		KernelThrowException("IRQHandler: An IRQ fired up without being handled!", EXC_ILLEGAL_STATE, irqNum, EXC_FLAG_SHOWREGS, regs);
	}

#ifdef RAY_KERNEL_DEBUGGER
	if (irqNum == debuggerPort.irq) {
		//KernelThrowException("COM1-IRQ received!", EXC_ILLEGAL_STATE, 0, 0, 0);
		C8259InterruptDone(irqNum);
		if (!debuggerPort.isReading) {
			int data = getDebugChar();
			if (data == 0x03) {
				BREAKPOINT();
			}
		}
	} else {
#endif

	if (userHandlers[irqNum].thread) {
		// This code defers the actual handling of the interrupt to a user mode
		// process (driver). Before the process/thread gets a chance to execute,
		// this ISR returns from kernel mode, which leaves room for potential new
		// interrupts by the same device. Consequently, we mask the device to force
		// it to be quiet until the driver handled the interrupt properly.
		C8259MaskIRQ(irqNum);

		ASSERT(userHandlers[irqNum].numPending == 1);
		SchedulerOverrideThreadState(userHandlers[irqNum].thread, T_RUNNING);
		Schedule();

	} else {

		if (GCONFIG_UNEXPECTED_IRQS) {
			KernelThrowException("Spurious interrupt!", EXC_UNKNOWN_INTERRUPT, irqNum, EXC_FLAG_SHOWREGS, regs);
		}
	}

	// Always acknowledge the PIC that we handled the interrupt
	// Otherwise no other interrupts could occur.
	C8259InterruptDone(irqNum);


#ifdef RAY_KERNEL_DEBUGGER
	}
#endif
}

RAY_TDM IRQRegisterHandler(UINT8 irqNum, PROCESS_PTR thread) {

	if (thread == thread->threadParent)
	{
		// IRQ deferred procedure calls must be started in a separate thread!
		ASSERT(FALSE);
		return TDM_IMPOSSIBLE;
	}

	// check for valid IRQ number (timer IRQ cannot be hooked on)
	if ((irqNum > 0) && (irqNum <= 15)) {
		// check if handler isn't already occupied
		if (userHandlers[irqNum].thread) {
			ASSERT(FALSE);
			return TDM_IMPOSSIBLE;
		}

		// save callbacks
		userHandlers[irqNum].thread = thread;
		return TDM_SUCCESS;
	} else {
		ASSERT(FALSE);
		return TDM_ILLEGAL_ARGUMENT;
	}
}

RAY_TDM IRQAcknowledgeAndWait(UINT8 irqNum, PROCESS_PTR thread) {

	if (irqNum > 15) {
		ASSERT(FALSE);
		return TDM_ILLEGAL_ARGUMENT;
	}
	if (userHandlers[irqNum].thread != thread) {
		// Only dispatcher threads are allowed to handle IRQs.
		ASSERT(FALSE);
		return TDM_IMPOSSIBLE;
	}

	// This function gets called in two cases:
	// a) The first invocation of the IRQ handler in order
	//    to wait for interrupts to come
	// b) After an interrupt has been handled by the process
	//    and this function is called again in the outer loop.
	//
	// In both cases, the IRQ has to unmaksed, may it be the
	// first time (a) so that it actually fires, or may it be
	// any other time (b) in order to tell the PIC that we
	// handled this interrupt.
	C8259UnmaskIRQ(irqNum);

	SchedulerOverrideThreadState(currTask, T_IDLE);
	SchedulerRelinquish();

	return TDM_SUCCESS;

}


RAY_TDM IRQUnregisterHandler(UINT8 irqNum, PROCESS_PTR thread) {
	if (irqNum > 15) {
		ASSERT(FALSE);
		return TDM_ILLEGAL_ARGUMENT;
	}
	if (userHandlers[irqNum].thread != thread) {
		// Only dispatcher threads are allowed to handle IRQs.
		ASSERT(FALSE);
		return TDM_IMPOSSIBLE;
	}

	// unregister callback
	userHandlers[irqNum].thread = NULL;

	// mask IRQ again (prevents spurious interrupts)
	C8259MaskIRQ(irqNum);

	return TDM_SUCCESS;
}

BOOL IRQUnregisterThread(PROCESS_PTR thread) {
	BOOL found = FALSE;
	UINT16 irqNum;

	for (irqNum = 0; irqNum < NUM_IRQS; irqNum++) {
		if (userHandlers[irqNum].thread == thread) {
			IRQUnregisterHandler(irqNum, thread);
			found = TRUE;
		}
	}
	return found;
}
