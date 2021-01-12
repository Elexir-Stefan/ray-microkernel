/**
 * @file
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief Interrupt Descriptor Table. Generates entries for processor exceptions as well as hardware-ints (IRQs)
 */

#include "typedefs.h"
#include "hal/idt.h"
#include "hal/isr.h"
#include "memory/memfunc.h"
#include "hal/irq.h"
#include "hal/8259.h"
#include "hal/gdt.h"
#include "hal/systrap.h"

#define KBASE (GDT_CODE_RING0<<3)+DPL0

/**
	use 256 entrys to catch all interrupts.
	we will generate the "unhandled interrupt" exception
	ourself
*/

static volatile IDT_ENTRY idt[256];
static volatile IDT_PTR idtp;

/* This exists in 'start.asm', and is used to load our IDT */

static void RegisterISR(UINT8 index, UINT32 base, UINT16 sel, UINT8 flags);

void IDTLoad(volatile IDT_PTR* idtPtr) {
	__asm__ __volatile__ ("lidt (%%eax);" : :"a"(idtPtr));
}


static void RegisterISR(UINT8 index, UINT32 base, UINT16 sel, UINT8 flags) {
	volatile IDT_ENTRY *currIDT = idt + index;

	currIDT->baseLow = (base & 0xFFFF);
	currIDT->baseHigh = (base >> 16) & 0xFFFF;
	currIDT->flags = flags;
	currIDT->sel = sel;
	currIDT->always0 = 0;
}

void OverrideKernelISR(UINT8 exceptionNumber, UINT32 address) {
	RegisterISR(exceptionNumber, address, KBASE, I_PRESENT | I_RING0 | I_INT386);
}

/* Installs the IDT */
void KernelIDTRegister() {


	/* Sets the special IDT pointer up */
	idtp.limit = (sizeof (IDT_ENTRY) * 256) - 1;
	idtp.base = (UINT32)&idt;

	/* Clear out the entire IDT, initializing it to zeros */
	memset((POINTER)&idt, 0, sizeof(IDT_ENTRY) * 256);

	/* CPU exceptions */
	RegisterISR(0, (UINT32)ISR_exc_DivZero, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(1, (UINT32)ISR_exc_Debug, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(2, (UINT32)ISR_exc_NonMaskableInterrupt, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(3, (UINT32)ISR_exc_Breakpoint, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(4, (UINT32)ISR_exc_Overflow, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(5, (UINT32)ISR_exc_OutOfBounds, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(6, (UINT32)ISR_exc_InvalidOpCode, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(7, (UINT32)ISR_exc_No387, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(8, (UINT32)ISR_exc_DoubleFault, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(9, (UINT32)ISR_exc_387SegOverrun, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(10, (UINT32)ISR_exc_BadTSS, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(11, (UINT32)ISR_exc_SegmentNotPresent, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(12, (UINT32)ISR_exc_StackFault, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(13, (UINT32)ISR_exc_GeneralProtectionFault, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(14, (UINT32)ISR_exc_PageFault, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(15, (UINT32)ISR_exc_UnknownInt, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(16, (UINT32)ISR_exc_387Fault, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(17, (UINT32)ISR_exc_Alignment, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(18, (UINT32)ISR_exc_Machine, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(19, (UINT32)ISR_exc_RESERVED19, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(20, (UINT32)ISR_exc_RESERVED20, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(21, (UINT32)ISR_exc_RESERVED21, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(22, (UINT32)ISR_exc_RESERVED22, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(23, (UINT32)ISR_exc_RESERVED23, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(24, (UINT32)ISR_exc_RESERVED24, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(25, (UINT32)ISR_exc_RESERVED25, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(26, (UINT32)ISR_exc_RESERVED26, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(27, (UINT32)ISR_exc_RESERVED27, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(28, (UINT32)ISR_exc_RESERVED28, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(29, (UINT32)ISR_exc_RESERVED29, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(30, (UINT32)ISR_exc_RESERVED30, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(31, (UINT32)ISR_exc_RESERVED31, KBASE, I_PRESENT | I_RING0 | I_INT386);

	/* hardware IRQs */
	RegisterISR(32, (UINT32)IRQ_0, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(33, (UINT32)IRQ_1, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(34, (UINT32)IRQ_2, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(35, (UINT32)IRQ_3, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(36, (UINT32)IRQ_4, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(37, (UINT32)IRQ_5, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(38, (UINT32)IRQ_6, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(39, (UINT32)IRQ_7, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(40, (UINT32)IRQ_8, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(41, (UINT32)IRQ_9, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(42, (UINT32)IRQ_10, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(43, (UINT32)IRQ_11, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(44, (UINT32)IRQ_12, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(45, (UINT32)IRQ_13, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(46, (UINT32)IRQ_14, KBASE, I_PRESENT | I_RING0 | I_INT386);
	RegisterISR(47, (UINT32)IRQ_15, KBASE, I_PRESENT | I_RING0 | I_INT386);

	RegisterISR(KERNEL_SYSTEM_TRAP, (UINT32)SysTrap, KBASE, I_PRESENT | I_RING3 | I_INT386);

	IDTLoad(&idtp);

	/* set up the PIC */
	/* IRQs 0-7 will be fired starting at 32, 8-15 at 40 */

	C8259Init(32, 40);
	C8259_1_Master();
	C8259_2_Slave();
	C8259_1_Mode8086();
	C8259_2_Mode8086();

	// disable all IRQs at the beginning
	C8259SetIRQmaskLO(0xFF);
	C8259SetIRQmaskHI(0xFF);
	
	// but enable timer IRQ (0) and PIC slave (2)
	C8259UnmaskIRQ(0);
	C8259UnmaskIRQ(2);


}
