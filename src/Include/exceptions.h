#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

/**
 * @file exceptions.h
 * @author Stefan Nuernberger
 * @date 04-10-2006
 * @brief Error numbers used for exceptions
 */

#include "hal/irq_han.h"


/**
 * displays an error message on the screen and halts the system.
 * @param message The message that will appear on the screen describing the error
 * @param errNumber Error number
 * @param addInfo Number for additional information
 */
void KernelThrowException(const char *message, UINT32 errNumber, UINT32 addInfo, UINT8 showFlags, INT_REG *regs);

#define EXC_FLAG_SHOWREGS		0x001

/* kernel mode */
#define EXC_FEATURE_PROCESSOR		0x050

#define EXC_OUT_OF_MEMORY		0x100
#define EXC_INIT_NO_MEMFLAG		0x101
#define EXC_INIT_NO_BOOTDEV		0x102
#define EXC_INIT_NO_PARAMFL		0x103
#define EXC_INIT_NOT_ELF		0x104
#define EXC_INIT_NO_MMAP		0x105
#define EXC_MEMORY_ALLOC_ERR	0x106
#define EXC_HASH_ERROR			0x107
#define EXC_ENTRY_NOT_FOUND 	0x108
#define EXC_RELOC_FAILED		0x109
#define EXC_EXTERNAL_NOT_FOUND	0x10A
#define EXC_PAGING_ERR			0x10B
#define EXC_ILLEGAL_STATE		0x200
#define EXC_UNKNOWN_INTERRUPT	0x400
#define EXC_PROFILING			0x500

/* user mode */
#define EXC_UNKNOWN_SYSCALL		0x200
#define EXC_INSUFFICIENT_RIGHTS	0x201

/* general stuff */
#define EXC_MEM_NOT4k			0x500

#endif
