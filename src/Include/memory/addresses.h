/**
 * @file addresses.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Mär 10 2007
 * @see addresses.c
 * @brief Global addresses used before memory management can take effect
 */

#ifndef _addresses_H
#define _addresses_H

/**
 * The virtual address where the kernel will be loaded at (a higher half kernel)
 */
#define VIRTUAL_KERNEL_SPACE_START	0xC0000000
#define VIRTUAL_KERNEL_SPACE_END	0xFFFFDFFF

#define VIRTUAL_USER_SPACE_START	0x00000000
#define VIRTUAL_USER_SPACE_END		0xBFFFCFFF

#define PHYSICAL_KERNEL_LOAD_ADDRESS	0x00100000

// 512 MB user code
// 2560 MB user data
#define USER_CODE_SIZE			0x20000000
#define USER_DATA_SIZE			0xA0000000

#define USER_SEGMENT_SIZE		(USER_CODE_SIZE + USER_DATA_SIZE)


#define USER_CODE_PAGES			(USER_CODE_SIZE / PAGE_SIZE)
#define USER_DATA_PAGES			(USER_DATA_SIZE / PAGE_SIZE)


#define VIRTUAL_USER_CODE_START		VIRTUAL_USER_SPACE_START
#define VIRTUAL_USER_CODE_END		(VIRTUAL_USER_CODE_START + USER_CODE_SIZE - 1)
#define VIRTUAL_USER_DATA_START		(VIRTUAL_USER_CODE_START + USER_CODE_SIZE)
#define VIRTUAL_USER_DATA_END		(VIRTUAL_USER_DATA_START + USER_DATA_SIZE - 1)


// static places at end of data segment
#define STATIC_ALTER_PAGE_TABLE	(VIRTUAL_KERNEL_SPACE_END + 0x00000001)
#define STATIC_ALTER_PAGE_POINTER	(VIRTUAL_KERNEL_SPACE_END + 0x00001001)

#define DIRECT_TABLE_POINTER_POINTER (STATIC_ALTER_PAGE_POINTER + ((STATIC_ALTER_PAGE_TABLE / PAGE_SIZE) % 1024) * sizeof(UINT32))

// to avoid null-pointers
#define USER_TEXT_SEG_START		(VIRTUAL_USER_CODE_START + 0x1000)

// TSS is 3 pages before end of user's data (1 for tss + 2 for IO-bitmap)
// these two (TSS + IOBM) have to be in contiguous virtual memory and the IO-bitmap must change on every CR3-reaload.
// As the kernel is always mapped in user space, the IO-bitmap could not be changed.
// Therefore they must reside in user space (but with privileged access only via MMU)
#define STATIC_TSS_VADDR		(VIRTUAL_USER_SPACE_END + 1)
#define IO_BITMAP_VADDR			(STATIC_TSS_VADDR + PAGE_SIZE)

// The address where the TaskInformationBlock (TIB) will be stored
#define STATIC_TIB_VADDR		(VIRTUAL_USER_SPACE_END + 1 - PAGE_SIZE)

/**
 * Size of stack
 */
#define KERNEL_STACK_SIZE		8192

#endif
