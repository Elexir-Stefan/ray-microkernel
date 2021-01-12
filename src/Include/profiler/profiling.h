/**
 * @file profiling.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Jan 1 2008
 * @see profiling.c
 * @brief Summary
 */

#ifndef _profiling_H
#define _profiling_H

#include "memory/memory.h"
#include "profiler/measurement.h"

typedef struct {
	UINT64 timeStamp;
	UINT32 additionalInfo;
} PROFILE_DATA;

#define PROFILEVARNAME(v)		__profile__##v
#define PROFILESTACK(v)			__profile_z_##v
#define PROFILESIZENAME(v)		__profile_s_##v

#define PROFILEINIT(varname) _PROFILEINIT(, varname)
#define _PROFILEINIT(mod, varname)		mod PROFILE_DATA* PROFILEVARNAME(varname);\
					mod PROFILE_DATA* PROFILESTACK(varname);\
					mod UINT32 PROFILESIZENAME(varname)

#define PROFILESTART(varname, kbSize)	PROFILESIZENAME(varname) = kbSize * 1024;\
					PROFILEVARNAME(varname) = KMalloc(PROFILESIZENAME(varname), MEMTYPE_DATA);\
					PROFILESTACK(varname) = PROFILEVARNAME(varname)

#define PROFILEDATASIZE(varname)	((UINT32)(PROFILESTACK(varname) - PROFILEVARNAME(varname)))

#define PROFILEADDDATA(varname, time, addinfo)		if ((PROFILEDATASIZE(varname) + 1) * sizeof(PROFILE_DATA) < PROFILESIZENAME(varname)) {\
								PROFILESTACK(varname)->timeStamp = time;\
								PROFILESTACK(varname)->additionalInfo = addinfo;\
								PROFILESTACK(varname)++;\
							} else {\
								KernelThrowException("PROFILING " #varname " stopped. Memory full.", EXC_OUT_OF_MEMORY, 0, 0, 0);\
							}

#define EXTERNPROFILE(varname) _PROFILEINIT(extern, varname)

#define GETPROFILE_PROTO(varname) PROFILE_DATA* GetProfileDataOf##varname()

#define GETPROFILE(varname) EXTERNPROFILE(varname);\
UINT32 GetProfileSizeOf##varname() {\
	return PROFILEDATASIZE(varname); \
}\
PROFILE_DATA* GetProfileDataOf##varname() {\
	return PROFILEVARNAME(varname);\
}
#endif
