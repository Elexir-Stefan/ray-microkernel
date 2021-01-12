/**
 * @file kdebug.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 12 2008
 * @see kdebug.h
 * @brief RAY Kernel level debugger
 */

#ifdef RAY_KERNEL_DEBUGGER

#include <typedefs.h>
#include <hal/uart.h>
#include <hal/uartports.h>
#include <kdebug/kdebug.h>
#include <kdebug/i386-stub.h>
#include <video/video_char.h>

void OverrideKernelISR(UINT8 exceptionNumber, UINT32 address);

COM_PORT debuggerPort = COM1;

void KernelDebuggerInit() {
	InitUARTPort(&debuggerPort, 115200, UART8Bits, UARTOneStop, UARTNoParity);
	SetDebugComPort(&debuggerPort);
	printf ("Kernel debugger support started.\n");

	set_debug_traps();

	printf ("Waiting for gdb... ");
	BREAKPOINT();
	printf ("Debugger connected.\n");
}


void exceptionHandler(int exceptionNumber, void *exceptionAddress) {
	OverrideKernelISR(exceptionNumber, (UINT32)exceptionAddress);
}


#endif /* RAY_KERNEL_DEBUGGER */
