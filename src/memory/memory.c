/**
 * @file memory.c
 * @author Stefan Nürnberger [NURNware Technologies] stefan@nurnware.de
 * @date 20-11-2006
 * @brief Memory functions for kernel and user mode to allocate and free (paged) memory
 */

#include <flags.h>

#include <typedefs.h>
#include <mtask/semaphore.h>
#include <exceptions.h>
#include <memory/memory.h>
#include "memory/memory_types.h"
#include <memory/adt/hashmap.h>
#include <memory/paging.h>
#include <memory/mmu.h>
#include <memory/addresses.h>
#include <memory/memtune.h>
#include <mtask/task_types.h>
#include <hal/atom.h>
#include <memory/usermem.h>
#include <debug.h>
#include <hal/processor.h>
#include <hal/tss.h>
#include <hal/gdt.h>
#include <app/ioperm.h>

extern PROCESS_PTR kernelThread;
extern PROCESS_PTR currTask;
extern IPLOCK M_PhysicalAccess;

extern UINT32 PAGE_TAB, PAGE_TAB_VIRT;

// internal functions
static UINT32 MemoryGiftInternal(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, UINT32 toAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind, UINT32 newAttributes);

/**
 * allocates size bytes of memory
 * @param size Amount of bytes to allocate (rounded to next 4k)
 * @param memKind Specifies the type of memory (code or data)
 * @return Pointer to virtual address (or NULL of out of memory)
 */
MEMPROFILE POINTER __attribute__ ((malloc)) KMalloc(UINT32 size, MEMORY_KIND memKind) {
	POINTER address = UserMalloc(kernelThread->userMemory, size, TRUE, PAGE_SUPERV | PAGE_WRITE | PAGE_GLOBAL, memKind);

	memset(address, 0, size);
	return address;
}

/**
 * allocates size bytes of memory (without array-out-of-bound protection)
 * @param size Amount of bytes to allocate (rounded to next 4k)
 * @param memKind Specifies the type of memory (code or data)
 * @return Pointer to virtual address (or NULL of out of memory)
 */
MEMPROFILE POINTER __attribute__ ((malloc)) KMallocUnProtected(UINT32 size, MEMORY_KIND memKind) {
	POINTER address = UserMalloc(kernelThread->userMemory, size, FALSE, PAGE_SUPERV | PAGE_WRITE | PAGE_GLOBAL, memKind);

	memset(address, 0, size);
	return address;
}

/**
 * Frees memory allocated with KMalloc
 * @param addr_ptr Virtual address to identify the allocation-information
 * @param instantanious Determines wehter memory will be inaccessable right now or after context-switch (nonetheless it's free and COULD be overwritten - ATTENTION!)
 * @return Pointer to deallocated space of successfull, NULL of address invalid
 */
MEMPROFILE POINTER KFree(POINTER address, MEMORY_KIND kind, BOOL instantanious) {
	return UserFree(kernelThread->userMemory, address, kind, instantanious);
}

/**
 * Allocates memory in user space
 * @param usersMem memory management structure of a user task
 * @param size size to allocate in bytes
 * @param useProtection Decides whether to use boundary protection or not
 * @param attribute attributes of the memory to be allocated
 * @param memKind specifies the type of memory (code or data)
 * @return a pointer to a virtual address
 */
MEMPROFILE POINTER __attribute__ ((malloc)) UserMalloc(MEM_MANAGE *usersMem, UINT32 size, BOOL useProtection, UINT32 attribute, MEMORY_KIND memKind) {
	UINT32 resVirtAddress;
	UINT32 usablePages = NEEDEDPAGES(size);
	UINT32 pages = usablePages + useProtection * RAY_FLAG_MEMALLOC_PROTECTION * 2;

	if (LIKELY(size)) {
		AtomicSectionStart();


		resVirtAddress = VirtualAlloc(usersMem,  memKind, pages, RAY_FLAG_MEMALLOC_PROTECTION * useProtection, RAY_FLAG_MEMALLOC_PROTECTION * useProtection, attribute);
		if (LIKELY(resVirtAddress)) {
			ASSERT((resVirtAddress & PAGE_OFFSET) == 0);
			// Mark the found space as used
			MarkAbsoluteAddressAsUsed(&usersMem->bitmaps, resVirtAddress, pages, memKind);

			// Prefetch the first page to the cache, as it very likely
			// that it will be used right after allocation. This safes
			// a few processor cycles as the prefetch is taking place
			// right now while the program continues, as we don't touch
			// the newly allocated data, hence no dependencies stall
			// the processor right now.
			PREFETCH(resVirtAddress, CACHE_WRITE_LIKELY, CACHE_PROBABLE_USE);

			ALLOCATION_INFO info;
			info.length = size;
			info.pages = pages;
			info.usablePages = usablePages;
			info.addressStart = resVirtAddress;
			info.paddEndPages = RAY_FLAG_MEMALLOC_PROTECTION * useProtection;
			info.paddStartPages = RAY_FLAG_MEMALLOC_PROTECTION * useProtection;
			info.usableAddressStart = resVirtAddress + (RAY_FLAG_MEMALLOC_PROTECTION * useProtection * PAGE_SIZE);

#ifdef RAY_FLAG_ARRAYBOUND_PROTECTION
			 // protection against writing out of boundaries
			info.usableAddressStart += ALIGNTOPAGEEND(size);
#endif

			HashInsert(usersMem->memAllocs, info.usableAddressStart, &info);

			usersMem->threadMemUsage += size;
			usersMem->threadMemPages += pages;

			AtomicSectionEnd();
			// if wanted, correct pointer to the real start instead of the
			// padding protected start
			return (POINTER)info.usableAddressStart;
		} else {
			AtomicSectionEnd();
			ASSERT(FALSE);
			return NULL;
		}
	} else {
		KernelThrowException("UserMalloc: Tried to allocate zero bytes!", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return NULL;
	}
}

/**
 * Unmaps and frees a chunk of (virtual) memory
 * Releases the physical memory to the free-stack
 * @param usersMem memory management structure of a user task
 * @param address virtual address
 * @param instantanious Determines wether memory will be accessible within the current timeslice (updates the TLB or not)
 * @return the pointer of the released address or NULL of unsuccessfull
 */
MEMPROFILE POINTER UserFree(MEM_MANAGE *usersMem, POINTER address, MEMORY_KIND kind, BOOL instantanious) {
	AtomicSectionStart();

	ALLOCATION_INFO* info= (ALLOCATION_INFO*)HashRetrieve(usersMem->memAllocs, (UINT32)address);
	if (LIKELY(info)) {
		ASSERT(info->length);

		ASSERT((info->addressStart & PAGE_OFFSET) == 0);
		ReleaseVirtualMemChannel(usersMem, info->usableAddressStart & PAGE_NO_OFFSET, info->usablePages, TRUE, instantanious);
		// mark the whole virtual memory (including padded protection) as free
		MarkAbsoluteAddressAsFree(&usersMem->bitmaps, info->addressStart, info->pages, kind);
		UNUSED BOOL delRes = HashDelete(usersMem->memAllocs, (UINT32)address);
		ASSERT(delRes);

		usersMem->threadMemUsage -= info->length;
		usersMem->threadMemPages -= info->pages;

		AtomicSectionEnd();
		return address;
	} else {
		AtomicSectionEnd();
		KernelThrowException("Tried to free non-allocated or already freed memory!", EXC_OUT_OF_MEMORY,(UINT32)address,0,0);
		return NULL;
	}
}

UINT32 GetMemorySize(MEM_MANAGE *usersMem, CPOINTER address) {
	AtomicSectionStart();

		UINT32 size = 0;

		ALLOCATION_INFO* info= (ALLOCATION_INFO*)HashRetrieve(usersMem->memAllocs, (UINT32)address);
		if (LIKELY(info)) {
			ASSERT(info->length);
			ASSERT((info->addressStart & PAGE_OFFSET) == 0);

			size = info->length;

		} else {
			ASSERT(FALSE);
		}


	AtomicSectionEnd();

	return size;

}

BOOL MemoryChangeAllocAttributes(PROCESS_PTR thread, POINTER address, UINT32 newAttributes, MEM_ALTER alterType) {
	ALLOCATION_INFO* info= (ALLOCATION_INFO*)HashRetrieve(thread->userMemory->memAllocs, (UINT32)address);
	if (LIKELY(info)) {
		return MemoryChangeAttribute(thread->userMemory, (POINTER)info->usableAddressStart, info->usablePages, newAttributes, alterType);
	} else {
		ASSERT(FALSE);
		return FALSE;
	}

}

/**
 * Cleares all memory occupied to manage paging for that thread
 * @param thread
 */
void FreeThreadMemory(PROCESS_PTR thread) {
	HASH *memAllocs = thread->userMemory->memAllocs;
	UINT32 hashSize = memAllocs->primeSize;
	HASH_ENTRY *curr = memAllocs->hashTabPtr;
	UINT32 entries = memAllocs->numEntries;
	UINT32 pageTablePhysAddress;
	UINT32 i;

	// TODO: Please! Make this nicer, really ugly method to iterate over a hash map.
	// But, the question is: How else?


	for (i = 0; i < hashSize; i++) {
		while (OCCUPIED_E(curr)) {
			entries--;
			UserFree(thread->userMemory, (UINT32*)curr->hashKey, MEMTYPE_DATA, TRUE);
		}
		curr += memAllocs->entryStep;
	}
	if (entries != 0) {
		KernelThrowException("Not all entries could be deleted!", EXC_OUT_OF_MEMORY, entries, 0,0);
	}

	HashDestroy(memAllocs);

	if (thread->userMemory->threadMemUsage != 0) {
		KernelThrowException("FreeThreadMemory: Something's left?!", EXC_OUT_OF_MEMORY, thread->userMemory->threadMemUsage, 0, 0);
	}
	if (thread->userMemory->threadMemPages != 0) {
		KernelThrowException("FreeThreadMemory: Something's left?!", EXC_OUT_OF_MEMORY, thread->userMemory->threadMemPages, 0, 0);
	}

	// delete dynamically allocated page tables
	for (i = 0; i < 1024; i++) {
		if (ISnthBITSET(thread->userMemory->usedPageTables, 32, i)) {
			pageTablePhysAddress = thread->userMemory->pageDirVirtAddress[i] & PAGE_NO_OFFSET;
			// free the memory used by it
			SetFreePhysChunk(pageTablePhysAddress);
		}
	}

	KFree(thread->userMemory->bitmaps.space[MEMTYPE_DATA].bitmapSpace, MEMTYPE_DATA, TRUE);
	KFree(thread->userMemory->bitmaps.space[MEMTYPE_CODE].bitmapSpace, MEMTYPE_DATA, TRUE);
	KFree(thread->userMemory->usedPageTables, MEMTYPE_DATA, TRUE);
	KFree(thread->userMemory->pageDirVirtAddress, MEMTYPE_DATA, FALSE);
	KFree(thread->userMemory, MEMTYPE_DATA, TRUE);
}

static void MapKernelSpace(UINT32 *pageDirectory) {
	UINT32 i;
	UINT32 kernelPageDirStart = VIRTUAL_KERNEL_SPACE_START / PAGE_SIZE / NUM_TABLE_ENTRIES;
	UINT32 kernelPageDirEnd = VIRTUAL_KERNEL_SPACE_END / PAGE_SIZE / NUM_TABLE_ENTRIES;

	UINT32 *kernelPageDirectory = kernelThread->userMemory->pageDirVirtAddress;

	// complete mapped kernel space
	for (i = kernelPageDirStart; i <= kernelPageDirEnd; i++) {
		pageDirectory[i] = kernelPageDirectory[i];
	}
}

/**
 * Prepares things needed for user page tables on each thread
 * @param newThread an emtpy structure that will be the new thread
 * @return a MEM_MANAGE structure for a thread
 */
BOOL PrepareUserPaging(PROCESS_PTR newThread) {
	UINT32 *pageDirectory;
	UINT32 dirPhysAddress = 0; /* keeps compiler happy */
	MEM_MANAGE *newMemManage;

	/* allocate memory for the page-directory and map it into the new pagedir itself */
	pageDirectory = KMalloc(PAGE_SIZE, MEMTYPE_DATA);

	if (pageDirectory == NULL) {
		KernelThrowException("PrepareUserPaging: Out of memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return FALSE;
	}

	dirPhysAddress = GetPhysicalAddressOf(kernelThread->userMemory->pageDirVirtAddress, pageDirectory) & PAGE_NO_OFFSET;

	/* clear the new entries */
	memset(pageDirectory, 0, PAGE_SIZE);

	/* map kernel space to user task (but RING0 privileged) */

	newMemManage = (MEM_MANAGE*)KMalloc(sizeof(MEM_MANAGE), MEMTYPE_DATA);
	memset(newMemManage, 0, sizeof(MEM_MANAGE));

	newMemManage->pageDirVirtAddress = pageDirectory;
	newMemManage->pageDirPhysAddress = (UINT32*)(dirPhysAddress | PAGE_PRESENT);

	// create memory bitmaps
	newMemManage->bitmaps.start[MEMTYPE_DATA] = VIRTUAL_USER_DATA_START;
	newMemManage->bitmaps.stop[MEMTYPE_DATA] = VIRTUAL_USER_DATA_END;
	newMemManage->bitmaps.start[MEMTYPE_CODE] = VIRTUAL_USER_CODE_START;
	newMemManage->bitmaps.stop[MEMTYPE_CODE] = VIRTUAL_USER_CODE_END;

	newMemManage->bitmaps.space[MEMTYPE_DATA].bitmapSpace = KMalloc(BITMAPBYTES(USER_DATA_PAGES), MEMTYPE_DATA);
	newMemManage->bitmaps.space[MEMTYPE_CODE].bitmapSpace = KMalloc(BITMAPBYTES(USER_CODE_PAGES), MEMTYPE_DATA);

	if (newMemManage->bitmaps.space[MEMTYPE_DATA].bitmapSpace == NULL) {
		// out of memory
		KernelThrowException("PrepareUserPaging: Out of memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return FALSE;
	}

	if (newMemManage->bitmaps.space[MEMTYPE_CODE].bitmapSpace == NULL) {
		// out of memory
		KernelThrowException("PrepareUserPaging: Out of memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return FALSE;
	}

	InitBitmap(&newMemManage->bitmaps.space[MEMTYPE_DATA], USER_DATA_PAGES);
	InitBitmap(&newMemManage->bitmaps.space[MEMTYPE_CODE], USER_CODE_PAGES);

	// create memory hash
	newMemManage->memAllocs = HashCreateAndInit(USER_MAX_ALLOCS, ALLOCATION_INFO);

	// create page table bitmap
	newMemManage->usedPageTables = KMalloc(1024 / 8, MEMTYPE_DATA);
	if (newMemManage->usedPageTables == NULL) {
		// out of memory
		KernelThrowException("PrepareUserPaging: Out of memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return FALSE;
	}
	memset(newMemManage->usedPageTables, 0, 1024 / 8);

	// map all pages that should be accessible in the thread
	MapKernelSpace(pageDirectory);
	newThread->userMemory = newMemManage;

	// map the kernel TSS
	MapPhysToVirt(newMemManage, STATIC_TSS_VADDR, GetPhysicalAddressOf(kernelThread->userMemory->pageDirVirtAddress, (UINT32*)STATIC_TSS_VADDR) & PAGE_NO_OFFSET, PAGE_SUPERV | PAGE_WRITE);

	newThread->TIB = KMalloc(PAGE_SIZE, MEMTYPE_DATA);
	MapPhysToVirt(newMemManage, STATIC_TIB_VADDR, GetPhysicalAddressOf(kernelThread->userMemory->pageDirVirtAddress, newThread->TIB), PAGE_USER | PAGE_WRITE);

	newThread->TIB->pid = newThread->pid;
	newThread->TIB->tid = newThread->tid;

	// create & map the IO bitmap
	return CreateThreadIOPermission(newThread);
}

/**
 * 'Donates' memory from thread A to thread B
 * The memory now no longer belongs to thread A.
 * @param fromThread thread to move from
 * @param toThread thread to move to
 * @param fromAddress virtual address to move from
 * @param kind Memory type (code or data)
 * @param newAttributes page tables attributes
 * @return the linear virtual address of the remote thread on success, NULL otherwise
 */
UINT32 MemoryGift(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind,UINT32 newAttributes) {
	return MemoryGiftInternal(fromThread, toThread, fromAddress, 0, fromKind, toKind, newAttributes);
}

UINT32 MemoryGiftTo(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, UINT32 toAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind,UINT32 newAttributes) {
	return MemoryGiftInternal(fromThread, toThread, fromAddress, toAddress, fromKind, toKind, newAttributes);
}


static UINT32 MemoryGiftInternal(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, UINT32 toAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind,UINT32 newAttributes) {

	ASSERT((fromKind == MEMTYPE_DATA) || (fromKind == MEMTYPE_CODE));
	ASSERT((toKind == MEMTYPE_DATA) || (toKind == MEMTYPE_CODE));


	AtomicSectionStart();
	// get length
	ALLOCATION_INFO* info = (ALLOCATION_INFO*)HashRetrieve(fromThread->userMemory->memAllocs, fromAddress);
	if (LIKELY(info)) {

		#ifdef RAY_FLAG_ARRAYBOUND_PROTECTION
		AssertLength(info->length, fromAddress);
		#endif


		ASSERT(info->length);
		UINT32 protectionBeforeStart = info->usableAddressStart - info->addressStart;
		UINT32 protectionAfter = info->paddEndPages * PAGE_SIZE;
		if (LIKELY(!toAddress)) {
			// find free space in target thread
			toAddress = GetFreeSpaceStart(&toThread->userMemory->bitmaps, info->pages, toKind);

			toAddress += protectionBeforeStart;

		}

		if (LIKELY(toAddress)) {

			// Assure, that address fits in appropriate segment
			if ((toAddress - protectionBeforeStart < toThread->userMemory->bitmaps.start[toKind]) || (toAddress + info->length + protectionAfter - 1 > toThread->userMemory->bitmaps.stop[toKind])) {
				ASSERT(FALSE);
				return 0;
			}

			ASSERT((toAddress & PAGE_OFFSET) == (fromAddress & PAGE_OFFSET));
			ASSERT((toAddress & PAGE_OFFSET) == (info->usableAddressStart & PAGE_OFFSET));

			// update data
			ALLOCATION_INFO oldInfo;
			memcpy(&oldInfo, info, sizeof(ALLOCATION_INFO));
			info->usableAddressStart = toAddress;
			info->addressStart = info->usableAddressStart - protectionBeforeStart;

			HashInsert(toThread->userMemory->memAllocs, info->usableAddressStart, info);
			HashDelete(fromThread->userMemory->memAllocs, fromAddress);


			fromThread->userMemory->threadMemPages -= info->pages;
			toThread->userMemory->threadMemPages += info->pages;

			fromThread->userMemory->threadMemUsage -= info->length;
			toThread->userMemory->threadMemUsage += info->length;

			if (LIKELY(MoveThreadMemory(oldInfo.addressStart, info->addressStart, fromThread->userMemory, toThread->userMemory, info->pages, info->paddStartPages, info->paddEndPages, fromKind, toKind, newAttributes))) {
				AtomicSectionEnd();
				return info->usableAddressStart;
			} else {
				KernelThrowException("MemoryGiftInternal: Memory move failed!", EXC_OUT_OF_MEMORY, 0, 0 ,0);
				// memory move failed
				AtomicSectionEnd();
				return 0;
			}
		} else {
			KernelThrowException("MemoryGiftInternal: No free space left in target thread!", EXC_OUT_OF_MEMORY, 0, 0, 0);
			// no free space left in target thread
			return 0;
		}
	} else {
		KernelThrowException("MemoryGiftInternal: Source address does not exist!", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return 0;
	}
}
