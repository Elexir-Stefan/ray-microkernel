#ifndef _KERNEL_H
#define _KERNEL_H

/**
 * @file kernel.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Main RAY Kernel file
 * The NURNware RAY Kernel.
 */

/**
 * Entry point of the C-Kernel
 * @param mbi Pointer to the multiboot struct
 */
void KernelInit (multiboot_info_t *mbi);

#endif
