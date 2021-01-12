/**
 * @file isr.c
 * @date 02-25-2006
 * @brief Interrupt Service Routines for Processor Exceptions
 */


#include "typedefs.h"
#include "exceptions.h"
#include "hal/int.h"
#include "hal/irq_han.h"
#include "hal/processor.h"

BOOL globalDoubleFault = FALSE;

/**
 * interrupt handler for int 0
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_DivZero(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Division by Zero", 0, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 56);
}

/**
 * interrupt handler for int 1
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_Debug(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Debug Exception", 1, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 2
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_NonMaskableInterrupt(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Non maskable Interrupt", 2, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 3
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_Breakpoint(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Breakpoint Exception", 3, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 4
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_Overflow(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Overflow", 4, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 5
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_OutOfBounds(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Out of Bounds", 5, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 6
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_InvalidOpCode(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Invalid Opcode", 6, (UINT32)&regs, EXC_FLAG_SHOWREGS, &regs);
}

/**
 * interrupt handler for int 7
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_No387(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("No Co-Processor", 7, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 8
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_DoubleFault(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Double Fault", 8, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 9
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_387SegOverrun(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Co-Processor: Overrun", 9, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 10
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_BadTSS(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Bad TSS", 10, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 11
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_SegmentNotPresent(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Segment not present", 11, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 12
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_StackFault(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Stack Fault Exception", 12, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}


/**
 * interrupt handler for int 13
 * @param regs Registers before throwing the interrupt
 */
/*void ISR_exc_GeneralProtectionFault(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("General Protection Fault", 13, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}*/

/**
 * interrupt handler for int 15
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_UnknownInt(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Interrupt occured", 15, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 16
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_387Fault(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Co-Processor: Fault Exception", 16, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 17
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_Alignment(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Alignment Exception", 17, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 18
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_Machine(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Machine Exception", 18, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 19
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED19(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 19, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 20
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED20(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 20, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 21
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED21(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 21, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 22
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED22(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 22, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 23
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED23(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 23, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 24
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED24(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 24, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 25
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED25(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 25, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 26
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED26(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 26, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 27
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED27(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 27, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 28
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED28(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 28, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 29
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED29(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 29, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 30
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED30(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 30, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}

/**
 * interrupt handler for int 31
 * @param regs Registers before throwing the interrupt
 */
void ISR_exc_RESERVED31(INT_REG regs) {
	KernelInterruptsDisable();
	KernelThrowException("Unknown Exception!", 31, (UINT32)&regs, EXC_FLAG_SHOWREGS, (void*)&regs - 52);
}
