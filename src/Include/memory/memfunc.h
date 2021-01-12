#ifndef _MEMFUNC_H
#define _MEMFUNC_H

/**
 * @file memfunc.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Prototypes of functions needed for memory management
 */

/**
 * copies count bytes from src to dest
 * @param dest Pointer to where data will be copied to
 * @param src Pointer from where data will be copied
 * @param count Amount of data to copy in bytes
 * @return dest
 */
POINTER memcpy(POINTER dest, CPOINTER src, UINT32 count);

/**
 * overwrites count bytes with c at position dest
 * @param dest a pointer to the beginning of the erasure
 * @param c the byte that will be written at every position
 * @param count length of the overwriting
 * @return dest
 */
POINTER memset(POINTER dest, UINT8 c, UINT32 count);

/**
 * overwrites count words with c at position dest
 * @param dest a pointer to the beginning of the erasure
 * @param c the word that will be written at every position
 * @param count length of the overwriting
 * @return dest
 */
POINTER memsetw(POINTER dest, UINT16 c, UINT32 count);

/**
 * Moves count bytes from src to dst
 * @param dst Pointer to where data will be moved to
 * @param src Pointer from where data will be moved
 * @param count amount of bytes to move
 * @return dest
 */
POINTER memmove(POINTER dst, const POINTER src, UINT32 count);

#endif
