#ifndef _KERNEL_PRIVATE_H
#define _KERNEL_PRIVATE_H


/**
 * @file kernel_private.h
 * @author Stefan Nuernberger
 * @date 09-13-2006
 * @brief Function declarations only used in the main kernel file.
 */

/**
 * @brief immediately halts the system
 */
static NORETURN void HaltCPU();

UNUSED static void PrintStackTrace(UINT32 address);


#endif
