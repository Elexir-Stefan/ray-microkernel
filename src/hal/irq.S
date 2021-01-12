/******************************************
	FILE:	irq.S
	DATE:	02-25-2006

	RAY Kernel
	(C) NURNware Tech. 2006 [Stefan Nuernberger]

	IRQ Handler for hardware-interrupts

*******************************************/

#define ASM

#define KERNEL_SEG_DATA $0x10
#define KERNEL_SEG_CODE $0x08
#define USER_SEG_DATA $0x23
#define USER_SEG_CODE $0x1B

#include "hal/msr.h"
#include "hal/systrap.h"

#define TSS_ESP_RING0	4

#define MEM_MANAGE_ATTR_PHYS_PAGE	4

#define THREAD_ATTR_CPU_RING		00
#define THREAD_ATTR_USER_STACK		04
#define THREAD_ATTR_REGS		16
#define THREAD_ATTR_MEM_MANAGE		88
#define THREAD_ATTR_REGPOINTER		92

#define INT_R_GS			00
#define INT_R_FS			04
#define INT_R_ES			08
#define INT_R_DS			12
#define INT_R_EDI			16
#define INT_R_ESI			20
#define INT_R_EBP			24
#define INT_R_ESP			28
#define INT_R_EBX			32
#define INT_R_EDX			36
#define INT_R_ECX			40
#define INT_R_EAX			44
#define INT_NUM				48
#define INT_EIP				52
#define INT_CS				56
#define INT_EFLAGS			60
#define INT_ESP				64
#define INT_SS				68
#define INT_ADD_CODE		72

#define INT_EIP_AFTER_REGS		(INT_EIP - 52)
#define INT_ESP_AFTER_REGS		(INT_ESP - 52)
#define INT_EFLAGS_AFTER_REGS		(INT_EFLAGS - 52)

#define INT_EIP_FROM_BOTTOM		(INT_EIP - 64)

/* #define REENTRANT */

.text
.globl  IRQ_0, IRQ_1, IRQ_2, IRQ_3, IRQ_4, IRQ_5, IRQ_6, IRQ_7, IRQ_8, IRQ_9, IRQ_10, IRQ_11, IRQ_12, IRQ_13, IRQ_14, IRQ_15
.globl SysCallEntry, ISR_exc_PageFault, ISR_exc_GeneralProtectionFault, SysTrap
.extern IRQHandler,  C8259InterruptDone, dontInterrupt, currTask, kernelTSS
.extern kernelStack, EMERGENCYSTK

// Debugging
#ifdef RAYPROFILING
.extern PROFILING_ENABLE
#endif

.macro SaveRegsINT SEC
	pushal
	push %ds
	push %es
	push %fs
	push %gs

	/* set up proper kernel segments */
	movw KERNEL_SEG_DATA, %cx
	movw %cx, %ds
	movw %cx, %es
	movw %cx, %fs
	movw %cx, %gs

	/* switch to kernel stack if from RING3 only*/
	movl INT_CS(%esp), %ecx
	cmpl KERNEL_SEG_CODE, %ecx
	je skipKernelStackSetup_$\SEC
	movl $kernelStack, %esp
	jmp saveRegsDone_$\SEC

skipKernelStackSetup_$\SEC:
	movl currTask, %ecx
	movl %esp, THREAD_ATTR_REGPOINTER(%ecx)
saveRegsDone_$\SEC:
.endm

.macro SaveRegsFASTCALL
	pushal
	push %ds
	push %es
	push %fs
	push %gs


	/* set up proper kernel segments */
	movw KERNEL_SEG_DATA, %cx
	movw %cx, %ds
	movw %cx, %es
	movw %cx, %fs
	movw %cx, %gs

	/* syscall does not save SS, ESP, EFLAGS, CS, EIP and does not push an int# ! */

	movl USER_SEG_DATA, 	INT_SS(%esp)
	movl %ebp, 		INT_ESP(%esp)
	movl USER_SEG_CODE, 	INT_CS(%esp)
	movl %edx, 		INT_EIP(%esp)

	movl $kernelStack, %esp

.endm

.macro RestoreRegsCommon
	/* load page directory */
	movl currTask, %ecx
	movl THREAD_ATTR_MEM_MANAGE(%ecx), %edx		/* pointer to MEM_MANAGE */
	movl MEM_MANAGE_ATTR_PHYS_PAGE(%edx), %edx	/* load physical address of page directory of current task in edx */

	mov %cr3, %eax
	cmpl %eax, %edx
	je 1f
	movl %edx, %cr3

1:
	/* restore stack to USERREGstack */
	movl THREAD_ATTR_REGPOINTER(%ecx), %esp

	/* reload registers */
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
	add $4, %esp					/* INT # */

.endm

.macro RestoreRegsINT name

	RestoreRegsCommon

#ifndef REENTRANT
	sti
#endif
	iretl
.endm

.macro RestoreRegsFASTCALL
	movl currTask, %edx

	/* switch to RING 3 or not? */
	cmpb $1, THREAD_ATTR_CPU_RING(%edx)
	jne directJump

	RestoreRegsCommon

	/* write user's ESP */
	movl INT_ESP_AFTER_REGS(%esp), %ecx		/* "ESP is loaded from ECX" (24594-2.pdf "AMD64 Technologies Programmer's Manual Volume 3" ) */

	/* write return address */
	movl INT_EIP_AFTER_REGS(%esp), %edx		/* "EIP is loaded from EDX" (24594-2.pdf "AMD64 Technologies Programmer's Manual Volume 3" ) */

	/* restore EFLAGS */
/*add $INT_EFLAGS_AFTER_REGS, %esp
	popf
	sub $(INT_EFLAGS_AFTER_REGS + 4), %esp
*/

#ifndef REENTRANT
	sti
#endif

	sysexit
directJump:
	RestoreRegsCommon
	add $12, %esp				/* normally iret would pop EIP, CS, EFLAGS */
/*	sti */
	jmp *INT_EIP_FROM_BOTTOM(%esp)
.endm

IRQ_0:
#ifndef REENTRANT
	cli
#endif


	pushl $0
	SaveRegsINT TIMER

	call Schedule

	pushl $0
	call C8259InterruptDone			/* C8259InterruptDone(0) */

	add $4, %esp

	RestoreRegsINT TIMER
.macro ISR name
IRQ_\name:
#ifndef REENTRANT
	cli
#endif

	pushl $\name
	SaveRegsINT IRQ_\name

	call IRQHandler

	RestoreRegsINT IRQ_\name
.endm

ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15

SysTrap:
#ifndef REENTRANT
	cli
#endif

	pushl $KERNEL_SYSTEM_TRAP
	SaveRegsINT SysTrap

	pushl %ebx	// argument pointer
	pushl %eax	// function number
	call SysCall
	add $8, %esp	// remove 2 arguments

	RestoreRegsINT SysTrap

SysCallEntry:
#ifndef REENTRANT
	cli
#endif

/*
TODO: Was ist mit den Werten hinter den 6 Stellen, die nicht durch den Interrupt gefüllt wurden?
Wird der Thread das nächste Mal normal geschedult (IRQ) dann gibt an dieser Stelle keine Rücksprungadresse!
*/
	subl $24, %esp
	SaveRegsFASTCALL

	pushl %ebx	// argument pointer
	pushl %eax	// function number
	call SysCall
	add $8, %esp	// remove 2 arguments

	RestoreRegsFASTCALL

.macro FaultHandler name

.func ISR_exc_\name
ISR_exc_\name:
	cli

	pushal
	push %ds
	push %es
	push %fs
	push %gs

// set up proper kernel segments
	movw KERNEL_SEG_DATA, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	#ifdef RAYPROFILING
	movb $0, ENABLE_PROFILING
	#endif

// Pretend function prologue for stack unwinding
	movl %esp, %ebx				// save old stack in ebx and change to kernel stack
	movl $EMERGENCYSTK, %esp

	movl INT_EIP(%ebx), %eax	// EAX now holds old EIP
	movl INT_R_EBP(%ebx), %ecx	// ECX now holds old EBP

	pushl %eax		// pretends CALL (EIP)

	// function prologue
	pushl %ecx
	mov %esp, %ebp


	// arguments for KernelThrowException
	pushl %ebx	// push "ESP" (saved in ebx)
	pushl $1
	pushl INT_ADD_CODE(%ebx)
	pushl $14
	pushl $strExc\name
	call KernelThrowException

	add $20, %esp	// remove KernelThrowException's arguments
	add $8, %esp	// remove pretended stack frame

	RestoreRegsINT name
.endfunc
.endm

FaultHandler PageFault
FaultHandler GeneralProtectionFault


strExcPageFault:
	.asciz "Page fault"
strExcGeneralProtectionFault:
	.asciz "General protection fault"
