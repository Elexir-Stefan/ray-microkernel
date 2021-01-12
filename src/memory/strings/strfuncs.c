/**
 * @file strfuncs.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date 2009/02/12
 * @brief Advanced string functions
 */

#include "typedefs.h"
#include "memory/strings/strings.h"

BOOL StrEndsWith(CString lookIn, CString lookFor) {
	int lengthIn = strlen(lookIn);
	int lengthFor = strlen(lookFor);

	if (lengthIn >= lengthFor) {
		CString start = lookIn + (lengthIn - lengthFor);
		return (StrEquals(start, lookFor));
	} else {
		// too short to be contained in lookIn
		return FALSE;
	}
}
