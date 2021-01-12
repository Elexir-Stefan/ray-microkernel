/**
 * @file paging.c
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief A lot of functions that make paging possible
 */

#include <flags.h>
#include <typedefs.h>
#include <memory/adt/hashmap.h>
#include <memory/paging.h>
#include <memory/memory.h>
#include <memory/addresses.h>
#include <memory/adt/bitmap.h>
#include <hal/tss.h>
#include <mtask/semaphore.h>
#include <mtask/task_types.h>
#include <mtask/scheduler.h>
#include <exceptions.h>
#include <hal/gdt.h>
#include <memory/mmu.h>
#include <hal/processor.h>
#include <debug.h>

#include "video/video_char.h"
#include "video/video_mode.h"

#include "hal/isr.h"

extern PROCESS_PTR kernelThread;
extern UINT32 ADDR_KERNEL_LOAD, ADDR_DATA, ADDR_BSS, ADDR_KERNEL_END;

extern UINT32 *PAGE_DIR_VIRT, *PAGE_DIR;
extern UINT32 *PAGE_TAB_VIRT, *PAGE_TAB;

/* global variables for other files */
extern IPLOCK M_PhysicalAccess;


MEMPROFILE UINT32 GetFreeSpaceStart(SEGMENT_BITMAPS *bitmaps, UINT32 pageCount, MEMORY_KIND kind) {
	UINT32 firstFreeAddress = FindFreeSpaceInBitmap(&bitmaps->space[kind], pageCount);

	if (firstFreeAddress != NO_FITTING_SPACE) {
		return firstFreeAddress * PAGE_SIZE + bitmaps->start[kind];
	} else {
		return 0;
	}
}

MEMPROFILE BOOL MarkAbsoluteAddressAsUsed(SEGMENT_BITMAPS *bitmaps, UINT32 from, UINT32 pageCount, MEMORY_KIND kind) {
	return MarkUsedSpaceInBitmap(&bitmaps->space[kind], (from - bitmaps->start[kind]) / PAGE_SIZE, pageCount);
}

MEMPROFILE BOOL MarkAbsoluteAddressAsFree(SEGMENT_BITMAPS *bitmaps, UINT32 from, UINT32 pageCount, MEMORY_KIND kind) {
	return MarkFreeSpaceInBitmap(&bitmaps->space[kind], (from - bitmaps->start[kind]) / PAGE_SIZE, pageCount);
}

MEMPROFILE UINT32 AllocToVirt(MEM_MANAGE *userMemory, UINT32 virtAddress, UINT32 pageCount, UINT32 attributes) {
	UINT32 newPhys = 0;

	ASSERT((virtAddress & PAGE_OFFSET) == 0);

	while (pageCount--) {
		newPhys = GetFreePhysChunk();
		if (!newPhys) {
			KernelThrowException("Out of physical memory!", EXC_OUT_OF_MEMORY, virtAddress, 0, NULL);
			ASSERT(FALSE);
			return 0;
		}
		MapPhysToVirt(userMemory, virtAddress, newPhys, attributes);
		virtAddress += PAGE_SIZE;
	}

	return newPhys;
}


MEMPROFILE void ReleaseVirtualMemChannel(MEM_MANAGE *memManage, UINT32 virtAddress, UINT32 pageCount, BOOL setFree, BOOL instantanious) {
	UINT32 freePhysMem;
	ASSERT((virtAddress & PAGE_OFFSET) == 0);
	while (pageCount--) {
		freePhysMem = UnMapVirt(memManage, virtAddress, instantanious);
		if (UNLIKELY(freePhysMem == 0)) {
			KernelThrowException("Tried to unmap unmapped memory!", EXC_OUT_OF_MEMORY,0,0,0);
		}
		if (setFree) {
			SetFreePhysChunk(freePhysMem);
		}
		virtAddress += PAGE_SIZE;
	}
}

MEMPROFILE BOOL MoveThreadMemory(UINT32 fromAddress, UINT32 toAddress, MEM_MANAGE* fromThread, MEM_MANAGE* toThread, UINT32 pageCount, UINT32 allocPagesSkip, UINT32 allocPagesPadd, MEMORY_KIND fromKind, MEMORY_KIND toKind, UINT32 newAttributes) {
	UINT32 physAddress = 0;

	ASSERT((fromAddress & PAGE_OFFSET) == 0);
	ASSERT((toAddress & PAGE_OFFSET) == 0);

	// change owner of every page
	MarkAbsoluteAddressAsFree(&fromThread->bitmaps, fromAddress, pageCount, fromKind);
	MarkAbsoluteAddressAsUsed(&toThread->bitmaps,     toAddress, pageCount, toKind);

	toAddress += PAGE_SIZE * allocPagesSkip;
	fromAddress += PAGE_SIZE * allocPagesSkip;
	pageCount -= allocPagesSkip + allocPagesPadd;
	while (pageCount--) {
		physAddress = UnMapVirt(fromThread, fromAddress, TRUE);
		if (physAddress == 0) {
			ASSERT(FALSE);
			return FALSE;
		}
		MapPhysToVirt(toThread, toAddress, physAddress, newAttributes);
		fromAddress += PAGE_SIZE;
		toAddress += PAGE_SIZE;
	}

	return TRUE;

}


MEMPROFILE UINT32 UnMapVirt(MEM_MANAGE *memManage, UINT32 virtAddress, BOOL instantanious) {
	UINT32 *pageDirectory = memManage->pageDirVirtAddress;
	UINT32 dirIndex = virtAddress>>22;
	UINT32 tabIndex = (virtAddress>>12) % NUM_TABLE_ENTRIES;
	UINT32 *pageTableVirt, *pageTablePhys;
	UINT32 mappedPhysAddress;

	ASSERT((virtAddress & PAGE_OFFSET) == 0);

	if (LIKELY(pageDirectory[dirIndex] & PAGE_PRESENT)) {
		pageTablePhys = (UINT32*)(pageDirectory[dirIndex] & PAGE_NO_OFFSET);

		/* before we can delete the entry in the page table
		we have to gain access through a virtual address */
		SemaphoreEnter(M_PhysicalAccess);
		pageTableVirt = OpenPhysMemory(pageTablePhys, TRUE);
		mappedPhysAddress = (pageTableVirt[tabIndex] & PAGE_NO_OFFSET);
		ASSERT(mappedPhysAddress != 0);
		/* delete entry in the pagetable */
		pageTableVirt[tabIndex] = 0;

		/* update the entry if it is in the TLB */
		if (LIKELY(instantanious)) {
			UpdateTLB((UINT32*)virtAddress);
		}
		SemaphoreLeave(M_PhysicalAccess);
		return mappedPhysAddress;
	} else {
		KernelThrowException("UnMapVirt: PageTable doesn't exist!", EXC_ILLEGAL_STATE, virtAddress, 0, 0);
		return 0;
	}
}

MEMPROFILE UINT32 GetPhysicalAddressOf(UINT32 *pageDirectory, POINTER virtAddress) {
	UINT32 dirIndex = ((UINT32)virtAddress)>>22;
	UINT32 tabIndex = (((UINT32)virtAddress) / PAGE_SIZE) % NUM_TABLE_ENTRIES;
	UINT32 *pageTablePhys, *pageTableVirt;
	UINT32 physicalPage;

	// check for presence of physical address mapped at that address range [x < virtAddress < y, (y - x ) = 4 MB]
	if (pageDirectory[dirIndex] & PAGE_PRESENT) {
		// get physical address of page table
		pageTablePhys = (UINT32*)(pageDirectory[dirIndex] & PAGE_NO_OFFSET);

		// get access to it from within our address space
		SemaphoreEnter(M_PhysicalAccess);
		pageTableVirt = OpenPhysMemory(pageTablePhys, FALSE);
		// check if table (4K) present
		if (pageTableVirt[tabIndex] & PAGE_PRESENT) {
			physicalPage = pageTableVirt[tabIndex];
		} else {
			physicalPage = 0;
		}
		SemaphoreLeave(M_PhysicalAccess);
		return physicalPage;
	} else {
		// not present
		return 0;
	}
}

MEMPROFILE BOOL MemoryChangePageAttribute(UINT32 *pageDirectory, POINTER virtAddress, UINT32 newAttribute, MEM_ALTER alter) {
	UINT32 dirIndex = ((UINT32)virtAddress)>>22;
	UINT32 tabIndex = (((UINT32)virtAddress) / PAGE_SIZE) % NUM_TABLE_ENTRIES;
	UINT32 *pageTablePhys, *pageTableVirt;


	// check for presence of physical address mapped at that address range [x < virtAddress < y, (y - x ) = 4 MB]
	if (pageDirectory[dirIndex] & PAGE_PRESENT) {
		// get physical address of page table
		pageTablePhys = (UINT32*)(pageDirectory[dirIndex] & PAGE_NO_OFFSET);

		// get access to it from within our address space
		SemaphoreEnter(M_PhysicalAccess);
		pageTableVirt = OpenPhysMemory(pageTablePhys, TRUE);
		// check if table (4K) present
		if (pageTableVirt[tabIndex] & PAGE_PRESENT) {
			// change the page according the the wanted modification
			switch (alter) {
			case MEM_ATTR_OVERWRITE:
				// overwrite the current settings with new settings
				// reset old settings
				pageTableVirt[tabIndex] &= PAGE_NO_OFFSET;
				pageTableVirt[tabIndex] |= PAGE_PRESENT | newAttribute;
				break;
			case MEM_ATTR_ENABLE:
				// just set the requested bit
				pageTableVirt[tabIndex] |= newAttribute;
				break;
			case MEM_ATTR_DISABLE:
				// keep all the bits except the requested one
				pageTableVirt[tabIndex] &= ~ newAttribute;
				break;
			}
			// Refresh the TLB
			UpdateTLB(virtAddress);
			SemaphoreLeave(M_PhysicalAccess);
			return TRUE;
		} else {
			SemaphoreLeave(M_PhysicalAccess);
			ASSERT(FALSE);
			return FALSE;
		}

	} else {
		// not present
		return FALSE;
	}
}

MEMPROFILE BOOL MapPhysToVirt(MEM_MANAGE *memManage, UINT32 virtAddress, UINT32 physAddress, UINT32 attributes) {
	UINT32 *pageDirectory = memManage->pageDirVirtAddress;
	UINT32 dirIndex = virtAddress>>22;
	UINT32 tabIndex = (virtAddress>>12) % NUM_TABLE_ENTRIES;

	ASSERT(virtAddress == (virtAddress & PAGE_NO_OFFSET));

	UINT32 tabPhysAddress;
	UINT32 *newPageTableVirt;

	BOOL allocatedPageDir = FALSE;

	if (LIKELY(pageDirectory[dirIndex] & PAGE_PRESENT)) {
		tabPhysAddress = (pageDirectory[dirIndex] & PAGE_NO_OFFSET);
		/* now we have the physical address, but we don't have access to the phys.
		memory at this address, so we need a virtual address to get access to it */
		newPageTableVirt = OpenPhysMemory((UINT32*)tabPhysAddress, TRUE);
		SemaphoreEnter(M_PhysicalAccess);
	} else {
		// according page table not present in directory
		if (pageDirectory == PAGE_DIR_VIRT) {
			// kernel mode
			KernelThrowException("MapPhysToVirt: Unmapped kernel detected!", EXC_MEMORY_ALLOC_ERR, dirIndex, 0, 0);
			return FALSE;
		}


		// page table doesn't exist yet
		// allocate new page table space
		tabPhysAddress = GetFreePhysChunk();
		// and mark it as used
		SETnthBIT(memManage->usedPageTables, 32, dirIndex);

		allocatedPageDir = TRUE;

		if (tabPhysAddress == 0) {
			KernelThrowException("MapPhysToVirt: Unable to allocate page table!", EXC_OUT_OF_MEMORY, 0, 0, 0);
			return FALSE;
		}

		SemaphoreEnter(M_PhysicalAccess);
		// now we have the physical adddress, but we don't have access to the phys.
		// memory at this address, so we need a virtual address to get access to it
		newPageTableVirt = OpenPhysMemory((UINT32*)tabPhysAddress, TRUE);

		// reset content
		memset(newPageTableVirt, 0x00, PAGE_SIZE);

		// page dir entry set to user and write, because it has a higher priority than
 		// page-table entries and would otherwise overwrite them!
		// The page-table entry can -of course- still be set to supervisor/read only.
		pageDirectory[dirIndex] = (tabPhysAddress & PAGE_NO_OFFSET) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;

	}

	// write to virtual address, hence to physical page table
	ASSERT(newPageTableVirt[tabIndex] == 0);
	newPageTableVirt[tabIndex] = (physAddress & PAGE_NO_OFFSET) | PAGE_PRESENT | attributes;
	UpdateTLB((UINT32*)virtAddress);
	SemaphoreLeave(M_PhysicalAccess);

	return allocatedPageDir;
}

/**
 * Allocates memory of size length to virtual address space. The physical memory will not be requested but taken from the provided pointer
 * @param usersMem thread's memory struct
 * @param physAddress where the real memory lives
 * @param length length of that mapping
 * @param attributes MMU attributes (write, read, kernel etc.)
 * @return a virtual address that points to the physical one specified. 0 if unsuccessfull.
 */
MEMPROFILE void* AllocatePhysicalMemory(MEM_MANAGE *memManage, UINT32 physicalAddress, UINT32 length, UINT32 attributes) {
	UINT32 reservedVirtAddress;
	UINT32 tmpFreeVirtAddress;
	UINT32 pageOffset = physicalAddress % PAGE_SIZE;
	UINT32 pageStart = physicalAddress - pageOffset;
	length += pageOffset;

	UINT32 pageCount = NEEDEDPAGES(length);

	tmpFreeVirtAddress = GetFreeSpaceStart(&memManage->bitmaps, pageCount, MEMTYPE_DATA);
	if (UNLIKELY(tmpFreeVirtAddress == 0)) {
		return 0;
	}

	MarkAbsoluteAddressAsUsed(&memManage->bitmaps, tmpFreeVirtAddress, pageCount, MEMTYPE_DATA);

	/* it is free, allocate contiguous phys. mem and map it to the  virtual contiguous pages */
	reservedVirtAddress = tmpFreeVirtAddress;
	while (pageCount--) {
		MapPhysToVirt(memManage, reservedVirtAddress, pageStart, attributes);
		reservedVirtAddress += PAGE_SIZE;
		pageStart += PAGE_SIZE;
	}

	return (UINT32*)(tmpFreeVirtAddress + pageOffset);
}


MEMPROFILE UINT32 VirtualAlloc(MEM_MANAGE *memManage, MEMORY_KIND kind, UINT32 pageCount, UINT32 allocPagesSkip, UINT32 allocPagesPadd, UINT32 attributes) {
	// is virtual memory free?
	// Automatically add the protection space twice (front and end)
	UINT32 tmpFreeVirtAddress = GetFreeSpaceStart(&memManage->bitmaps, pageCount, kind);
	if (UNLIKELY(tmpFreeVirtAddress == 0)) {
		// no, it isn't
		ASSERT(FALSE);
		return 0;
	}

	AllocToVirt(memManage, tmpFreeVirtAddress + allocPagesSkip * PAGE_SIZE, pageCount - allocPagesSkip - allocPagesPadd, attributes);
	return tmpFreeVirtAddress;
}

MEMPROFILE BOOL MemoryChangeAttribute(MEM_MANAGE* memManage, POINTER virtualAddress, UINT32 pageCount, UINT32 attributes, MEM_ALTER alterType) {
	while(pageCount--) {
		if (!MemoryChangePageAttribute(memManage->pageDirVirtAddress, virtualAddress, attributes, alterType)) {
			ASSERT(FALSE);
			return FALSE;
		}
		virtualAddress += PAGE_SIZE;
	}

	return TRUE;
}


void UpdateVideoMem() {
	UINT32 frameBuffer;

	frameBuffer = GetFreeSpaceStart(&kernelThread->userMemory->bitmaps, 2, MEMTYPE_DATA);
	MarkAbsoluteAddressAsUsed(&kernelThread->userMemory->bitmaps, frameBuffer, 2, MEMTYPE_DATA);

	/* map to virtual address space */
	MapPhysToVirt(kernelThread->userMemory, frameBuffer, GetFBAddress(), PAGE_SUPERV | PAGE_WRITE);
	MapPhysToVirt(kernelThread->userMemory, frameBuffer + PAGE_SIZE, GetFBAddress() + PAGE_SIZE, PAGE_SUPERV | PAGE_WRITE);

	VideoInit(frameBuffer);

}
