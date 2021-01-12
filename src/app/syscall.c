/**
 * @file syscall.c
 * @author Stefan Nuernberger
 * @date Mo Sep 25 2006
 * @brief Kernel System Call Interface - To allow user-mode tasks to call kernel-mode functions
 */

#include <typedefs.h>
#include <memory/memory.h>
#include <hal/syscall.h>
#include <hal/tss.h>
#include <hal/processor.h>
#include <hal/msr.h>
#include <hal/gdt.h>
#include <hal/abi.h>
#include <mtask/task_types.h>
#include <mtask/scheduler.h>
#include <exceptions.h>
#include <hal/timer.h>
#include <debug.h>

#ifdef RAYPROFILING
#include <profiler/profiling.h>
#endif

extern PROCESS_PTR currTask;
extern PROCESS_PTR kernelThread;
extern TSS *kernelTSS;
/**
 * The Cold iGnition System Gate
 */
extern void SysCallEntry(void);

static CALLITEM* sysFunctionList;

#ifdef RAYPROFILING
PROFILEINIT(sysCallTime);
#endif

void SysCallInit(void) {
	/* allocate space for system call table (SCT) */
	sysFunctionList = (CALLITEM*)KMalloc(NUM_SYSCALLS * sizeof(CALLITEM), MEMTYPE_DATA);

	/* init the values: highest level needed, no functions registered */
	memset(sysFunctionList, 0, NUM_SYSCALLS * sizeof(CALLITEM));

#ifdef ENABLE_FASTCALL
	/* enable SYSENTER functions */
	WriteMSR(MSR_SYSENTER_CS, KERNEL_SEG_CODE, 0);
	WriteMSR(MSR_SYSENTER_ESP, kernelTSS->esp_ring0, 0);
	WriteMSR(MSR_SYSENTER_EIP, (UINT32)SysCallEntry, 0);
#endif


#ifdef RAYPROFILING
	PROFILESTART(sysCallTime, 2000);
#endif
}


/**
 * Makes the SCT write protected
 */
void SealSCTFunctions() {
	ProtectKernelStructure(sysFunctionList);
}

/**
 * Registeres a function in the system call table for the specified number
 * that can then be called from user-mode
 * @param kernelFuncNum a unique number for that function
 * @param minLevel minimum required privilege-level (of user-mode app) to be allowed to call that function
 * @param funcAddress pointer to the function
 * @param returnValue Specifies whether this function returns a value (UINT32) or not (void).
 */
void RegisterSCTFunction(UINT32 kernelFuncNum, UINT8 minLevel, POINTER funcAddress, BOOL returnValue){
	if (kernelFuncNum < NUM_SYSCALLS) {
		ASSERT(sysFunctionList[kernelFuncNum].functionAddress == NULL);
		sysFunctionList[kernelFuncNum].functionAddress = funcAddress;
		sysFunctionList[kernelFuncNum].minPrivlevel = minLevel;
		sysFunctionList[kernelFuncNum].doesReturnValue = returnValue;
	}
}

/**
 * Deletes an antry in the system call table
 * @param kernelFuncNum Number to delete
 */
void DeleteSCTFunction(UINT32 kernelFuncNum) {
	if (kernelFuncNum < NUM_SYSCALLS) {
		memset(sysFunctionList + kernelFuncNum, 0, sizeof(CALLITEM));
	}
}

/**
 * Returns the registered function at the specified address
 * @param kernelFuncNum Index to the system call table
 * @return function address, 0 if nothing registered
 */
POINTER GetSCTFunctionAddress(UINT32 kernelFuncNum) {
	if (kernelFuncNum < NUM_SYSCALLS) {
		return sysFunctionList[kernelFuncNum].functionAddress;
	} else {
		return NULL;
	}

}

/**
 * returns the minimum privilege level needed to call the function (if registered)
 * @param kernelFuncNum Index to the SCT
 * @return privilege level, 0 if nothing registered
 */
UINT8 GetSCTminPrivilege(UINT32 kernelFuncNum) {
	if (kernelFuncNum < NUM_SYSCALLS) {
		return sysFunctionList[kernelFuncNum].minPrivlevel;
	} else {
		return 0;
	}
}


void SysCall(UINT32 functionNumber, UINT32* argumentPointer) {

	PROCESS_PTR caller = currTask;

	// Start privileged time cycle counter
	currTask->privilegedTimeStart = ReadTSC();

	if (LIKELY(functionNumber < NUM_SYSCALLS)) {
		CALLITEM* function = &sysFunctionList[functionNumber];
		if (LIKELY(function->functionAddress)) {
			if (LIKELY(function->minPrivlevel >= currTask->privLevel)) {

				#ifdef RAYPROFILING
				UINT64 profStart, profStop;
				profStart = ReadTSC();
				#endif

				UINT32 (*Callee)(UINT32* argumentPointer) = function->functionAddress;


				if (function->doesReturnValue) {
					// Function returns a value. Write that back.
					ProcessWriteResult(caller, Callee(argumentPointer));
				} else {
					// Does not return a value
					Callee(argumentPointer);
				}

				#ifdef RAYPROFILING
				profStop = ReadTSC();
				UINT32 timeTook = profStop - profStart;
				PROFILEADDDATA(sysCallTime, timeTook, (UINT32)Callee);
				#endif

			} else {
				KernelThrowException("SysCall: Insufficient Rights!", EXC_INSUFFICIENT_RIGHTS, (function->minPrivlevel<<8) + currTask->privLevel, 0, 0);
			}
		} else {
			KernelThrowException("SysCall: Unimplemented!", EXC_UNKNOWN_SYSCALL, functionNumber, 0, 0);
		}

	} else {
		KernelThrowException("SysCall: Unknown function invoked\n",EXC_UNKNOWN_SYSCALL, functionNumber, 0, 0);
		/** TODO Kill task */
		//SetLastError( ... );
		// KillTask (... );
	}

	// Update privileged time cycle counter
	currTask->privilegedCycles += ReadTSC() - currTask->privilegedTimeStart;


}
