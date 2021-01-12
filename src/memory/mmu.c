/**
 * @file mmu.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 5 2008
 * @see mmu.h
 * @brief Memory Management Unit (MMU) functions providing paging (and support for higher half kernel)
 */

#include <typedefs.h>
#include <memory/mmu.h>
#include <memory/paging.h>
#include <multiboot.h>
#include <mtask/semaphore.h>
#include <mtask/semaphore_types.h>
#include <hal/processor.h>
#include <exceptions.h>
#include <memory/addresses.h>
#include <memory/memory_types.h>
#include <memory/adt/hashmap.h>
#include <memory/strings/strings.h>
#include <memory/memory.h>
#include <memory/memtune.h>
#include <memory/stackmem.h>
#include <video/video_char.h>
#include <math/mathfuncs.h>
#include <debug.h>

extern PROCESS_PTR kernelThread;
extern PROCESS_PTR currTask;

typedef struct _AvailableMemoryRegion{
	UINT32 start;
	UINT32 stop;
	struct _AvailableMemoryRegion *next;
} AvailableMemoryRegion;

/**
 * variables from the linker
 */
UINT32 KERNEL_ADDR_VIRT_CODE_START;
UINT32 KERNEL_ADDR_VIRT_CODE_END;
UINT32 KERNEL_ADDR_VIRT_DATA_START;
UINT32 KERNEL_ADDR_VIRT_DATA_END;
UINT32 KERNEL_ADDR_VIRT_BSS_START;
UINT32 KERNEL_ADDR_VIRT_BSS_END;

extern UINT32 __KERNEL_ADDR_VIRT_CODE_START;
extern UINT32 __KERNEL_ADDR_VIRT_CODE_END;
extern UINT32 __KERNEL_ADDR_VIRT_DATA_START;
extern UINT32 __KERNEL_ADDR_VIRT_DATA_END;
extern UINT32 __KERNEL_ADDR_VIRT_BSS_START;
extern UINT32 __KERNEL_ADDR_VIRT_BSS_END;


extern void UndoGDTTrick();

/* global variables for this file */
static UINT32 *mallocStack;
static UINT32 memFree, memUsed;

UINT32 *PAGE_DIR_VIRT, *PAGE_DIR;

/* global variables for other files */
IPLOCK M_PhysicalAccess;


/* local function */
static void CreateKernelThread();
static void MapPhysBlock(UINT32 *pageDirectory, UINT32 virtAddress, UINT32 physAddress, UINT32 length, UINT32 attributes);
static void EnablePaging(UINT32 *pageDirectory);
static void RudimentalPaging(UINT32 *pageDirectory, UINT32 *pageDirPhys);
static void RudimentalMapPhysToVirt(UINT32 *pageDirectory, UINT32 virtAddress, UINT32 physAddress, UINT32 attributes);
static void InitMemTypes(UINT32 codeStart, UINT32 codeEnd, UINT32 dataStart);

MEMPROFILE UINT32 GetFreePhysChunk(void) {
	mallocStack--;
	memFree -= PAGE_SIZE;
	memUsed += PAGE_SIZE;

	return *mallocStack;
}

MEMPROFILE void SetFreePhysChunk(UINT32 address) {
#ifdef RAYDEBUG
	if (address == 0x00000000) {
		KernelThrowException("SetFreePhysChunk: Pushed 0 to stack!", EXC_MEMORY_ALLOC_ERR, 0, 0, 0);
	}

#endif
	*mallocStack = address;
	memFree += PAGE_SIZE;
	memUsed -= PAGE_SIZE;
	mallocStack++;
}

UINT32 SystemMemoryFree() {
	return memFree;
}

UINT32 SystemMemoryUsed() {
	return memUsed;
}

static void CreateKernelThread() {
	kernelThread = (PROCESS_PTR)StackMemAlloc(sizeof(struct schedNode));
	MEM_MANAGE* userMemory = (MEM_MANAGE*)StackMemAlloc(sizeof(MEM_MANAGE));

	memset(kernelThread, 0, sizeof(struct schedNode));
	memset(userMemory, 0, sizeof(MEM_MANAGE));

	kernelThread->CPUring = RING0;

	kernelThread->userMemory = userMemory;


	HASH *kernelMemHash = (HASH*) StackMemAlloc(sizeof(HASH));
	kernelThread->userMemory->memAllocs = kernelMemHash;

	HashCreate(kernelMemHash, KERNEL_MAX_ALLOCS, ALLOCATION_INFO);

	UINT32 hashTableSize = HashTableSize(kernelMemHash);
	kernelMemHash->hashTabPtr = (HASH_ENTRY*)StackMemAlloc(hashTableSize);
	memset(kernelMemHash->hashTabPtr, 0, hashTableSize);

	kernelThread->state = T_RUNNING;
	kernelThread->next = NULL;
	kernelThread->priority = PRIO_IDLE;
	kernelThread->currPrio = 0;
	kernelThread->privLevel = PRIV_PRIORITY; // Rights, to change priority. Nothing more.
	kernelThread->isParent = TRUE;
	kernelThread->threadParent = kernelThread;
	kernelThread->pid = 1;

	kernelThread->TIB = StackMemAlloc(sizeof(ThreadInformationBlock));
	kernelThread->TIB->pid = kernelThread->pid;
	kernelThread->TIB->tid = kernelThread->tid;

	strcpy(kernelThread->ident,"Ray Kernel");

	// current task is the kernel
	currTask = kernelThread;
}

static void RudimentalMapPhysToVirt(UINT32 *pageDirectory, UINT32 virtAddress, UINT32 physAddress, UINT32 attributes) {
	UINT32 dirIndex = virtAddress>>22;
	UINT32 tabIndex = (virtAddress>>12) % NUM_TABLE_ENTRIES;

	UINT32 tabPhysAddress;
	UINT32 *newPageTableVirt;

	if (pageDirectory[dirIndex] & PAGE_PRESENT) {
		tabPhysAddress = (pageDirectory[dirIndex] & 0xfffff000);
		// now we have the physical address, but we don't have access to the phys.
		// memory at this address, so we need a virtual address to get access to it
		newPageTableVirt = (UINT32*)(tabPhysAddress + VIRTUAL_KERNEL_SPACE_START);


	} else {

		// Allocate the needed space
		StackMemAlign(PAGE_SIZE);
		newPageTableVirt = StackMemAlloc(PAGE_SIZE);
		tabPhysAddress = (UINT32)newPageTableVirt - VIRTUAL_KERNEL_SPACE_START;
		pageDirectory[dirIndex] = tabPhysAddress | PAGE_PRESENT | PAGE_WRITE | PAGE_SUPERV;
		//printf ("virtAddress = %x, dirIndex = %u, tabIndex = %u\n", virtAddress, dirIndex, tabIndex);
		//KernelThrowException("RudimentalMapPhysToVirt: Requested page table not present", EXC_OUT_OF_MEMORY, 0, 0, 0);
	}


	// write to virtual address and therefore to physical page table
	newPageTableVirt[tabIndex] = (physAddress & 0xfffff000) | PAGE_PRESENT | attributes;
}

static void MapPhysBlock(UINT32 *pageDirectory, UINT32 virtAddress, UINT32 physAddress, UINT32 length, UINT32 attributes) {
	UINT32 pages = NEEDEDPAGES(length);
	while(pages--) {
		RudimentalMapPhysToVirt(pageDirectory, virtAddress, physAddress, attributes);
		virtAddress += PAGE_SIZE;
		physAddress += PAGE_SIZE;
	}
}

static void UnMapPhysBlock(MEM_MANAGE *memManage, UINT32 virtAddress, UINT32 length) {
	UINT32 pages = NEEDEDPAGES(length);
	while(pages--) {
		UnMapVirt(memManage, virtAddress, TRUE);
		virtAddress += PAGE_SIZE;
	}
}



static void EnablePaging(UINT32 *pageDirectory) {
	// write the page directory to CR3-registers
	ProcessorWriteCR3((UINT32)pageDirectory | PAGE_PRESENT);

	// enable paging and the usermode write proection (otherwise user mode read-only pages would
	// be writable from the kernel space)
#ifdef RAY_KERNEL_DEBUGGER
	// unfortunately the kernel debugger needs write-access again
	ProcessorWriteCR0(ProcessorReadCR0() | PROC_ENABLE_PAGING);
#else
	ProcessorWriteCR0(ProcessorReadCR0() | PROC_ENABLE_PAGING | PROC_USER_WRITE_PROTECT);
#endif

	// enable PGE (page global enable) to be able the use of the global bit to partially avoid TLB flushes
	ProcessorWriteCR4(ProcessorReadCR4() | CR4_PG_GLOBAL);
}


static void InitMemTypes(UINT32 codeStart, UINT32 codeEnd, UINT32 dataStart) {
	// set up memory bitmap
	UINT32 codePages = NEEDEDPAGES(codeEnd - codeStart);
	UINT32 dataPages = NEEDEDPAGES(VIRTUAL_KERNEL_SPACE_END - dataStart);

	UINT32 kernelCodeBitmapSize = BITMAPBYTES(codePages);
	UINT32 kernelDataBitmapSize = BITMAPBYTES(dataPages);

	printf("CODE = [%x - %x] (%d pages)\n", codeStart, codeEnd, codePages);
	printf("DATA = [%x - %x] (%d pages)\n", dataStart, VIRTUAL_KERNEL_SPACE_END, codePages);

	kernelThread->userMemory->bitmaps.space[MEMTYPE_CODE].bitmapSpace = (UINT32*)StackMemAlloc(kernelCodeBitmapSize);
	kernelThread->userMemory->bitmaps.space[MEMTYPE_DATA].bitmapSpace = (UINT32*)StackMemAlloc(kernelDataBitmapSize);

	// initialize them
	InitBitmap(&kernelThread->userMemory->bitmaps.space[MEMTYPE_CODE], codePages);
	InitBitmap(&kernelThread->userMemory->bitmaps.space[MEMTYPE_DATA], dataPages);

	// and set offsets for the different segments
	kernelThread->userMemory->bitmaps.start[MEMTYPE_CODE] = codeStart;
	kernelThread->userMemory->bitmaps.stop[MEMTYPE_CODE] = codeEnd;
	kernelThread->userMemory->bitmaps.start[MEMTYPE_DATA] = dataStart;
	kernelThread->userMemory->bitmaps.stop[MEMTYPE_DATA] = VIRTUAL_KERNEL_SPACE_END;

	// and create space for the page table bitmap (32 ints = 1024 bits -> for 1024 page tables)
	kernelThread->userMemory->usedPageTables = StackMemAlloc(1024 / 8);
	memset(kernelThread->userMemory->usedPageTables, 0, 1024 / 8);

}

static void RudimentalPaging(UINT32 *pageDirectory, UINT32 *pageDirPhys) {

	UINT32 KERNEL_ADDR_PHYS_CODE_START = KERNEL_ADDR_VIRT_CODE_START - VIRTUAL_KERNEL_SPACE_START;
	UINT32 KERNEL_ADDR_PHYS_DATA_START = KERNEL_ADDR_VIRT_DATA_START - VIRTUAL_KERNEL_SPACE_START;
	UINT32 KERNEL_ADDR_PHYS_BSS_START =  KERNEL_ADDR_VIRT_BSS_START  - VIRTUAL_KERNEL_SPACE_START;

	InitMemTypes(KERNEL_ADDR_VIRT_CODE_START, KERNEL_ADDR_VIRT_CODE_END, KERNEL_ADDR_VIRT_DATA_START);

	// pre-initialize all page tables
	// this is necessary because user-mode programs simply copy the pointers from the page-dir of the kernel

	UINT32 vKernelStart = VIRTUAL_KERNEL_SPACE_START / PAGE_SIZE / NUM_TABLE_ENTRIES;
	// calculate page directories (rounded!)
	UINT32 kernelUsage = VIRTUAL_KERNEL_SPACE_END - VIRTUAL_KERNEL_SPACE_START;
	UINT32 NumKernelPageTable = (NEEDEDPAGES(kernelUsage) + NUM_TABLE_ENTRIES -1 ) / NUM_TABLE_ENTRIES;

	StackMemAlign(PAGE_SIZE);
	UINT32* pageTables = StackMemAlloc((NumKernelPageTable + 1) * PAGE_SIZE);
	memset(pageTables, 0x00, (NumKernelPageTable + 1) * PAGE_SIZE);
	UINT32 i;
	// convert to physical address
	pageTables = (UINT32*)((UINT32)pageTables - VIRTUAL_KERNEL_SPACE_START);
	printf ("Kernel page directory entries for %u kB (%u pages) =  %u entries [%u to %u]\n", kernelUsage >> 10, NEEDEDPAGES(kernelUsage), NumKernelPageTable, vKernelStart, vKernelStart + NumKernelPageTable);
	for (i = 0; i < NumKernelPageTable; i++) {
		ASSERT(pageDirectory[vKernelStart + i] == 0);
		pageDirectory[vKernelStart + i] = (UINT32)(pageTables + (i * NUM_TABLE_ENTRIES)) | PAGE_PRESENT | PAGE_WRITE | PAGE_SUPERV;
	}


	UINT32 tssPageDirIndex = STATIC_TSS_VADDR / PAGE_SIZE / NUM_TABLE_ENTRIES;
	pageDirectory[tssPageDirIndex] = (UINT32)(pageTables + NumKernelPageTable * NUM_TABLE_ENTRIES) | PAGE_PRESENT | PAGE_WRITE | PAGE_SUPERV;


	// CPU instructions are read-only
#ifdef RAY_KERNEL_DEBUGGER
	// unfortunately CPU instructions must be writable in order to work correctly
	MapPhysBlock(pageDirectory, KERNEL_ADDR_VIRT_CODE_START, KERNEL_ADDR_PHYS_CODE_START, KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START, PAGE_SUPERV | PAGE_WRITE | PAGE_GLOBAL);
#else
	MapPhysBlock(pageDirectory, KERNEL_ADDR_VIRT_CODE_START, KERNEL_ADDR_PHYS_CODE_START, KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START, PAGE_SUPERV | PAGE_READ | PAGE_GLOBAL);
#endif
	// variables can be written, of course
	MapPhysBlock(pageDirectory, KERNEL_ADDR_VIRT_DATA_START, KERNEL_ADDR_PHYS_DATA_START, KERNEL_ADDR_VIRT_DATA_END - KERNEL_ADDR_VIRT_DATA_START, PAGE_SUPERV | PAGE_WRITE | PAGE_GLOBAL);
	MapPhysBlock(pageDirectory, KERNEL_ADDR_VIRT_BSS_START,  KERNEL_ADDR_PHYS_BSS_START,  KERNEL_ADDR_VIRT_BSS_END  - KERNEL_ADDR_VIRT_BSS_START,  PAGE_SUPERV | PAGE_WRITE | PAGE_GLOBAL);


	// mark all the created mappings as used
	printf("Marking KERNEL_ADDR_VIRT_CODE_START(%x [%d]) as used\n", KERNEL_ADDR_VIRT_CODE_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START));
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, KERNEL_ADDR_VIRT_CODE_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START), MEMTYPE_CODE);
	printf("Marking KERNEL_ADDR_VIRT_DATA_START(%x [%d]) as used\n", KERNEL_ADDR_VIRT_DATA_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_DATA_END - KERNEL_ADDR_VIRT_DATA_START));
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, KERNEL_ADDR_VIRT_DATA_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_DATA_END - KERNEL_ADDR_VIRT_DATA_START), MEMTYPE_DATA);
	printf("Marking KERNEL_ADDR_VIRT_BSS_START(%x [%d]) as used\n",KERNEL_ADDR_VIRT_BSS_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_BSS_END  - KERNEL_ADDR_VIRT_BSS_START));
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, KERNEL_ADDR_VIRT_BSS_START, NEEDEDPAGES(KERNEL_ADDR_VIRT_BSS_END  - KERNEL_ADDR_VIRT_BSS_START),  MEMTYPE_DATA);


	// also map the code block (and the .bss-segment for the stack) to itself (identity mapping)
	// as we still use the preliminary trick-GDT
	printf ("Mapping KERNEL_ADDR_PHYS_CODE_START\n");
	MapPhysBlock(pageDirectory, KERNEL_ADDR_PHYS_CODE_START, KERNEL_ADDR_PHYS_CODE_START, KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START, PAGE_SUPERV | PAGE_READ);
	printf ("Mapping KERNEL_ADDR_PHYS_DATA_START\n");
	MapPhysBlock(pageDirectory, KERNEL_ADDR_PHYS_DATA_START, KERNEL_ADDR_PHYS_DATA_START, KERNEL_ADDR_VIRT_DATA_END - KERNEL_ADDR_VIRT_DATA_START, PAGE_SUPERV | PAGE_WRITE);
	printf ("Mapping KERNEL_ADDR_PHYS_BSS_START\n");
	MapPhysBlock(pageDirectory, KERNEL_ADDR_PHYS_BSS_START,  KERNEL_ADDR_PHYS_BSS_START,  KERNEL_ADDR_VIRT_BSS_END  - KERNEL_ADDR_VIRT_BSS_START,  PAGE_SUPERV | PAGE_WRITE);

	// create the alter-page-pointer:
	// A page table entry, that points to the physical page of THAT page table.

	// Map dummy page (0), then get physical address of the according page directory, then map it to that address.
	RudimentalMapPhysToVirt(pageDirectory, STATIC_ALTER_PAGE_POINTER, 0, PAGE_SUPERV | PAGE_WRITE);
	UINT32 alterPagePhys = pageDirectory[STATIC_ALTER_PAGE_POINTER >> 22] & PAGE_NO_OFFSET;
	RudimentalMapPhysToVirt(pageDirectory, STATIC_ALTER_PAGE_POINTER, alterPagePhys, PAGE_SUPERV | PAGE_WRITE);


	// mark the pointers as used as well
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, STATIC_ALTER_PAGE_POINTER, 1, MEMTYPE_DATA);
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, STATIC_ALTER_PAGE_TABLE, 1, MEMTYPE_DATA);

	EnablePaging(pageDirPhys);

	// we are still at virtual address 0x00100000 (just because of the trick-GDT wich still has an offset of 0xC0000000)
	// so install the real GDT with offset 0
	UndoGDTTrick();

	// we are also at 0xc0000000 now -thanks to UndoGDTTrick -
	// this stuff is no longer needed

	UnMapPhysBlock(kernelThread->userMemory, KERNEL_ADDR_PHYS_CODE_START, KERNEL_ADDR_VIRT_CODE_END - KERNEL_ADDR_VIRT_CODE_START);
	UnMapPhysBlock(kernelThread->userMemory, KERNEL_ADDR_PHYS_DATA_START, KERNEL_ADDR_VIRT_DATA_END - KERNEL_ADDR_VIRT_DATA_START);
	UnMapPhysBlock(kernelThread->userMemory, KERNEL_ADDR_PHYS_BSS_START,  KERNEL_ADDR_VIRT_BSS_END  - KERNEL_ADDR_VIRT_BSS_START);

}

/**
 * returns a pointer to a virtual page that is mapped to the particual physical address
 * @param physAddr the address you want to write to
 * @return virtual address to a piece of physical memory
 */
MEMPROFILE UINT32 *OpenPhysMemory(UINT32 *physAddr, BOOL write) {
	*((UINT32*)DIRECT_TABLE_POINTER_POINTER) = ((UINT32)physAddr & PAGE_NO_OFFSET) | PAGE_PRESENT | PAGE_SUPERV | (write ? PAGE_WRITE: 0);
	UpdateTLB((UINT32*)STATIC_ALTER_PAGE_TABLE);
	return (UINT32*)(STATIC_ALTER_PAGE_TABLE + ((UINT32)physAddr & PAGE_OFFSET));
}

UNUSED static void EliminateIntersections(UNUSED AvailableMemoryRegion* regions) {

}

/**
 * Sorts the regions by their start address
 * @param regions
 */
UNUSED static void SortRegions(AvailableMemoryRegion* regions) {


	// For linked lists, a simple bubble sort is the easiest.
	// For such small amounts of data, it's a viable decision

	BOOL sorted;
	do {
		sorted = TRUE;
		AvailableMemoryRegion* curr = regions;
		while(curr) {
			if (curr->next) {
				AvailableMemoryRegion* next = curr->next;
				// in order?
				sorted = (next->start >= curr->start);
				if (!sorted) {
					// swap
					UINT32 nextStart = next->start;
					UINT32 nextStop = next->stop;
					next->start = curr->start;
					next->stop = curr->stop;
					curr->start = nextStart;
					curr->stop = nextStop;

					break;
				}
			}

			curr = curr->next;
		}
	} while(!sorted);

}

/**
 * Takes a list of memory regions as an argument and cuts out the specified range
 * @param regions
 * @param addrStat
 * @param addrLength
 */
static void CutOutRegion(AvailableMemoryRegion* regions, UINT32 addrStart, UINT32 addrEnd) {
	AvailableMemoryRegion* curr = regions;

	while(curr) {
		// if it is inside, cut away a bit
		if ((addrStart <= curr->stop) && (addrEnd >= curr->start)) {
			// check, if it's completely inside
			if ((addrEnd < curr->stop) && (addrStart > curr->start)) {
				AvailableMemoryRegion* middle = StackMemAlloc(sizeof(AvailableMemoryRegion));
				// insert in between
				middle->next = curr->next;
				curr->next = middle;

				middle->stop = curr->stop;
				middle->start = addrEnd + 1;
				curr->stop = addrStart - 1;
			} else {
				// otherwise it overlaps - so just adjust the border accordingly
				if (addrEnd < curr->stop) {
					curr->start = addrEnd + 1;
				}
				if (addrStart > curr->start) {
					curr->stop = addrStart - 1;
				}

				if ((addrStart <= curr->start) && (addrEnd >= curr->stop))
				{
					// the current one is completely covered - remove it
					// NOTE: We cannot simply skip the pointer, as it could
					// be the first one, that is to be removed. But other
					// code holds pointers to the first one
					curr->start = 0;
					curr->stop = 0;
				}
			}
		}

		curr = curr->next;
	}
}

/**
 * Creates a list of memory regions that are available
 * (by gathering the overall available regions and cutting
 * out the reserved ones)
 * @param mbi
 * @return List of memory regions (not necessarily ordered
 * in any way)
 */
static AvailableMemoryRegion* GetAvailMem(multiboot_info_t *mbi) {
	AvailableMemoryRegion* regions = NULL;

	UINT32 mmapAddr = (mbi->mmap_addr + VIRTUAL_KERNEL_SPACE_START);
	multiboot_memory_map_t *mmap;

	// Go through the list and fill the available memory blocks
	mmap = (multiboot_memory_map_t *) mmapAddr;

	while((UINT32) mmap < mmapAddr + mbi->mmap_length) {
		UINT64 addrStart = mmap->addr;
		UINT64 addrLength = mmap->len;
		UINT64 addrStop = addrStart + addrLength - 1L;
		// avoid memory remapping in 32 bit (NON-PAE!) mode
		if (addrStart < 0xFFFFFFFFL) {
			if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
				AvailableMemoryRegion* newRegion = StackMemAlloc(sizeof(AvailableMemoryRegion));
				newRegion->start = (UINT32)(addrStart & 0xFFFFFFFF);
				newRegion->stop = (UINT32)(addrStop & 0xFFFFFFFF);
				newRegion->next = regions;

				regions = newRegion;


			}
		}
		printf ("BIOS MAP: %x%x - %x%x %s\n",
				(UINT32)(addrStart  >> 32), (UINT32)(addrStart  & 0xFFFFFFFF),
				(UINT32)((addrStart + addrLength) >> 32), (UINT32)((addrStart + addrLength) & 0xFFFFFFFF),
				((mmap->type == MULTIBOOT_MEMORY_AVAILABLE) ? "    usable" :"not usable"));
		mmap = (multiboot_memory_map_t *) ((UINT32) mmap + mmap->size + sizeof (mmap->size));
	}

	return regions;
}

static void CutOutWholePages(AvailableMemoryRegion* regions, UINT32 addrStart, UINT32 addrEnd) {

	if (addrEnd <= addrStart)
	{
		KernelThrowException("CutOutWholePages: End < Start", EXC_OUT_OF_MEMORY, addrStart, 0, NULL);
	}
	addrStart = ROUNDPAGEDOWN(addrStart);
	addrEnd = ROUNDPAGEUP(addrEnd) - 1;

	CutOutRegion(regions, addrStart, addrEnd);
}

static UINT32 GetPagesInRegion(AvailableMemoryRegion* regions) {
	AvailableMemoryRegion* curr = regions;

	UINT32 pages = 0;
	while(curr) {
		UINT32 addrStart = ROUNDPAGEDOWN(curr->start);
		UINT32 addrEnd = ROUNDPAGEUP(curr->stop);

		pages += (addrEnd - addrStart) / PAGE_SIZE;

		printf ("From %x to %x (%u pages)\n", addrStart, addrEnd, pages);

		curr =  curr->next;
	}

	return pages;
}

static void CutOutUsedData(AvailableMemoryRegion* regions, multiboot_info_t *mbi) {
	CutOutWholePages(regions, (UINT32)mbi, (UINT32)mbi + sizeof(multiboot_info_t));

	String cmdLine = (String)(mbi->cmdline + VIRTUAL_KERNEL_SPACE_START);
	CutOutWholePages(regions, mbi->cmdline, mbi->cmdline + (strlen(cmdLine) + 1));

	// mmap
	CutOutWholePages(regions, mbi->mmap_addr, mbi->mmap_addr + mbi->mmap_length);

	// modules
	UINT32 i;
	multiboot_module_t* module = (multiboot_module_t*)(mbi->mods_addr + VIRTUAL_KERNEL_SPACE_START);
	for(i = 0; i < mbi->mods_count; i++) {
		CutOutWholePages(regions, module->mod_start, module->mod_end);
		String modCmd = (String)(module->cmdline + VIRTUAL_KERNEL_SPACE_START);
		CutOutWholePages(regions, module->cmdline, module->cmdline + strlen(modCmd) + 1);
	}

	// kernel itself (physical address)
	CutOutWholePages(regions, KERNEL_ADDR_VIRT_CODE_START - VIRTUAL_KERNEL_SPACE_START, KERNEL_ADDR_VIRT_CODE_END - VIRTUAL_KERNEL_SPACE_START);
	CutOutWholePages(regions, KERNEL_ADDR_VIRT_DATA_START - VIRTUAL_KERNEL_SPACE_START, KERNEL_ADDR_VIRT_DATA_END - VIRTUAL_KERNEL_SPACE_START);
	CutOutWholePages(regions, KERNEL_ADDR_VIRT_BSS_START  - VIRTUAL_KERNEL_SPACE_START, KERNEL_ADDR_VIRT_BSS_END  - VIRTUAL_KERNEL_SPACE_START);

	// and the complete region below 1MB
	CutOutWholePages(regions, 0x00000000, 0x00100000);

}

static BootInfo CreateBootInfo(multiboot_info_t *mbi) {
	String cmdLine = (String)(mbi->cmdline + VIRTUAL_KERNEL_SPACE_START);
	UINT32 cmdLineLength = strlen(cmdLine) + 1;

	// Create main structure
	BootInfo info = StackMemAlloc(sizeof(struct _BootInfo));
	info->commandLine = StackMemAlloc(cmdLineLength);
	info->cmdLength = cmdLineLength;
	memcpy(info->commandLine, cmdLine, cmdLineLength);
	info->moduleCount = mbi->mods_count;

	// and append modules

	info->modules = StackMemAlloc(sizeof(struct _ModuleLoadInfo) * mbi->mods_count);
	ModuleLoadInfo mInfo = info->modules;
	multiboot_module_t* module = (multiboot_module_t*)(mbi->mods_addr + VIRTUAL_KERNEL_SPACE_START);
	UINT32 i;
	for(i = 0; i < mbi->mods_count; i++, module++, mInfo++) {
		String modCmd = (String)(module->cmdline + VIRTUAL_KERNEL_SPACE_START);
		mInfo->cmdLineLength = strlen(modCmd) + 1;

		mInfo->moduleCmdLine = StackMemAlloc(mInfo->cmdLineLength);
		memcpy(mInfo->moduleCmdLine, modCmd, mInfo->cmdLineLength);
		mInfo->unpagedMemStart = module->mod_start;
		mInfo->moduleLength = module->mod_end - module->mod_start;

	}

	return info;

}

/**
 * Initializes the kernel's memory management (paging)
 * @param mbi Multiboot information structure containing
 * memory layout and module load information
 * @return BootInfo structure pointer, containing
 * command line arguments and module pointers rescued from MBI.
 */

BootInfo KernelInitMMU(multiboot_info_t *mbi) {
	UINT32 totalMemory = mbi->mem_upper << 10;
	KERNEL_ADDR_VIRT_CODE_START = (UINT32)&__KERNEL_ADDR_VIRT_CODE_START;
	KERNEL_ADDR_VIRT_CODE_END   = (UINT32)&__KERNEL_ADDR_VIRT_CODE_END;
	KERNEL_ADDR_VIRT_DATA_START = (UINT32)&__KERNEL_ADDR_VIRT_DATA_START;
	KERNEL_ADDR_VIRT_DATA_END   = (UINT32)&__KERNEL_ADDR_VIRT_DATA_END;
	KERNEL_ADDR_VIRT_BSS_START  = (UINT32)&__KERNEL_ADDR_VIRT_BSS_START;
	KERNEL_ADDR_VIRT_BSS_END    = (UINT32)&__KERNEL_ADDR_VIRT_BSS_END;


	// check if there's enough memory
	if (totalMemory < 25000000) {
		KernelThrowException("Not enough memory for memory management", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return NULL;
	}

	BootInfo bootInfo = CreateBootInfo(mbi);

	AvailableMemoryRegion* regions = GetAvailMem(mbi);
	AvailableMemoryRegion* currReg = regions;
	while(currReg) {
		printf ("Memory map: %x - %x\n", currReg->start, currReg->stop);
		currReg = currReg->next;
	}
	// Cut out the regions, that are in use by the bootloader, the loaded kernel and modules
	CutOutUsedData(regions, mbi);
	currReg = regions;
	printf ("\n");
	while(currReg) {
		printf ("Memory map: %x - %x\n", currReg->start, currReg->stop);
		currReg = currReg->next;
	}

	// reserve 3 more pages for TSS & IOBM -> so 1 more page directory entry
	//UINT32 numPageTables = NEEDEDPAGES(VIRTUAL_KERNEL_SPACE_END - VIRTUAL_KERNEL_SPACE_START) / NUM_TABLE_ENTRIES + 1;

	printf ("Stack mem usage: %u\n",  StackMemGetUsage());

	StackMemAlign(PAGE_SIZE);
	PAGE_DIR_VIRT = StackMemAlloc(PAGE_SIZE);
	printf ("Stack mem usage: %u\n",  StackMemGetUsage());


	PAGE_DIR = (UINT32*)((UINT32)PAGE_DIR_VIRT - VIRTUAL_KERNEL_SPACE_START);

	// clear page directory
	memset(PAGE_DIR_VIRT, 0, PAGE_SIZE);

	// create a kernel thread - we'll need one for further progress
	CreateKernelThread();
	printf ("Stack mem usage: %u\n",  StackMemGetUsage());

	// Enable semaphores
	HASH* semaphoreHash = (HASH*)StackMemAlloc(sizeof(HASH));
	HashCreate(semaphoreHash, NUM_IP_SEMAPHORES, SEMAPHORE);
	UINT32 semHashSize = HashTableSize(semaphoreHash);
	semaphoreHash->hashTabPtr = StackMemAlloc(semHashSize);
	memset(semaphoreHash->hashTabPtr, 0, semHashSize);
	SemaphoresInit(semaphoreHash);

	printf ("Stack mem usage: %u\n",  StackMemGetUsage());

	M_PhysicalAccess = MutexCreate("PhysicalAccess", TRUE);

	kernelThread->userMemory->pageDirVirtAddress = PAGE_DIR_VIRT;
	kernelThread->userMemory->pageDirPhysAddress = PAGE_DIR;

	UINT32 availablePages = GetPagesInRegion(regions);
	// Reserve one additional value for 0x00000000, indicating "OUT OF MEMORY"
	UINT32 mallocStackSize = (availablePages + 1) * sizeof(UINT32);
	printf ("Free pages stack size: %u bytes (for %u pages = %u MB RAM)\n", mallocStackSize, availablePages, availablePages >> 8);

	// reserve malloc stack space for physical pages.
	mallocStack = StackMemAlloc(mallocStackSize);

	memFree = availablePages * PAGE_SIZE;
	memUsed = 0;

	printf ("Initialized everything: memFree = %u, memUsed = %x, PAGE_DIR_VIRT=%x, PAGE_DIR=%x\n", memFree, memUsed, (UINT32)PAGE_DIR_VIRT, (UINT32)PAGE_DIR);
	RudimentalPaging(PAGE_DIR_VIRT, PAGE_DIR);
	//
	//  READY TO RUMBLE!
	//

	*mallocStack = 0x00000000;
	mallocStack++;
	// push all the free pages to the stack
	AvailableMemoryRegion* curr = regions;
	while(curr) {
		UINT32 startPage = curr->start & PAGE_NO_OFFSET;

		while((startPage > 0) && (startPage < curr->stop)) {
			// Do not push zero to stack
			// Zero is used as a special value
			// to detect errors (if we pop Zero
			// off the stack, sth. is wrong)
			if (startPage > 0) {
				*mallocStack = startPage;
				mallocStack++;
			}

			startPage += PAGE_SIZE;
			// NOTE:
			// Additionally, when the last page has ben pushed
			// to the stack, startPage += PAGE_SIZE will generate
			// an overflow and startPage will equal 0x00000000.
			// Only true for 4GB systems.
			if (startPage == 0x00000000) {
				break;
			}
		}

		curr = curr->next;
	}

	return bootInfo;

}
