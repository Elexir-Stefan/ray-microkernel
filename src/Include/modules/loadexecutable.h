/**
 * @file loadexecutable.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Mär 18 2008
 * @see loadexecutable.c
 * @brief Summary
 */

#ifndef _loadexecutable_H
#define _loadexecutable_H

#include <mtask/task_types.h>
#include <modules/load_types.h>

namespace Executables
{


class Process
{
public:
	static PROCESS_PTR LoadProcess(LoadProcessInfo& process);
	static UINT32 CreateAttributes(UINT16 memAttribs);
	static MEMORY_KIND GetKindByAttributes(UINT16 memAttribs);

private:
	static void CopyDataSectionsToProcess(StaticList<LOAD_MEM_SEGMENT>& readySegments, PROCESS_PTR child);
	static BOOL MoveAddDataToProcess(StaticList<LOAD_ADD_MEM>* addMem, ARGUMENTS* oldArgs, ARGUMENTS* newArgs, PROCESS_PTR child);
};

}

#endif
