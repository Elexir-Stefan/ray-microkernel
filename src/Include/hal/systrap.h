/**
 * @file systrap.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Mär 10 2008
 * @see systrap.c
 * @brief Summary
 */

#ifndef _systrap_H
#define _systrap_H

#define KERNEL_SYSTEM_TRAP	0xC1

#ifndef ASM
/**
 * Entrypoint for the system kernel interrupt
 */
void SysTrap();
#endif

#endif
