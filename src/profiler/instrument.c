/**
 * @file instrument.c
 * @brief Instrumentation and profiling support
 */

#ifdef RAYPROFILING

#include <typedefs.h>
#include <profiler/useprofile.h>
#include <hal/timer.h>

extern FUNC_PROFILE_ENTRY* profilingBuffer;
extern FUNC_PROFILE_ENTRY* profilingBufferPointer;
extern FUNC_PROFILE_ENTRY* profilingBufferEnd;
extern BOOL ENABLE_PROFILING;

DONTPROFILE void __cyg_profile_func_intern(void *func, void *callsite, SAMPLEFLAGS flags) {
	if (LIKELY(ENABLE_PROFILING)) {
		if (LIKELY(profilingBufferPointer != profilingBufferEnd)) {
			profilingBufferPointer->calleeAddress = func;
			profilingBufferPointer->callerAddress = callsite;
			profilingBufferPointer->timeStamp = ReadTSC();
			profilingBufferPointer->flags = flags;

			profilingBufferPointer++;
		} else {
			ENABLE_PROFILING = FALSE;
			ProfilingError();
		}
	}
}

DONTPROFILE void __cyg_profile_func_enter(void *func, void *callsite) {
	__cyg_profile_func_intern(func, callsite, PROF_FLG_FRAMEENTER);
}

DONTPROFILE void __cyg_profile_func_exit(void *func, void *callsite) {
	__cyg_profile_func_intern(callsite, func, PROF_FLG_FRAMEDROP);
}

#endif
