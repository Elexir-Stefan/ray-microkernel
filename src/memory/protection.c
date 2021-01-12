/**
 * @file protection.c
 * @author Stefan Nuernberger
 * @date So Okt 29 2006
 * @brief
 * Provided functions will hide the kernel memory from the page table so that
 * potential harmful tasks, that will be executed in RING0 cannot manipulate
 * kernel memory
 */

#include <typedefs.h>
#include <memory/adt/hashmap.h>
#include <memory/paging.h>
#include <memory/protection.h>

/**
 * Checks the pointst from user mode.
 * Pointer passed to kernel space can be potentially harmful as they
 * may intentionally point to kernel mode structures
 * @param process
 * @param userModePointer
 */
void CheckUserModePointers(UNUSED PROCESS_PTR process, UNUSED UINT32 userModePointer) {
	//MEM_MANAGE* memManage = process->userMemory;

	//memManage->bitmaps.start[MEMTYPE_DATA]
}

void CheckUserModePointersCode(UNUSED PROCESS_PTR process, UNUSED UINT32 userModePointer) {

}
