/**
 * @file ioperm.c
 * @author Stefan Nuernberger
 * @date Fr Okt 27 2006
 * @brief Controls I/O permissions
 */

#include <typedefs.h>
#include <mtask/scheduler.h>
#include <hal/tss.h>
#include <app/ioperm.h>
#include <exceptions.h>
#include <memory/memory.h>
#include <memory/usermem.h>
#include <memory/addresses.h>
#include <memory/mmu.h>
#include <debug.h>

extern PROCESS_PTR currTask;
UINT32 *IOBitMap;

void InitIOPermissions() {
	IOBitMap = (UINT32*)KMalloc(PORTMAPSIZE, MEMTYPE_DATA);

	if (!IOBitMap) {
		KernelThrowException("InitIOPermissions: Out of memory", EXC_OUT_OF_MEMORY,0 ,0 ,0);
	}

	memset(IOBitMap, 0, PORTMAPSIZE);
}


BOOL CreateThreadIOPermission(PROCESS_PTR thread) {
	UINT32 *ThreadBitMap;

	ThreadBitMap = UserMalloc(currTask->userMemory, PORTMAPSIZE, FALSE, PAGE_SUPERV | PAGE_WRITE, MEMTYPE_DATA);
	if (ThreadBitMap == NULL) {
		KernelThrowException("CreateThreadIOPermission: Out of Memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
		return FALSE;
	}

	/* prohibit every single port */
	memset(ThreadBitMap, 0xff, PORTMAPSIZE);

	/**
	 * @note Only possible, if (currTask != thread)
	 * move to thread
	 */
#ifdef RAYDEBUG
	if (currTask == thread) {
		KernelThrowException("Autsch!",0,0,0,0);
	}
#endif
	//if (!MemoryGift((UINT32)ThreadBitMap, IO_BITMAP_VADDR, currTask, thread, PAGE_SUPERV | PAGE_WRITE)) {
	if (!MemoryGiftTo(currTask, thread, (UINT32)ThreadBitMap, IO_BITMAP_VADDR, MEMTYPE_DATA, MEMTYPE_DATA, PAGE_SUPERV | PAGE_WRITE)) {
		KernelThrowException("CreateThreadIOPermission: Cannot move unmapped memory!", EXC_OUT_OF_MEMORY, 0, 0, 0);
	}
	return TRUE;
}

RAY_TDM FlipIOPort(UINT16 port) {
	UINT32 *ThreadBitMap = (UINT32*)IO_BITMAP_VADDR;
	UINT32 pos = port / 32;
	UINT8 bit = port % 32;
	UINT32 pattern = 1 << bit;

	/* check if port is alread in use by another process */
	if (IOBitMap[pos] & pattern) {
		// it's in use - is it the current process?
		if (ThreadBitMap[pos] & pattern) {
			// clear the bit
			ThreadBitMap[pos] &= ~pattern;
			return TDM_SUCCESS;
		}
		ASSERT(FALSE);
		return TDM_IMPOSSIBLE;
	} else {
		/* it's still free, open the port */
		ThreadBitMap[pos] ^= pattern;
		IOBitMap[pos] ^= pattern;
	}

	return TDM_SUCCESS;
}
