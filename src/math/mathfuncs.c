/**
 * @file mathfuncs.c
 * @author Stefan Nuernberger
 * @date 11-09-2006
 * @brief math functions
 * Implementation of several math functions for internal calculation
 */

#define MAX_TRIES 10
#define FAST_START 550

#include <typedefs.h>
#include <math/mathfuncs.h>


UINT32 MathSquareRoot(UINT32 value) {
	UINT8 tries;
	UINT32 sqr_est = FAST_START;

	for (tries = 0; tries < MAX_TRIES; tries++) {
		sqr_est = (sqr_est + value/sqr_est)>>1;
	}
	return sqr_est;
}

BOOL IsPrime(UINT32 toCheck) {
	UINT32 maxCheck = MathSquareRoot(toCheck);
	UINT32 checkPrime;
		for (checkPrime = 3; checkPrime < maxCheck; checkPrime++) {
			if ((toCheck % checkPrime) == 0) {
				return FALSE;
			}
		}
		return TRUE;
}

UINT32 GetNextPrime(UINT32 start) {
	//  change to odd number if it isn't already
	if ((start % 2) == 0) start++;

	// check only odd numbers
	while(!IsPrime(start)) {
		start += 2;
	}
	return start;
}
