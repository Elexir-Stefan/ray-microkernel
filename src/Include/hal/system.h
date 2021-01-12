/**
 * @file system.h
 * @author Stefan Nuernberger
 * @date 2012-04-01
 * @brief System Management Functions (Reset, ... etc.)
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define RESET_METHOD_KBD	1
#define RESET_METHOD_IDT	2
#define RESET_METHOD_HLT	4

RAY_TDM SystemHardResetUser(PROCESS_PTR process, UINT32 method);
RAY_TDM SystemHardReset(UINT32 method);

BOOL SystemResetByKeyboard(void);
BOOL SystemResetByIDT(void);
BOOL SystemResetByHLT(void);

#endif /* SYSTEM_H_ */
