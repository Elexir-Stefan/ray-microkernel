#ifndef _IO_H
#define _IO_H

#define asmv(x...) __asm__ __volatile__(x)

/**
 * @file io.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Port input/output
 * Hardware abstraction layer to write to different ports of different sizes
 */

/**
 * input a byte from a port
 * @param port port number to read from
 */
__attribute__ ((unused)) static inline UINT8 InPortB(UINT16 port) {
	UINT8 ret;
	asmv ("inb %w1, %b0" : "=a"(ret):"d"(port));
	return ret;
}

/**
 * write a byte to a port
 * @param port number to write to
 */
__attribute__ ((unused))  static inline void OutPortB(UINT16 port, UINT8 value) {
	asm ("outb %%al, %%dx"::"d"(port), "a" (value));
}

/**
 * input a word from a port
 * @param port port number to read from
 */
static inline UINT16 InPortW(UINT16 port) {
	UINT16 ret;
	asmv ("inw %w1, %b0" : "=a"(ret):"d"(port));
	return ret;
}

/**
 * write a word to a port
 * @param port port number to write to
 */
static inline void OutPortW(UINT16 port, UINT16 value) {
	asmv ("outw %%ax, %%dx"::"d"(port), "a" (value));
}

#endif
