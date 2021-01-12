/**
 * @file processnames.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Fr Aug 1 2008
 * @see processnames.c
 * @brief Summary
 */

#ifndef _processnames_H
#define _processnames_H

#include <mtask/pid_types.h>
#include <mtask/task_types.h>

PID GetPIDByDescriptiveName(CString name);
String GetNameByPID(PID pid);
String GetDescriptionByPID(PID pid);
BOOL RegisterDescriptiveName(PID pid, CString name, CString description);

#endif
