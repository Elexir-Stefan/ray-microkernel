/**
 * @file abi.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sep 27, 2009
 * @brief Defines architecture specific functions for the Kernel ABI
 * (Application Binary Interface)
 */

#include <typedefs.h>
#include <mtask/task_types.h>

void ProcessWriteResult(PROCESS_PTR thread, UINT32 result) {
	thread->regs.eax = result;
}
