/*
 * strcmp.c
 */

#include <typedefs.h>

int strcmp(CString s1, CString s2) {
	while(*s1 == *s2) {
		    if (0 == *s1) {
			return 0;
		    }
		    s1++;
		    s2++;
		}
		return *s1 - *s2;
}

BOOL StrEquals(CString s1, CString s2) {
	return strcmp(s1, s2) == 0;
}

