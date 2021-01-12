/*
 * cppsupport.h
 *
 *  Created on: 07.01.2010
 *      Author: stefan
 */

#ifndef CPPSUPPORT_H_
#define CPPSUPPORT_H_

#ifdef __cplusplus

extern  "C" {
#endif

// #include <reent.h>
// Linker generated variables
typedef void (*func_ptr) (void);
extern void* __EH_FRAME_BEGIN;
extern UINT32 __EH_FRAME_END;
extern func_ptr* __CTOR_LIST;
extern func_ptr* __DTOR_LIST;

void CPlusPlusInit();
void CPlusPlusPagedMemAvailable();
void CPPTest();

#ifdef __cplusplus
}
#endif

typedef void (*Constructor)();

#endif /* CPPSUPPORT_H_ */
