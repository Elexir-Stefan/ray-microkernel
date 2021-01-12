/*
 * strcpy.c
 *
 * strcpy()
 */

#include <typedefs.h>
#include <memory/strings/strings.h>

CString strcpy(CString dst, CString src) {
	String q = (String)dst;
	char ch;

	do {
		ch = *q++ = *src++;
	} while ( ch );

	return dst;
}

String strcat(String destination, CString concat) {
	UINT32 pos = strlen(destination);

	strcpy(destination + pos, concat);

	return destination + pos;
}
