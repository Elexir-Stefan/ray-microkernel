/**
 * @file features.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Mär 10 2007
 * @see features.h
 * @brief Checks processor features before starting the kernel
 */

#include <typedefs.h>
#include <hal/features.h>
#include <exceptions.h>
#include <video/video_char.h>
#include <hal/timer.h>
#include <hal/timer_ctrl.h>
#include <hal/syscall.h>
#include <hal/timer.h>


#define CPUIDread(function,a,b,c,d) asm volatile ("cpuid" : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "0" (function))
#define CPUextended 0x80000000UL
#define CPUmodelName 0x80000002UL

#define TIMERDIFF(start, stop) ((stop) > (start) ? ((stop) - (start)) : (0xffff - (start) + (stop)))

PROCESSOR_INFO CPUinfo;
char wrongProcessor[] = {"Your Processor is too old.\nAt least a 686 (Pentium II, AMD K6) or newer is required tu run RAY!"};

const char *featureNames[] = {
	"Floating Point Unit",
	"Virtual Mode Extension",
	"Debugging Extension",
	"Page Size Extension",
	"Time Stamp Counter",
	"Model Specific Registers",
	"Physical Address Extesnion",
	"Machine Check Extension",
	"CMPXCHG8 Instruction",
	"On-chip APIC Hardware",
	"",
	"SYSENTER SYSEXIT",
	"Machine Type Range Registers",
	"Global Paging Extension",
	"Machine Check Architecture",
	"Conditional Move Instrction",
	"Page Attribute Table",
	"36-bit Page Size Extension",
	"96-bit Processor Serial Number",
	"CLFLUSH Instruction",
	"",
	"Debug Trace Store",
	"ACPI Support",
	"MMX Technology",
	"FXSAVE FXRSTOR (Fast save and restore)",
	"Streaming SIMD Extensions",
	"Streaming SIMD Extensions 2",
	"Self-Snoop",
	"Hyper-Threading Technology",
	"Thermal Monitor Supported",
	"AMD64",
	""
};

static BOOL is386();
static BOOL is486();
static UINT32 GetMaxCPUID();
static void GetProcessorInfo(PROCESSOR_INFO *CPUinfo);
void ShowFeatures(UINT32 has, UINT32 needs);

static BOOL is386() {
	UINT32 flags_before, flags_after;
	asm("pushfl; popl %%eax; movl %%eax, %0; xorl $0x40000, %%eax; pushl %%eax; popfl; pushfl; popl %%eax; movl %%eax, %1; pushl %0; popfl "
	: "=r" (flags_before),
	"=r" (flags_after):: "eax");


	flags_before &= 0x40000;
	flags_after  &= 0x40000;

	if(flags_before == flags_after) {
		/* it's a 386 ! */
		return TRUE;
	} else {
		return FALSE;
	}
}

static BOOL is486() {
	UINT32 flags_before, flags_after;

	asm("pushfl; popl %%eax; movl %%eax, %0; xorl $0x200000, %%eax; pushl %%eax; popfl; pushfl; popl %%eax; movl %%eax, %1; pushl %0; popfl "
	    : "=r" (flags_before),
	    "=r" (flags_after): : "eax");

	flags_before &= 0x200000;
	flags_after  &= 0x200000;

	if(flags_before == flags_after) {
	/* We have found an old 486 */
		return TRUE;
	} else {
		return FALSE;
	}

}

static UINT32 GetMaxCPUID() {
	UINT32 eax, ebx, ecx, edx;
	CPUIDread(0, eax, ebx, ecx, edx);
	return eax;
}

static void GetProcessorInfo(PROCESSOR_INFO *CPUinfo) {
	UINT32 eax, ebx, ecx, edx;
	CPUIDread(1, eax, ebx, ecx, edx);

	CPUinfo->family = (eax >> 8) & 0x0f;
	CPUinfo->model = (eax >> 4) & 0x0f;
	CPUinfo->stepping = eax & 0x0f;

	CPUinfo->familyEX= (eax >> 20) & 0x0ff;
	CPUinfo->modelEX = (eax >> 16) & 0x0f;

	CPUinfo->features = edx;

	// fill in vendor name
	CPUIDread(0, eax, CPUinfo->vendorI[0], CPUinfo->vendorI[2], CPUinfo->vendorI[1]);
	CPUinfo->vendorI[3] = 0;

	CPUIDread(CPUmodelName, CPUinfo->descriptionI[0], CPUinfo->descriptionI[1], CPUinfo->descriptionI[2], CPUinfo->descriptionI[3]);
	CPUIDread(CPUmodelName+1, CPUinfo->descriptionI[4], CPUinfo->descriptionI[5], CPUinfo->descriptionI[6], CPUinfo->descriptionI[7]);
	CPUIDread(CPUmodelName+2, CPUinfo->descriptionI[8], CPUinfo->descriptionI[9], CPUinfo->descriptionI[10], CPUinfo->descriptionI[11]);

	CPUinfo->vendor = (char*)&CPUinfo->vendorI;
	CPUinfo->description= (char*)&CPUinfo->descriptionI;

}

void ShowFeatures(UINT32 has, UINT32 needs) {
	UINT32 i;

	for (i = 0; i < 32; i++) {
		if ((needs & MKBIT(i)) && (!(has & MKBIT(i)))) {
				printf ("Your processor lacks %s!\n", featureNames[i]);
		}

	}
}


void CheckProcessorFeatures(BOOL showInfo) {
	UINT32 neededFeatures = MKBIT(CPU_FEATURE_MSR) | MKBIT(CPU_FEATURE_TSC) | MKBIT(CPU_FEATURE_CMOV) ;
#ifdef ENABLE_SYSENTER
	neededFeatures |= MKBIT(CPU_FEATURE_SEP);
#endif



	if (is386()) {
		KernelThrowException(wrongProcessor,EXC_FEATURE_PROCESSOR,0,0,0);
	} else if (is486()) {
		KernelThrowException(wrongProcessor,EXC_FEATURE_PROCESSOR,0,0,0);
	} else {

		/* Now it's save to call the CPUID instruction */
		CPUinfo.maxCPUIDs = GetMaxCPUID();

		if (CPUinfo.maxCPUIDs >= 1) {
			/* get detailed information about the processor */
			GetProcessorInfo(&CPUinfo);
			/* it's 686, check for features needed */
			if (showInfo) {
				printf ("Processor: %s %s\nFamily %u Model %u Stepping %u.\n", CPUinfo.vendor, CPUinfo.description, CPUinfo.family, CPUinfo.model, CPUinfo.stepping);
				ShowFeatures(CPUinfo.features, neededFeatures);
			}

			if ((CPUinfo.features & neededFeatures) != neededFeatures) {
				ShowFeatures(CPUinfo.features, neededFeatures);
				KernelThrowException(wrongProcessor,EXC_FEATURE_PROCESSOR,0,0,0);
			}
		} else {
			KernelThrowException("Your processor does not support CPUID instructions needed to guarantee successful operation of the operating system", EXC_FEATURE_PROCESSOR, CPUinfo.maxCPUIDs, 0, 0);
		}

	}

}
