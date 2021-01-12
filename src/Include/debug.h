#ifndef _DEBUG_H
#define _DEBUG_H


/**
 * @file debug.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Include this file for debugging purpose and debugging of sections
 */

#ifdef RAYDEBUG
	#include <video/video_char.h>
	#include <exceptions.h>

	#define DBG_B_MEM	1
	#define DBG_B_MUTEX	2
	#define DBG_B_IRQ	4
	#define DBG_B_SCHEDULE	8
	#define DBG_B_HASH	16


	#if (DBG_SECTIONS & DBG_B_MEM)
		#define DBG_MEMORY
	#endif

	#if (DBG_SECTIONS & DBG_B_MUTEX)
		#define DBG_MUTEX
	#endif

	#if (DBG_SECTIONS & DBG_B_IRQ)
		#define DBG_IRQ
	#endif

	#if (DBG_SECTIONS & DBG_B_SCHEDULE)
		#define DBG_SCHEDULE
	#endif

	#if (DBG_SECTIONS & DBG_B_HASH)
		#define DBG_HASH
	#endif

#define __ASSERT(cond, msg) \
if (!(cond)) { \
KernelThrowException(msg, EXC_ILLEGAL_STATE, 0, 0, 0); \
}

#define ASSERT(cond) __ASSERT(cond, "Assertion failed (" __STRINGIFY(cond) ") at "  __AT)
#define __STRINGIFY(x) #x
#define __TOSTRING(x) __STRINGIFY(x)
#define __AT __FILE__ ":" __TOSTRING(__LINE__)

#define UNIMPLEMENTED()	__ASSERT(FALSE, "Unimplemented in "  __AT)

#else
#define ASSERT(cond)
#define UNIMPLEMENTED()
#endif	/* RAYDEBUG */




#endif	/* _DEBUG_H */
