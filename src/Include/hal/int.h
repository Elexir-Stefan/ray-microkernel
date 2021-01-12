#ifndef _INT_H
#define _INT_H

/**
 * @file int.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Enable and disable interrupts
 */

/** 
 * disables interrupts
 */
static inline void KernelInterruptsDisable(void){
	__asm__ __volatile__ ("cli"::);
}

/**
 * enables interrupts
 */
static inline void KernelInterruptsEnable(void){
	__asm__ __volatile__ ("sti"::);
}

#endif
