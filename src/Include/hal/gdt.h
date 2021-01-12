#ifndef _GDT_H
#define _GDT_H

/**
 * @file gdt.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief File used for initializing the global descriptor table (GDT)
 */

#define G_SEGMENT	0x80
#define G_RING0		0x00
#define G_RING1		0x20
#define G_RING2		0x40
#define G_RING3		0x60
#define G_DESCR		0x10
#define G_CODE_S	0x08
#define G_DATA_S	0x00
#define G_READEXEC	0x02
#define G_32BIT		0x08

#define G_GRAN4K	0x80
#define G_OPER16	0x00
#define G_OPER32	0x40

#define TSS_386_FREE	0x09
#define TSS_386_OCC		0x0B

#define GDT_NULLPOINTER 0
#define GDT_CODE_RING0	1
#define GDT_DATA_RING0	2
#define GDT_CODE_RING3	3
#define GDT_DATA_RING3	4
#define GDT_TSS			5
#define GDT_TIB			6

#define DPL0	0
#define DPL3	3


#define KERNEL_SEG_CODE ((GDT_CODE_RING0<<3) | DPL0)
#define KERNEL_SEG_DATA ((GDT_DATA_RING0<<3) | DPL0)
#define KERNEL_SEG_TIB ((GDT_TIB<<3) | DPL0)

#define USER_SEG_CODE ((GDT_CODE_RING3<<3) | DPL3)
#define USER_SEG_DATA ((GDT_DATA_RING3<<3) | DPL3)
#define USER_SEG_TIB ((GDT_TIB<<3) | DPL3)

#define NUM_GDTs 7


/**
 * A single entry describing a piece of memory
 */
typedef struct gdt_entry {
	UINT16 limitLow;	/**< The limit for the memory block */
	UINT16 baseLow;		/**< The base for the memory block (lower 16 bits) */
	UINT8 baseMiddle;	/**< The base for the memory block (middle 8 bits) */
	UINT8 access;		/**< acces flags */
	UINT8 granularity;	/**< granularity (0=1 Byte, 1=4KB) */
	UINT8 baseHigh;		/**< The base for the memory block (upper 8 bits) */
} PACKED GDT_ENTRY;

/**
 * A pointer used to point to the array of GDT structs
 */
typedef struct gdt_ptr {
	UINT16 limit;	/**< size of the array in bytes -1 */
	UINT32 base;	/**< pointer to the array */
} PACKED GDT_PTR;

/**
 * Sets up a Globas Descriptor Table
 * @param load_start Memory address to use for internal GDT data
 * @param mem_upper Amout of memory in bytes
 */
void KernelInitGDT();

#endif
