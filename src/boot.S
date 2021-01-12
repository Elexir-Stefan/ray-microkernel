/**
 * @file boot.S
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief Sets up paging (for a higher half kernel) and jumps into the C part
 */

// Use ELF format
#define MULTIBOOT_HEADER_FLAGS         0x00000003


#define ASM_FILE
#include <multiboot.h>
#include <memory/addresses.h>

	.globl  start
	.globl UndoGDTTrick
.section .text.start
.func start
start:

             jmp     multiboot_entry

             /* Align 32 bits boundary. */
             .align  4

             /* Multiboot header. */
     multiboot_header:
             /* magic */
             .long   MULTIBOOT_HEADER_MAGIC
             /* flags */
             .long   MULTIBOOT_HEADER_FLAGS
             /* checksum */
             .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
     #ifndef __ELF__
             /* header_addr */
             .long   multiboot_header
             /* load_addr */
             .long   _start
             /* load_end_addr */
             .long   _edata
             /* bss_end_addr */
             .long   _end
             /* entry_addr */
             .long   multiboot_entry
     #endif /* ! __ELF__ */

     multiboot_entry:

	/* set up the preliminary GDT */
	lgdtl (preliminaryGDT - 0xC0000000)

	/* set up the correct segment selectors */
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	/* and activate the GDT */
	ljmp $0x08, $GDTFlush
GDTFlush:

	/* Initialize the stack pointer. */
	movl    $kernelStack, %esp

	/* Reset EFLAGS. */
	pushl   $2
	popf

	/* Push the pointer to the Multiboot information structure. */
	addl $0xc0000000, %ebx		/* correct offset as GDT offset is now 0x40000000 -> 0xc0000000 + 0x40000000 = 0x0000000 */
	pushl   %ebx

	/* Now enter the RAY kernel... */
	call    KernelInit

	/* pop last argument */
	add $4, %esp

	/* Shouldn't get here... */
	pushl   $halt_message
	call    printf

loop:
	cli
	hlt
	jmp     loop
.endfunc

UndoGDTTrick:
	lgdt resetGDT
	/* set up the correct segment selectors */
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	/* and activate the GDT */
	ljmp $0x08, $GDTFlushUndo
GDTFlushUndo:
	ret


.section .data
halt_message:
	.asciz  "SERIOUS ERROR: Kernel killed!. SYSTEM HALTED"

resetGDT:
	.2byte	resetGDTEnd - resetGDTstruct - 1	/* size of the GDT structure */
	.4byte	resetGDTstruct - 0xC0000000				/* address of the following structure */

resetGDTstruct:
	.4byte	0, 0
	.byte	0xFF, 0xFF, 0, 0, 0, 0x9A, 0xCF, 0x00		/* code selector 0x08: base 0x00000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF */
	.byte	0xFF, 0xFF, 0, 0, 0, 0x92, 0xCF, 0x00		/* data selector 0x10: base 0x00000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF */
resetGDTEnd:

preliminaryGDT:
	.2byte	preliminaryGDTEnd - preliminaryGDTstruct - 1	/* size of the GDT structure */
	.4byte	(preliminaryGDTstruct - 0xC0000000) 				/* address of the following structure */

preliminaryGDTstruct:
	.4byte	0, 0
	.byte	0xFF, 0xFF, 0, 0, 0, 0x9A, 0xCF, 0x40		/* code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF */
	.byte	0xFF, 0xFF, 0, 0, 0, 0x92, 0xCF, 0x40		/* data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF */
preliminaryGDTEnd:


.section .bss
	.globl EMERGENCYSTK
	.globl kernelStack
	.lcomm EmergencyStackStart, KERNEL_STACK_SIZE
	.lcomm EMERGENCYSTK, 4
	.lcomm kernelStackStart, KERNEL_STACK_SIZE
	.lcomm kernelStack, 4

