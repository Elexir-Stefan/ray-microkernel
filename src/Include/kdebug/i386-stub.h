/**
 * @file i386-stub.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Do Mär 13 2008
 * @see i386-stub.c
 * @brief Summary
 */

#ifndef _i386_stub_H
#define _i386_stub_H

void set_debug_traps();

#define BREAKPOINT()	__asm__("int $3")

#endif
