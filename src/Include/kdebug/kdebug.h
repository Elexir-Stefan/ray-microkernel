/**
 * @file kdebug.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 12 2008
 * @see kdebug.c
 * @brief Kernel debugging support
 */

#ifdef RAY_KERNEL_DEBUGGER

#ifndef _kdebug_H
#define _kdebug_H

void KernelDebuggerInit();

void exceptionHandler(int exceptionNumber, void *exceptionAddress);


#endif /*_kdebug_H */

#endif /* RAY_KERNEL_DEBUGGER */
