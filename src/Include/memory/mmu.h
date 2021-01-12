/**
 * @file mmu.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 5 2008
 * @see mmu.c
 * @brief Memory contsants etc. needed for paging (MMU)
 */

#ifndef _mmu_H
#define _mmu_H

#include "multiboot.h"

typedef struct _ModuleLoadInfo{
	UINT32 unpagedMemStart;
	UINT32 moduleLength;
	String moduleCmdLine;
	UINT16 cmdLineLength;
	void* virtualMemChannel;
} *ModuleLoadInfo;

typedef struct _BootInfo{
	String commandLine;
	UINT16 cmdLength;
	ModuleLoadInfo modules;
	UINT16 moduleCount;
} *BootInfo;



BootInfo KernelInitMMU(multiboot_info_t* mbi);

UINT32 SystemMemoryFree();
UINT32 SystemMemoryUsed();

UINT32 GetFreePhysChunk(void);
void SetFreePhysChunk(UINT32 address);

/// size of one page on x86
#define PAGE_SIZE			4096
#define NUM_TABLE_ENTRIES	1024

/// bits of one page
#define PAGE_BITS		12
#define PAGE_OFFSET		0xFFF
#define PAGE_NO_OFFSET	0xFFFFF000


/// page attributes
#define PAGE_PRESENT	0x0001
#define PAGE_READ		0x0000
#define PAGE_WRITE		0x0002
#define PAGE_USER		0x0004
#define PAGE_SUPERV		0x0000
#define PAGE_GLOBAL		0x0100
// Page Cache Disable
#define PAGE_PCD		0x0010
// Page Write-Through
#define PAGE_PWT		0x0008


/// bits in cr0
#define PROC_ENABLE_PAGING			0x80000000
#define PROC_USER_WRITE_PROTECT		0x00010000
#define PROC_WRITE_THROUGH			0x20000000

#define CR4_PG_GLOBAL				0x00000080


#endif
