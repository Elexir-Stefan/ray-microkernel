#ifndef _USERMEM_H
#define _USERMEM_H

/**
 * @file usermem.h
 * @author Stefan Nuernberger
 * @date 09-23-2006
 * @brief Memory allocation functions that target userspace page directory etc.
 */

#include "memory/memfunc.h"
#include "memory/paging.h"
#include "mtask/task_types.h"



BOOL PrepareUserPaging(PROCESS_PTR newThread);
void FreeThreadMemory(PROCESS_PTR thread);
POINTER __attribute__ ((malloc)) UserMalloc(MEM_MANAGE *usersMem, UINT32 size, BOOL useProtection, UINT32 attribute, MEMORY_KIND memKind);
UINT32 GetMemorySize(MEM_MANAGE *usersMem, CPOINTER address);
POINTER UserFree(MEM_MANAGE *usersMem, POINTER address, MEMORY_KIND kind, BOOL instantanious);
UINT32 MemoryGift(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind, UINT32 newAttributes);
UINT32 MemoryGiftTo(PROCESS_PTR fromThread, PROCESS_PTR toThread, UINT32 fromAddress, UINT32 toAddress, MEMORY_KIND fromKind, MEMORY_KIND toKind, UINT32 newAttributes);

#endif
