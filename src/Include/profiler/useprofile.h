/**
 * @file useprofile.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Feb 11, 2009
 * @brief ...
 */

#include <profiler/profiling.h>

typedef enum {
	PROF_FLG_FRAMEENTER = 0,
	PROF_FLG_FRAMEDROP = 1,
	PROF_FLG_MALLOC = 2,
	PROF_FLG_FREE = 3
} __attribute__((packed)) SAMPLEFLAGS;


typedef struct {
	SAMPLEFLAGS flags;
	POINTER calleeAddress;
	POINTER callerAddress;
	UINT64 timeStamp;
} __attribute__((packed)) FUNC_PROFILE_ENTRY;

// Automatic inclusion of other profiling variables
GETPROFILE_PROTO(sysCallTime);


void ProfilingEnable(UINT32 bufferEntries);
void ProfilingStart();
void ProfilingReset();
void ProfilingStop();
UINT32 ProfilingGetSize();
UINT32 ProfilingGetUsed();
FUNC_PROFILE_ENTRY* ProfilingCreateUserModeBuffer(PROCESS_PTR thread);
void ProfilerFlushCollection();
void ProfilingError();
