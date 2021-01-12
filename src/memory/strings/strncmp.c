/*
 * strncmp.c
 */

#include <typedefs.h>

int strncmp(const char *s1, const char *s2, UINT32 n) {
	const unsigned char *c1 = (unsigned char*)s1, *c2 = (unsigned char*)s2;
	unsigned char ch;
	int d = 0;

	while ( n-- ) {
		d = (int)(ch = *c1++) - (int)*c2++;
		if ( d || !ch ) {
			break;
		}
	}

	return d;
}
