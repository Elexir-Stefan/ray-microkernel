/**
 * @file loadmodule.c
 * @author Stefan Nuernberger
 * @date Do Sep 14 2006
 * @brief Functions to start a module loaded by grub as a thread.
 */

extern "C" {
	#include <typedefs.h>
	#include <multiboot.h>
	#include <memory/memory.h>
	#include <memory/usermem.h>
	#include <memory/mmu.h>
	#include <memory/arguments.h>
	#include <exceptions.h>
	#include <mtask/task_types.h>
	#include <mtask/thread.h>
	#include <mtask/scheduler.h>
	#include <memory/strings/strings.h>
	#include <debug.h>

	extern PROCESS_PTR currTask;
}
#include <modules/loadmodule.h>
#include <modules/elf32.h>
#include <modules/loadexecutable.h>

using namespace Executables;

extern "C" void StartModules(ModuleLoadInfo module, UINT32 moduleCount) {
	Modules::Start(module, moduleCount);
}

void Modules::ExtractFileName(char *fileName, char *source) {
	SINT32 lastSlash;
	UINT32 start, stop, len;
	UINT32 i;

	len = strlen(source);

	lastSlash = -1;
	for (i = 0; i < len; i++) {
		if (source[i] == '/') {
			lastSlash = i;
		}

	}

	stop = len;
	if (lastSlash == -1) {
		start = 0;
	} else {
		/* slash found */
		start = lastSlash + 1;
	}

	/* strip arguments */
	for (i = start; i < len; i++) {
		if (source[i] == ' ') {
			stop = i;
			break;
		}
	}

	memcpy(fileName, source + start, stop-start);
	fileName[stop-start] = 0;
}

/**
 * Takes each module loaded by grub and starts it as a thread
 * @param module
 * @param moduleCount
 */
void Modules::Start(ModuleLoadInfo module, UINT32 moduleCount) {

	for (UINT32 i = 0; i < moduleCount; i++, module++) {
		// copy all necessary sections to the right places
		module->virtualMemChannel = AllocatePhysicalMemory(kernelThread->userMemory, module->unpagedMemStart, module->moduleLength, PAGE_SUPERV | PAGE_READ);
		UINT8* tempBufferStart = (UINT8*)module->virtualMemChannel;

		Elf32 elfFile(tempBufferStart, module->moduleLength);
		LoadProcessInfo newProcess = elfFile.CreateLoadInfo();

		ExtractFileName(newProcess.processInfo.name, module->moduleCmdLine);

		// Add path as argument
		UINT32 pathLength = strlen(module->moduleCmdLine) + 1; // including 0 termination

		newProcess.additionalMemory = new StaticList<LOAD_ADD_MEM>(1);

		// copy actual payload to it
		String argumentString = new char[pathLength];
		memcpy(argumentString, module->moduleCmdLine, pathLength);

		StaticList<LOAD_ADD_MEM>& addMem = *newProcess.additionalMemory;
		addMem[0].allocPointer = (UINT32)argumentString;
		addMem[0].memoryAttributes = MEM_ATTRIB_READABLE;

		Process::LoadProcess(newProcess);


	}
}
