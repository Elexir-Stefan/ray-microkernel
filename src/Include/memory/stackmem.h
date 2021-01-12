/*
 * @file stackmem.h
 * @brief Provides memory management using just a (different) stack, not paging or similar
 */

#ifndef STACKMEM_H_
#define STACKMEM_H_

UINT32 StackMemGetPointer();
UINT32 StackMemGetUsage();
void* StackMemAlloc(UINT32 size);
void StackMemAlign(UINT32 aligment);


#endif /* STACKMEM_H_ */
