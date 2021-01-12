/**
 * @file tss.c
 * @author Stefan Nuernberger
 * @date Fr Sep 15 2006
 * @brief Functions to set up and maintain the Task State Segment
 */

#include "typedefs.h"
#include "hal/tss.h"
#include "memory/memory.h"
#include "memory/mmu.h"
#include "memory/addresses.h"
#include "exceptions.h"
#include "hal/gdt.h"
#include "mtask/task_types.h"
#include "memory/usermem.h"
#include "app/ioperm.h"

extern PROCESS_PTR kernelThread;

TSS *CreateTSS(void) {

	// reserve 3 pages for TSS + IOBM
	// don't mark it as used as it is outside the kernel space and therefor not inside the kernel's memory-bitmap
	AllocToVirt(kernelThread->userMemory, STATIC_TSS_VADDR, 3, PAGE_SUPERV | PAGE_WRITE);

	TSS* currTSS = (TSS*)STATIC_TSS_VADDR;
	// clear TSS & IOBM
	memset(currTSS, 0, 3 * PAGE_SIZE);

	currTSS->esp_ring0 = (UINT32)(&kernelThread->regs + 1);
	currTSS->cr3 = (UINT32)kernelThread->userMemory->pageDirPhysAddress;
	currTSS->ss_ring0 = KERNEL_SEG_DATA;
	currTSS->iobase = PAGE_SIZE;

	currTSS->cs = USER_SEG_CODE;
	currTSS->ds = USER_SEG_DATA;
	currTSS->es = USER_SEG_DATA;
	currTSS->fs = USER_SEG_DATA;
	currTSS->gs = USER_SEG_DATA;

	return currTSS;
}
