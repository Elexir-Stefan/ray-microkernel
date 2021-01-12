/**
 * @file 8259.c
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @date 02-25-2006
 * @brief Routines for communication with the 8259 programmable interrupt controller (PIC)
 */

#include <typedefs.h>
#include <hal/io.h>
#include <hal/8259.h>
#include <hal/8259_ctrl.h>
#include <hal/irq_han.h>
#include <exceptions.h>

extern REGISTERED_HANDLER *userHandlers;

static void FlipIRQMaskBit(UINT8 irqNum, BOOL enable);

void C8259InterruptDone(UINT8 irq) {
	if (irq >= 8) OutPortB (CTL_8259A_2, CTL_EOI);
	OutPortB (CTL_8259A_1, CTL_EOI);
	userHandlers[irq].numPending--;
}


void C8259Init(UINT8 low_start, UINT8 high_start) {
	/* initialize controllers */
	OutPortB (CTL_8259A_1, CTL_INIT);
	OutPortB (CTL_8259A_2, CTL_INIT);

	/* send int start address to both controllers */
	OutPortB (CTL_8259A_1REG, low_start);
	OutPortB (CTL_8259A_2REG, high_start);
}


void C8259_1_Master(void) {
	OutPortB (CTL_8259A_1REG, CTL_MASTER);
}


void C8259_1_Slave(void) {
	OutPortB (CTL_8259A_1REG, CTL_SLAVE);
}


void C8259_2_Master(void) {
	OutPortB (CTL_8259A_2REG, CTL_MASTER);
}


void C8259_2_Slave(void) {
	OutPortB (CTL_8259A_2REG, CTL_SLAVE);
}

static void FlipIRQMaskBit(UINT8 irqNum, BOOL enable) {
	UINT8 currMask;
	UINT16 ctrlPort;
	UINT8 irqBit;

	if (irqNum > 15) {
		KernelThrowException("C8259UnmaskIRQ: IRQ > 15 supplied.", EXC_UNKNOWN_INTERRUPT, irqNum, 0, 0);
	}

	if (irqNum < 8) { /* 0 to 7 */
		ctrlPort = CTL_8259A_1REG;
		irqBit = 1 << irqNum;
	} else { /* 8 to 15 */
		ctrlPort = CTL_8259A_2REG;
		irqBit = 1 << (irqNum - 8);
	}

	/* save current masking info of irqs */
	currMask = InPortB (ctrlPort);

	if (enable) {
		/* clear bit if it is set */
		currMask &= ~irqBit;
	} else {
		/* set bit if it is cleared */
		currMask |= irqBit;
	}
	OutPortB (ctrlPort, currMask);

}

void C8259UnmaskIRQ(UINT8 irqNum) {
	FlipIRQMaskBit(irqNum, TRUE);
}


void C8259MaskIRQ(UINT8 irqNum) {
	FlipIRQMaskBit(irqNum, FALSE);
}


void C8259_1_Mode8086(void) {
	OutPortB (CTL_8259A_1REG, CTL_M8086);
}


void C8259_2_Mode8086(void) {
	OutPortB (CTL_8259A_2REG, CTL_M8086);
}

void C8259SetIRQmaskLO(UINT8 irqMask) {
	OutPortB (CTL_8259A_1REG, irqMask);
}

void C8259SetIRQmaskHI(UINT8 irqMask) {
	OutPortB (CTL_8259A_2REG, irqMask);
}
