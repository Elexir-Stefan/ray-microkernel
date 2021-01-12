/**
 * @file features.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Mär 10 2007
 * @see features.c
 * @brief Names of x86 CPU-features
 */

#ifndef _features_H
#define _features_H

#include <typedefs.h>

#define MKBIT(n) (1<<n)


enum CPU_FEATURES {
	CPU_FEATURE_FPU = 0,	/**< Floating Point Unit */
	CPU_FEATURE_VME = 1,	/**< Virtual Mode Extension */
	CPU_FEATURE_DE = 2,		/**< Debugging Extension */
	CPU_FEATURE_PSE = 3,	/**< Page Size Extension */
	CPU_FEATURE_TSC = 4,	/**< Time Stamp Counter */
	CPU_FEATURE_MSR = 5,	/**< Model Specific Registers */
	CPU_FEATURE_PAE = 6,	/**< Physical Address Extesnion */
	CPU_FEATURE_MCE = 7,	/**< Machine Check Extension */
	CPU_FEATURE_CX8 = 8,	/**< CMPXCHG8 Instruction */
	CPU_FEATURE_APIC = 9,	/**< On-chip APIC Hardware */
	CPU_FEATURE_SEP = 11,	/**< SYSENTER SYSEXIT */
	CPU_FEATURE_MTRR = 12,	/**< Machine Type Range Registers */
	CPU_FEATURE_PGE = 13,	/**< Global Paging Extension */
	CPU_FEATURE_MCA = 14,	/**< Machine Check Architecture */
	CPU_FEATURE_CMOV = 15,	/**< Conditional Move Instrction */
	CPU_FEATURE_PAT = 16,	/**< Page Attribute Table */
	CPU_FEATURE_PSE36 = 17,	/**< 36-bit Page Size Extension */
	CPU_FEATURE_PSN = 18,	/**< 96-bit Processor Serial Number */
	CPU_FEATURE_CLFSH = 19,	/**< CLFLUSH Instruction */
	CPU_FEATURE_DS = 21,	/**< Debug Trace Store */
	CPU_FEATURE_ACPI = 22,	/**< ACPI Support */
	CPU_FEATURE_MMX = 23,	/**< MMX Technology */
	CPU_FEATURE_FXSR = 24,	/**< FXSAVE FXRSTOR (Fast save and restore) */
	CPU_FEATURE_SSE = 25,	/**< Streaming SIMD Extensions */
	CPU_FEATURE_SSE2 = 26,	/**< Streaming SIMD Extensions 2 */
	CPU_FEATURE_SS = 27,	/**< Self-Snoop */
	CPU_FEATURE_HTT = 28,	/**< Hyper-Threading Technology */
	CPU_FEATURE_TM = 29,	/**< Thermal Monitor Supported */
	CPU_FEATURE_IA64 = 30,	/**< IA-64 capable */
};

typedef struct {
	UINT32 maxCPUIDs;			/**< Maxiumum argument allowed for CPUID instruction */

	UINT8 family;				/**< 586, 686 etc... */
	UINT8 model;
	UINT8 stepping;

	UINT16 familyEX;
	UINT16 modelEX;

	UINT32 features;

	UINT32 vendorI[4];
	UINT32 descriptionI[13];
	char *vendor;
	char *description;
} PROCESSOR_INFO;

/**
 * Checks if all features RAY needs are supported by the current processor
 * @param showInfo if TRUE processor information will be printed on the screen
 */
void CheckProcessorFeatures(BOOL showInfo);


#endif
