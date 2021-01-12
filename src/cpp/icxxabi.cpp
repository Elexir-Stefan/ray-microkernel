/**
 * @file icxxabi.cpp
 *
 * @date 04.01.2010
 * @author stefan
 * @brief C++ ABI stub
 */

#include <typedefs.h>
#include "./icxxabi.h"

	#ifdef __cplusplus
	extern "C" {
	#endif

atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
uarch_t __atexit_func_count = 0;

	#ifdef __cplusplus
	};
	#endif

	namespace __cxxabiv1
	{
		/* guard variables */

		/* The ABI requires a 64-bit type.  */
		__extension__ typedef int __guard __attribute__((mode(__DI__)));

		extern "C" int __cxa_guard_acquire (__guard *);
		extern "C" void __cxa_guard_release (__guard *);
		extern "C" void __cxa_guard_abort (__guard *);

		extern "C" int __cxa_guard_acquire (__guard *g)
		{
			return !*(char *)(g);
		}

		extern "C" void __cxa_guard_release (__guard *g)
		{
			*(char *)g = 1;
		}

		extern "C" void __cxa_guard_abort (__guard *)
		{

		}
	}
