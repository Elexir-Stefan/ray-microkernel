/**
 * @file loadexecutable.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Mär 18 2008
 * @see loadexecutable.h
 * @brief Loads several sections into memory and sets up all the necessary things to run the code segment as a seperate process
 */

extern "C" {
#include <typedefs.h>
#include <memory/memory.h>
#include <memory/usermem.h>
#include <memory/mmu.h>
#include <memory/arguments.h>
#include <memory/addresses.h>
#include <memory/strings/strings.h>
#include <mtask/thread.h>
#include <mtask/scheduler.h>
#include <exceptions.h>
#include <debug.h>

#include <profiler/useprofile.h>

extern PROCESS_PTR currTask;
}
#include <modules/loadexecutable.h>
#include <modules/load_types.h>


using namespace Executables;

UINT32 Process::CreateAttributes(UINT16 memAttribs) {
	UINT32 attributes = PAGE_USER;

	// avoid nonsense
	ASSERT(memAttribs & MEM_ATTRIB_READABLE);

	if (memAttribs & MEM_ATTRIB_READABLE) {
		attributes |= PAGE_READ;
	}
	if (memAttribs & MEM_ATTRIB_WRITABLE) {
		attributes |= PAGE_WRITE;
	}

	return attributes;
}

MEMORY_KIND Process::GetKindByAttributes(UINT16 memAttribs)
{
	return memAttribs & MEM_ATTRIB_EXECUTABLE ? MEMTYPE_CODE : MEMTYPE_DATA;
}

void Process::CopyDataSectionsToProcess(StaticList<LOAD_MEM_SEGMENT>& readySegments, PROCESS_PTR child) {

	for (UINT32 i = 0; i < readySegments.Size; i++) {
		// check whether code segment is correct
		MEMORY_KIND kind = GetKindByAttributes(readySegments[i].memoryAttributes);

		if (kind == MEMTYPE_CODE) {
			if ((readySegments[i].loadAddress < USER_TEXT_SEG_START) || (readySegments[i].loadAddress + readySegments[i].segmentLength >= VIRTUAL_USER_DATA_START)) {
				KernelThrowException("CopyDataSectionsToProcess: module has wrong .text-address", 0xdead, 0,0,0);
			}

			if (!child->codeStart) {
				child->codeStart = (UINT32*)readySegments[i].loadAddress;
			} else {
				KernelThrowException("CopyDataSectionsToProcess: Two code segments are impossible!", 0xdead, readySegments[i].loadAddress, 0, 0);
			}

		} else {
			// must be data
			if ((readySegments[i].loadAddress < VIRTUAL_USER_DATA_START) || (readySegments[i].loadAddress + readySegments[i].segmentLength > VIRTUAL_USER_DATA_END)) {
				KernelThrowException("CopyDataSectionsToProcess: Data section not inside data segment!", 0xdead, 0, 0, 0);
			}
		}

		UINT32 attributes = CreateAttributes(readySegments[i].memoryAttributes);

		// move the thread's memory
		#ifdef RAY_KERNEL_DEBUGGER
		// must be writable for kernel debugger
		MemoryGiftTo(currTask, child, (UINT32)readySegments[i].segmentStart, readySegments[i].loadAddress, MEMTYPE_DATA, kind, attributes | PAGE_WRITE);
		#else
		// normal, protected way
		MemoryGiftTo(currTask, child, (UINT32)readySegments[i].segmentStart, readySegments[i].loadAddress, MEMTYPE_DATA, kind, attributes);
		#endif

	}

}

/**
 * Moves additional data to the process and appends their pointers to the argument list
 * NOTE: Already expects an allocated pointer to an ARGUMENTS struct (newArgs).
 * @param addMem Additional memory regions to move to the process and append to the argument list
 * @param numAllocs Number of LOAD_ADD_MEMs to handle
 * @param oldArgs old arguments to preserve (copy)
 * @param newArgs Already initialized ARGUMENTS struct to use to appending moved data
 * @param child Thread to move to
 * @return
 */
BOOL Process::MoveAddDataToProcess(StaticList<LOAD_ADD_MEM>* addMem, ARGUMENTS* oldArgs, ARGUMENTS* newArgs, PROCESS_PTR child) {
	UINT32 remoteAddress;
	UINT32 attributes;

	if (newArgs->count == 0) {
		// nothing to load
		newArgs->values = NULL;
		return FALSE;
	}

	// copy old argument list
	for (UINT32 i = 0; i < oldArgs->count; i++) {
		newArgs->values[i] = oldArgs->values[i];
	}

	if (addMem)
	{
		StaticList<LOAD_ADD_MEM>& memory = *addMem;
		for (UINT32 i = 0; i < memory.Size; i++)
		{
			attributes = CreateAttributes(memory[i].memoryAttributes);
			remoteAddress = MemoryGift(currTask, child, memory[i].allocPointer, MEMTYPE_DATA, MEMTYPE_DATA, attributes);
			ASSERT(remoteAddress != 0);
			newArgs->values[oldArgs->count + i] = remoteAddress;
		}
	}


	return TRUE;
}

/**
 * Creates a process based on the LOAD_PROCESS information providing loadable segment information
 * @param process segment, memory and process information
 * @return a new process
 */
PROCESS_PTR Process::LoadProcess(LoadProcessInfo& process) {
	// some security tests
	if ((process.processInfo.priority > currTask->priority) && (currTask->privLevel < PRIV_PRIORITY)) {
		KernelThrowException("LoadProcess: Tried to execute process with higher priory", EXC_ILLEGAL_STATE, process.processInfo.priority, 0, 0);
	}
	if (process.processInfo.privilegeLevel < currTask->privLevel) {
		KernelThrowException("LoadProcess: Tried to execute process with higher privilege level", EXC_ILLEGAL_STATE, process.processInfo.privilegeLevel, 0, 0);
	}


	PROCESS_PTR newThread = SpawnProcess();
	newThread->privLevel = process.processInfo.privilegeLevel;
	strncpy(newThread->ident, process.processInfo.name, 64);

	// copy all the requested data sections to the new process
	CopyDataSectionsToProcess(process.loadableSegments, newThread);

	// move all the additional memory allocations to the new process and pass them as
	// arguments after the existing arguments
	UINT32 addMemCount = process.additionalMemory ? process.additionalMemory->Size : 0;
	UINT32 tempArgumentValues[process.processInfo.arguments.count + addMemCount];
	ARGUMENTS newArgs;
	newArgs.count = process.processInfo.arguments.count + addMemCount;
	newArgs.values = tempArgumentValues;

	MoveAddDataToProcess(process.additionalMemory,  &process.processInfo.arguments, &newArgs, newThread);

	PrepareThread(newThread,(POINTER)process.processInfo.programEntryPoint , NULL, process.processInfo.priority, newArgs);
	SchedulerInsertNode(newThread);

	return newThread;
}
