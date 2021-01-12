/**
 * @file compilerspec.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Dez 31 2007
 * @see compilerspec.c
 * @brief Summary
 */

#ifndef _compilerspec_H
#define _compilerspec_H


#define LIKELY(arg)	__builtin_expect(!!(arg), 1)
#define UNLIKELY(arg)	__builtin_expect(!!(arg), 0)


// cache 
#define PREFETCH(address, rw, locality)		__builtin_prefetch((void*)(address), rw, locality)
#define CACHE_WRITE_LIKELY			1
#define CACHE_READ_LIKELY			0
#define CACHE_NO_FURTHER_USE			0
#define CACHE_PROBABLE_USE			1
#define CACHE_FURTHER_USE			3

#endif
