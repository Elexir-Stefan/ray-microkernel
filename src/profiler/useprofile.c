/**
 * @file useprofile.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Feb 11, 2009
 * @brief Profiling management functions
 */


#ifdef RAYPROFILING
#include <typedefs.h>
#include <memory/memory.h>
#include <memory/usermem.h>
#include <exceptions.h>
#include "profiler/profiling.h"
#include <profiler/useprofile.h>
#include <hal/uart.h>
#include <hal/uartports.h>
#include <video/video_char.h>

FUNC_PROFILE_ENTRY* profilingBuffer = NULL;
FUNC_PROFILE_ENTRY* profilingBufferPointer = NULL;
FUNC_PROFILE_ENTRY* profilingBufferEnd = NULL;
static UINT32 profilingBufferEntries = 0;
BOOL ENABLE_PROFILING = FALSE;

GETPROFILE(sysCallTime);

/**
 * Enables the profiling of function calls and their TSC values
 * @param bufferEntries The number of entries in the buffer (96 byte each)
 */
void ProfilingEnable(UINT32 bufferEntries) {
	// if buffer is alredy set, throw an exception

	if (profilingBuffer) {
		KernelThrowException("Profiling buffer already initialized!", EXC_PROFILING, 1, FALSE, NULL);
	}
	profilingBuffer = KMalloc(bufferEntries * sizeof(FUNC_PROFILE_ENTRY), MEMTYPE_DATA);
	if (!profilingBuffer) {
		KernelThrowException("ProfilingEnable: Out of memory!", EXC_OUT_OF_MEMORY, 0, FALSE, NULL);
	}
	profilingBufferEntries = bufferEntries;
	ProfilingReset();
}

/**
 * Starts the profiling process
 * @note EnableProfiling must be called first!
 */
void ProfilingStart() {
	if (!profilingBuffer) {
		KernelThrowException("Profiling buffer not initialized yet!", EXC_PROFILING, 2, FALSE, NULL);
	}

	VideoSetBCol(14);
	VideoSetFCol(8);
	VideoFillLine();
	printf ("                            !!! PROFILING ENABLED !!!\n");
	VideoSetBCol(7);
	VideoSetFCol(15);

	ENABLE_PROFILING = TRUE;
}

/**
 * Stops the currently running profiling process
 */
void ProfilingStop() {
	ENABLE_PROFILING = FALSE;
	VideoSetBCol(14);
	VideoSetFCol(8);
	VideoFillLine();
	printf ("                            !!! PROFILING STOPPED !!!\n");
	VideoSetBCol(7);
	VideoSetFCol(15);
}

/**
 * Rewinds the profiling pointer and therefore frees up space to use for
 * profiling
 */
void ProfilingReset() {
	profilingBufferPointer = profilingBuffer;
	profilingBufferEnd = profilingBufferPointer + profilingBufferEntries;
}

/**
 * Returns the size (# of elements, not bytes!) of the current profiling setup
 * Returns 0 if not set up
 */
UINT32 ProfilingGetSize() {
	return (profilingBufferEnd - profilingBuffer);
}

/**
 * Returns the number of events recorded so far
 */
UINT32 ProfilingGetUsed() {
	return (profilingBufferPointer - profilingBuffer);
}

/**
 * Writes all the collected data (function calls and other registered profiling data)
 * to the serial port (RS232 UART)
 */
void ProfilerFlushCollection() {
	UINT32 size = ProfilingGetUsed() * sizeof(FUNC_PROFILE_ENTRY);
	if (size) {
		BOOL wasEnabled = ENABLE_PROFILING;

		if (wasEnabled) {
			ProfilingStop();
		}

		COM_PORT flushPort = COM2;
		InitUARTPort(&flushPort, 115200, UART8Bits, UARTOneStop, UARTNoParity);

		// dump all the data in the array
		UINT32 i;
		char* buffer = (char*)profilingBuffer;
		for(i = 0; i < size; i++) {
			UARTWriteByte(&flushPort, buffer[i]);
		}

		if (wasEnabled) {
					ProfilingStart();
		}
	}
}

/**
 * Creates a copy of the current profiling buffer for use in user mode
 * @return Pointer to user mode memory containing the buffer filled
 * with collected data so far
 */
FUNC_PROFILE_ENTRY* ProfilingCreateUserModeBuffer(PROCESS_PTR thread) {
	UINT32 size = ProfilingGetUsed() * sizeof(FUNC_PROFILE_ENTRY);
		if (size) {
			BOOL wasEnabled = ENABLE_PROFILING;

			if (wasEnabled) {
				ProfilingStop();
			}

			FUNC_PROFILE_ENTRY* array = UserMalloc(thread->userMemory, size, TRUE, PAGE_READ | PAGE_USER, MEMTYPE_DATA);
			memcpy(array, profilingBuffer, size);

			if (wasEnabled) {
						ProfilingStart();
			}

			return array;
		} else {
			return NULL;
		}
}

/**
 * Throws a kernel exception stating out of memory
 */
void ProfilingError() {
	KernelThrowException("Profiler: Out of memory!", EXC_OUT_OF_MEMORY, ProfilingGetUsed(), FALSE, NULL);
}

#endif
