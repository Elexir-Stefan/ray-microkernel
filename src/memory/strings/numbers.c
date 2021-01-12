/**
 * @file numbers.c
 * @brief Converts from and to string numbers from and to integers
 */

#include <typedefs.h>

SINT32 atoi(CString numberString) {
	BOOL neg = FALSE;
	if (*numberString == '-') {
		neg = TRUE;
		numberString++;
	}

	SINT32 radix = 10;

	SINT32 number = 0;

	// iterate through all the digits...
	while(*numberString) {
		if ((*numberString >= '0') && (*numberString <= '9')) {
			UINT8 digit = *numberString - '0';
			number *= radix;
			number += digit;
		} else {
			// invalid number format
			return 0;
		}
		numberString++;
	}

	if (neg) {
		return -number;
	} else {
		return number;
	}
}
