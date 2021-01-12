/*
 * strlen()
 */

#include <typedefs.h>

UINT32 strlen(CString s) {
	UINT32 len = 0;
    while (*s) {
        s++;
        len++;
    }
    return len;
}

UINT32 strnlen(CString s, UINT32 maxLength) {
	const char *ss = s;
	while ((*ss) && (maxLength--)) {
		ss++;
	}
	return ss-s;
}
