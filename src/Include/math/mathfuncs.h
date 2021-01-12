#ifndef _MATHFUNCS_H
#define _MATHFUNCS_H

/**
 * @file mathfuncs.h
 * @date 09-11-2006
 * @brief Mathematical functions (header)
 */

/**
 * @param value to be square-rooted
 * @return the square-root
 */
UINT32 MathSquareRoot (UINT32 value);

/**
 * Checks whether a natural number is prime or not
 * @param toCheck Number to check
 */
BOOL IsPrime(UINT32 toCheck);

/**
 * Returns the next prime starting at <code>start</code>
 * @param start Number to start search at
 */
UINT32 GetNextPrime(UINT32 start);

/**
 * Calculates the minimum of two values a and b
 */
#define MathMin(a,b) ((a < b) ? a : b)

/**
 * Calculates the minimum of two values a and b
 */
#define MathMax(a,b) ((a > b) ? a : b)

/**
 * Rounds up (ceils) the division of a by b
 */
#define CeilDiv(a,b) ((a % b) ? ((a / b) + 1) : (a / b))

#endif
