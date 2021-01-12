/*
 * @file stackmem.c
 * @author Stefan Nürnberger [stefan@nurnware.de]
 */

#include <typedefs.h>
#include <memory/addresses.h>
#include <exceptions.h>
#include <video/video_char.h>

// 10 MB
#define MAX_STACK_MEM 0xA00000

static UINT8 flatStack[MAX_STACK_MEM];
static UINT8* flatStackPointer = &flatStack[0];


UINT32 StackMemGetUsage() {
	return (UINT32)flatStackPointer - (UINT32)&flatStack[0];
}

void StackMemAlign(UINT32 alignment) {
	UINT32 tempPointer = (UINT32)flatStackPointer;

	// if not already aligned, align it properly
	UINT32 offBy = tempPointer % alignment;
	if (offBy != 0) {
		tempPointer += alignment - offBy;
	}

	// check overflow
	if (((UINT32)tempPointer) >= (UINT32)flatStack + MAX_STACK_MEM) {
		KernelThrowException("Stack alignment: Out of pre-paged .bss-memory!", EXC_OUT_OF_MEMORY, alignment, FALSE, NULL);
	}
	flatStackPointer = (UINT8*)tempPointer;
}

/**
 * Returns a pointer to the BSS stack allocator (.bss)
 * @param size Size of the requested memory
 */
void* StackMemAlloc(UINT32 size) {
	if (((UINT32)flatStackPointer + size) >= (UINT32)flatStack + MAX_STACK_MEM) {
		#ifdef RAYDEBUG
		printf ("Requested %u bytes (%u of %u bytes in use)\n", size, StackMemGetUsage(), MAX_STACK_MEM);
		#endif
		KernelThrowException("Stack allocator: Out of pre-paged .bss-memory!", EXC_OUT_OF_MEMORY, size, FALSE, NULL);
	}
	void* address = (void*)flatStackPointer;
	flatStackPointer += size;
	return address;
}
