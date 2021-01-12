#ifndef _8259_H
#define _8259_H

/**
 * @file 8259.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Functions that allow to control the Programmable Interrupt Controller (PIC)
 */


/**
 * Tells the 8259 controller that new IRQs may be processed
 * @param irq The IRQ that is now availible again
 */
void C8259InterruptDone(UINT8 irq);

/**
 * Initializes the 8259 controller to use the specified ints for IRQs
 * @param low_start int-number of where IRQs starting at 0 will be fired
 * @param high_start int-number of where IRQs starting at 8 will be fired
 * @sa C8259_1_master C8259_2_slave C8259_1_mode8086 C8259_2_mode8086
 */
void C8259Init(UINT8 low_start, UINT8 high_start);

/**
* Sets 1st PIC as master
*/
void C8259_1_Master(void);

/**
 * Sets 1st PIC as slave
 */
void C8259_1_Slave(void);

/**
 * Sets 2nd PIC as master
 */
void C8259_2_Master(void);

/**
 * sets 2nd PIC as slave
 */
void C8259_2_Slave(void);

/**
 * @brief Unmasks an IRQ
 * irq_num IRQ number to mask
 * \sa C8259_mask_irq
 */
void C8259UnmaskIRQ(UINT8 irqNum);

/**
 * Masks an IRQ
 * @param irq_num Number to mask
 * \sa C8259_unmask_irq
 */
void C8259MaskIRQ(UINT8 irqNum);

/**
 * Sets 1st PIC to 8086 mode
 */
void C8259_1_Mode8086(void);

/**
 * Sets 1st PIC to 8086 mode
 */

void C8259_2_Mode8086(void);

/**
 * Sets a complete mask for IRQs 0-7
 * \sa C8259_set_irqmask_hi
 */
void C8259SetIRQmaskLO(UINT8 irqMask);

/**
 * Sets a complete mask for IRQs 8-15
 * \sa C8259_set_irqmask_lo
 */
void C8259SetIRQmaskHI(UINT8 irqMask);

#endif
