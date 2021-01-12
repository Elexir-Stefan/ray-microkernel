#ifndef _PROCESSOR_H
#define _PROCESSOR_H

/**
 * @file processor.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief direct processor writing reading
 */


static inline void WriteMSR(UINT32 msr, UINT32 val1, UINT32 val2){
	__asm__ __volatile__("wrmsr" \
	: /* no outputs */ \
	                     : "c" (msr), "a" (val1), "d" (val2));
}

/**
 * writes a value to the cr0 register
 * \note the values in cr0 will be overwritten!
 * @param value value to write
 */
static inline void ProcessorWriteCR0(UINT32 value){
	__asm__ __volatile__ ("movl %0, %%cr0;"::"r"(value));
}


/**
 * reads the cr0 register
 * @return cr0 register
 */
static inline UINT32 ProcessorReadCR0() {
	UINT32 result;
	__asm__ __volatile__ ("movl %%cr0, %0":"=r"(result));
	return (result);
}

/**
 * writes a value to the cr1 register
 * \note the values in cr1 will be overwritten!
 * @param value value to write
 */
static inline void ProcessorWriteCR1(UINT32 value){
	__asm__ __volatile__ ("movl %0, %%cr1;"::"r"(value));
}

/**
 * reads the cr1 register
 * @return cr1 register
 */
static inline UINT32 ProcessorReadCR1() {
	UINT32 result;
	__asm__ __volatile__ ("movl %%cr1, %0":"=r"(result));
	return (result);
}

/**
 * writes a value to the cr2 register
 * \note the values in cr2 will be overwritten!
 * @param value value to write
 */
static inline void ProcessorWriteCR2(UINT32 value){
	__asm__ __volatile__ ("movl %0, %%cr2;"::"r"(value));
}


/**
 * reads the cr2 register
 * @return cr2 register
 */
static inline UINT32 ProcessorReadCR2() {
	UINT32 result;
	__asm__ __volatile__ ("movl %%cr2, %0":"=r"(result));
	return (result);
}

/**
 * writes a value to the cr3 register
 * \note the values in cr3 will be overwritten!
 * @param value value to write
 */
static inline void ProcessorWriteCR3(UINT32 value){
	__asm__ __volatile__ ("movl %0, %%cr3;"::"r"(value));
}


/**
 * reads the cr3 register
 * @return cr3 register
 */
static inline UINT32 ProcessorReadCR3() {
	UINT32 result;
	__asm__ __volatile__ ("movl %%cr3, %0":"=r"(result));
	return (result);
}

/**
 * writes a value to the cr4 register
 * \note the values in cr4 will be overwritten!
 * @param value value to write
 */
static inline void ProcessorWriteCR4(UINT32 value){
	__asm__ __volatile__ ("movl %0, %%cr4;"::"r"(value));
}


/**
 * reads the cr4 register
 * @return cr4 register
 */
static inline UINT32 ProcessorReadCR4() {
	UINT32 result;
	__asm__ __volatile__ ("movl %%cr4, %0":"=r"(result));
	return (result);
}

/**
 * invalidates a single entry in the TLB
 * @param address the virtual address that is no longer valid
 */
MEMPROFILE static inline void UpdateTLB(UINT32 *address) {
	__asm__ __volatile__ ("invlpg %0"::"m"(*(char*)address): "memory");
}


#endif
