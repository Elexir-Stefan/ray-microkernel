/**
 * @file load_types.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Mär 18 2008
 * @see load_types.c
 * @brief Different types used to load segments into memory
 */

#ifndef _load_types_H
#define _load_types_H

extern "C" {
#include <mtask/task_types.h>
#include <memory/arguments.h>
}
#include <cpp/staticlist.h>
#include <map>

#define MEM_ATTRIB_READABLE		0x01
#define MEM_ATTRIB_WRITABLE		0x02
#define MEM_ATTRIB_EXECUTABLE	0x04
#define MEM_ATTRIB_INJECTED		0x08
#define MEM_ATTRIB_MONITOR		0x10

struct LOAD_MEM_SEGMENT {
	POINTER segmentStart;
	UINT8* payloadStart;
	UINT32 segmentLength;
	UINT32 loadAddress;
	UINT16 memoryAttributes;

}__attribute__((packed)) ;

struct LOAD_ADD_MEM {
	UINT32 allocPointer;
	UINT16 memoryAttributes;
} __attribute__((packed));

struct ProgramStartParameters {
	char name[64];
	PRIORITY priority;
	PRIVLEVEL privilegeLevel;
	UINT32 programEntryPoint;
	ARGUMENTS arguments;
};


struct LoadProcessInfo {
	ProgramStartParameters processInfo;

	StaticList<LOAD_MEM_SEGMENT>& loadableSegments;

	StaticList<LOAD_ADD_MEM>* additionalMemory;

	LoadProcessInfo(StaticList<LOAD_MEM_SEGMENT>& loadableSegments,
	StaticList<LOAD_ADD_MEM>* additionalMemory) :
		loadableSegments(loadableSegments),
		additionalMemory(additionalMemory)
	{

	}

	~LoadProcessInfo()
	{
		if (additionalMemory)
		{
			delete additionalMemory;
		}
	}

};


#endif
