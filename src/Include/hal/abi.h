/**
 * @file abi.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sep 27, 2009
 * @brief ...
 */

#include <mtask/task_types.h>


/**
 * Writes an UINT32 result to the process' EAX register (the result
 * register) so that it will be overwritten the next time the process
 * will be scheduled
 */
void ProcessWriteResult(PROCESS_PTR thread, UINT32 result);
