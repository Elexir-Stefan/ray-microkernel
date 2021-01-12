#ifndef _MEMORY_H
#define _MEMORY_H

/**
 * @file memory.h
 * @date 09-11-2006
 * @brief Memory management for user- and kernel mode
 */

#include <memory/memfunc.h>
#include <mtask/task_types.h>
#include <memory/mmu.h>
#include <flags.h>

/**
 * Changes the attributes of an allocated piece of memory
 * @param thread Thread of which the address belongs to
 * @param address Address that was return by UserMalloc (represeting whole array...)
 * @param newAttributes New Attributs
 * @param alterType How the bits are applied (set, unset, reset...)
 */
BOOL MemoryChangeAllocAttributes(PROCESS_PTR thread, POINTER address, UINT32 newAttributes, MEM_ALTER alterType);

#ifdef RAY_FLAG_KERNEL_STRUCTURE_PROTECTION
	extern PROCESS_PTR kernelThread;
	#define ProtectKernelStructure(address) MemoryChangeAllocAttributes(kernelThread, address, PAGE_WRITE, MEM_ATTR_DISABLE)
	#define UnProtectKernelStructure(address) MemoryChangeAllocAttributes(kernelThread, address, PAGE_WRITE, MEM_ATTR_ENABLE)
#else
	#define ProtectKernelStructure(address)
	#define UnProtectKernelStructure(address)
#endif


POINTER __attribute__ ((malloc)) KMalloc(UINT32 size, MEMORY_KIND memKind);
POINTER __attribute__ ((malloc)) KMallocUnProtected(UINT32 size, MEMORY_KIND memKind);
POINTER KFree(POINTER address, MEMORY_KIND kind, BOOL instantanious);
UINT32 *OpenPhysMemory(UINT32 *physAddr, BOOL write);
void* AllocatePhysicalMemory(MEM_MANAGE *usersMem, UINT32 physAddress, UINT32 length, UINT32 attributes);

#endif
