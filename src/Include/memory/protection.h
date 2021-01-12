#ifndef _PROTECTION_H
#define _PROTECTION_H

/**
 * @file protection.h
 * @author Stefan Nuernberger
 * @date 10-29-2006
 * @brief Functions for protection of the kernel
 * @see protection.c
 */

#include <typedefs.h>
#include <mtask/task_types.h>


void CheckUserModePointers(PROCESS_PTR process, UINT32 userModePointer);
void CheckUserModePointersCode(PROCESS_PTR process, UINT32 userModePointer);

#endif
