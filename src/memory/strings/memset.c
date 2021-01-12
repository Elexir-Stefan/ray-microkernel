/*
 * memset.c
 */

#include <typedefs.h>

MEMPROFILE void *memset(void *dst, UINT8 c, UINT32 n) {
	char *q = dst;

	UINT32 nl = n >> 2;
	asm volatile("cld\n"
	             "rep\n"
	             "stosl\n"
	             "movl %3,%0\n"
	             "rep\n"
	             "stosb"
	             : "+c" (nl), "+D" (q)
	             : "a" ((unsigned char)c * 0x01010101U), "r" (n & 3));
	return dst;
}

void memsetw(UINT16 *dst, UINT16 c, UINT32 n) {
	while ( n-- ) {
		*dst++ = c;
	}
}
