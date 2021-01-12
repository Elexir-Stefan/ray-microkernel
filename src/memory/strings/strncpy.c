/*
 * strncpy.c
 *
 * strncpy()
 */

#include <typedefs.h>

char *strncpy(char *dst, const char *src, UINT32 n) {
	char *q = dst;
	const char *p = src;
	char ch;

	while ( n-- ) {
		*q++ = ch = *p++;
		if ( !ch ) {
			break;
		}
	}

	return dst;
}
