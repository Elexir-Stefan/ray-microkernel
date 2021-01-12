#ifndef _IDT_H
#define _IDT_H

/**
 * @file idt.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Interrupt Descriptor Table
 * Functions for registering interrupt-handlers and processor exception handlers
 */

/**
 * Entry for an interrupt handler
 */
typedef struct {
	UINT16 baseLow;	/**< base address (lower 16 bits) */
	UINT16 sel;			/**< kernel segment */
	UINT8 always0;		/**< self explaining ;) */
	UINT8 flags;		/**< flags */
	UINT16 baseHigh;	/**< base address (upper 16 bits) */
} PACKED IDT_ENTRY;

/**
 * Pointer to the array of IDT entries
 */
typedef struct {
	UINT16 limit;	/**< size of the array in bytes -1 */
	UINT32 base;	/**< pointer to array \sa IDT_ENTRY */
} PACKED IDT_PTR;

#define I_RING0	0x0
#define I_RING1 0x20
#define I_RING2 0x40
#define I_RING3 0x60
#define I_PRESENT 0x80
#define I_INT386	0x0E

/**
 * @brief Registers an Interrupt Descriptor Table and installs IRQ handling (not -handlers!)
 */
void KernelIDTRegister();
void IDTLoad(volatile IDT_PTR* idtPtr);

#endif
