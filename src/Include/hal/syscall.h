#ifndef _SYSCALL_H
#define _SYSCALL_H

/**
 * @file syscall.h
 * @author Stefan Nuernberger
 * @date 09-25-2006
 * @brief Type definitions for the stack of the SysCall function
 */

#define NUM_SYSCALLS 512

#define ENABLE_FASTCALL


typedef struct {
	POINTER functionAddress;
	PRIVLEVEL minPrivlevel;
	BOOL doesReturnValue;
} CALLITEM;

/**
 * Initializes the syscall kernel system to register kernel functions for user-mode use
 */
void SysCallInit(void);


void RegisterSCTFunction(UINT32 kernelFuncNum, UINT8 minLevel, POINTER funcAddress, BOOL returnValue);

void DeleteSCTFunction(UINT32 kernelFuncNum);

POINTER GetSCTFunctionAddress(UINT32 kernelFuncNum);


UINT8 GetSCTminPrivilege(UINT32 kernelFuncNum);

void SealSCTFunctions();

#endif
