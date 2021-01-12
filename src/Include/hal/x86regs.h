/**
 * @file x86regs.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Aug 25 2007
 * @brief Registers for x86 when fireing an interrupt or exception
 */

#ifndef _x86regs_H
#define _x86regs_H

typedef struct {
	UINT32 gs, fs, es, ds;
	UINT32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	UINT32 intNum;
	UINT32 eip, cs, eflags, userESP, ss;
} PACKED INT_REG;

#endif
