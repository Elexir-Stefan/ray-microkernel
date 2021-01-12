#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

/**
 * @file typedefs.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Common type definitions of the RAY kernel
 * Common type definitions
 */


#define UNUSED __attribute__ ((unused))
#define DONTPROFILE __attribute__ ((no_instrument_function))
#define NORETURN __attribute__((noreturn))

// Decide, whether to profile memory-related functions or not
#define MEMPROFILE DONTPROFILE

#define PACKED __attribute__ ((packed))

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef unsigned long long UINT64;

typedef signed char  SINT8;
typedef signed short SINT16;
typedef signed int   SINT32;
typedef signed long long SINT64;

typedef char* String;
typedef const char* CString;

typedef enum {
	FALSE = 0,
	TRUE = 1
} PACKED BOOL;

typedef void* POINTER;
typedef const void* CPOINTER;
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((POINTER)0)
#endif
#endif



#define NewType(baseType, newType) typedef union {baseType Value; UINT32 CastIntValue; UINT8 GetByte[4];} newType;

#ifndef __cplusplus
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#else /* C++ */
/* The reference cast is necessary to thwart an operator& that might
   be applicable to MEMBER's type.  See DR 273 for details.  */
#define offsetof(TYPE, MEMBER) (reinterpret_cast <size_t> \
    (&reinterpret_cast <char &>(static_cast <TYPE *> (0)->MEMBER)))
#endif /* C++ */

#include "compilerspec.h"

#endif
