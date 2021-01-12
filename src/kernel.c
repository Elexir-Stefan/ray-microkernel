/**
 * @file kernel.c
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Main kernel file
 * Procedures for setting up the kernel.
 * Calls functions that initialize the GDT, IDT, IRQ handlers and starts multitasking
 */
#include <typedefs.h>
#include <multiboot.h>
#include <video/video_char.h>
#include <video/video_mode.h>
#include <memory/memory.h>
#include <memory/addresses.h>
#include <memory/mmu.h>
#include <memory/strings/strings.h>
#include <hal/gdt.h>
#include <hal/idt.h>
#include <hal/int.h>
#include <hal/processor.h>
#include <hal/features.h>
#include <kernel.h>
#include <hal/timer.h>
#include <app/rmi.h>
#include <app/callbacks.h>
#include <app/ioperm.h>
#include <mtask/scheduler.h>
#include <mtask/thread.h>
#include <exceptions.h>
#include <appearance/appearance.h>
#include <cmdline.h>
#include <kernel_private.h>
#include <hal/syscall.h>
#include <hal/irq_han.h>
#include <app/usermodefuncs.h>
#include <modules/loadmodule.h>
#include <hal/atom.h>
#include <kdebug/symboltable.h>

#include <cpp/cppsupport.h>

#include <globalconfig.h>

#ifdef RAYDEBUG
#include <test/unittest.h>
#endif

#ifdef RAY_KERNEL_DEBUGGER
#include <kdebug/kdebug.h>
#include <kdebug/i386-stub.h>
#endif


extern PROCESS_PTR currTask;
extern PROCESS_PTR kernelThread;
extern BOOL globalDoubleFault;

static NORETURN void HaltCPU() {
	for(;;) {
		__asm__ ("hlt");
	}
}

#ifdef RAYDEBUG
DONTPROFILE static void UnwindStack() {
	UINT32 caller_return;
	UINT32 *ebp;
	UINT32 physicalCodeAddress, physicalStackAddress;
	CString functionName;
	UINT32 functionAddress;

	// get EBP
	__asm__("mov %%ebp, %0": "=r"(ebp));

	printf("\nKernel trace:\n");

	UINT32 count = 0;
	while(ebp) {
		// get EIP value, one word after EBP
		physicalCodeAddress = GetPhysicalAddressOf(currTask->userMemory->pageDirVirtAddress, (ebp + 1));
		if (physicalCodeAddress) {
			caller_return = *(ebp + 1);
			// EBP pointer points to caller's EBP

			physicalStackAddress = GetPhysicalAddressOf(currTask->userMemory->pageDirVirtAddress, ebp);
			if (physicalStackAddress) {
				ebp = (UINT32*)*ebp;
					functionName = GetSymbolByAddress(caller_return, &functionAddress);
					printf (" %u: %x(%x) %s\n", count++, caller_return, (UINT32)ebp, functionName);
			} else {
				break;
			}
		} else {
			break;
		}
	}
}
#endif

DONTPROFILE static void PrintStackTrace(UINT32 address) {
	UINT32 caller_return;
	UINT32 count;
	UINT32 *ebp = (UINT32*)address;

	UINT32 physicalCodeAddress, physicalStackAddress;

	printf ("Stack trace:\n");
	//while (((UINT32)ebp < STATIC_MALLOC_START) && ((UINT32)ebp > 0x100000)) {
	for (count = 0; count < 6; count++) {
		// get EIP value, one word after EBP
		physicalCodeAddress = GetPhysicalAddressOf(currTask->userMemory->pageDirVirtAddress, (ebp + 1)) & PAGE_NO_OFFSET;
		if (physicalCodeAddress) {
			caller_return = *(ebp + 1);
			// EBP pointer points to caller's EBP

			physicalStackAddress = GetPhysicalAddressOf(currTask->userMemory->pageDirVirtAddress, ebp) & PAGE_NO_OFFSET;
			if (physicalStackAddress) {
				ebp = (UINT32*)*ebp;
				printf ("%u: %x [PHYS: %x] - Frame @ %x [PHYS: %x]\n", count, caller_return, physicalCodeAddress, (UINT32)ebp, physicalStackAddress);
			} else {
				printf ("%u: %x [PHYS: %x] - Frame not accessible!\n", count, caller_return, physicalCodeAddress);
			}
		} else {
			break;
		}
	}
}


DONTPROFILE NORETURN void KernelThrowException(const char *message, UINT32 errNumber, UINT32 addInfo, UINT8 showFlags, INT_REG *regs) {
	KernelInterruptsDisable();
	const char *childThreadName;
	UINT32 lastAccess;
	if (globalDoubleFault) {
		for(;;);
	}
	globalDoubleFault = TRUE;

#ifndef RAYDEBUG
	/* display information on a dangerous looking red-screen */
	VideoSetBCol(4);
	VideoClear();
	VideoSetBCol(6);
	VideoSetFCol(15);
	VideoFillLine();
	printf ("NURNware RAY Kernel mode exception encountered\n");
	VideoSetBCol(4);
#endif
	printf("\n");

	if (currTask->isParent) {
		childThreadName = "main";
	} else {
		childThreadName = currTask->ident;
	}

	printf ("KERNEL CRASH.");
	printf ("ERROR %x occurred in process \n%s.%s (PID %u, thread %u).\nAdditional info: %x\n",
	        errNumber, currTask->threadParent->ident, childThreadName, currTask->pid, currTask->tid, addInfo);
	printf ("Error: %s\n\n",
			message);

	lastAccess = ProcessorReadCR2();
	if (lastAccess) {
		printf ("Last Access on %x - PageDir=%x\n",
				lastAccess,
				ProcessorReadCR3());
		/*
		UINT32 physAddr = GetPhysicalAddressOf(currTask->userMemory->pageDirVirtAddress, (UINT32*)lastAccess);
		if (physAddr == 0) {
			printf ("Page is not mapped!\n");
		} else {
			printf ("Phys. page: %x (", physAddr & PAGE_NO_OFFSET);
			if (physAddr & PAGE_PRESENT) printf ("R");
			if (physAddr & PAGE_WRITE) printf ("W");
			if (physAddr & PAGE_USER) {
				printf ("U)\n");
			} else {
				printf ("K)\n");
			}

		}*/
	}

	if (showFlags & EXC_FLAG_SHOWREGS) {

		printf ("EIP=%x \t  CS=%x \t SS=%x \t FLG=%x\n",
		        regs->eip,
		        regs->cs,
		        regs->ss,
		        regs->eflags);
		printf ("EAX=%x \t EBX=%x \t ECX=%x \t EDX=%x\n",
			regs->eax,
			regs->ebx,
			regs->ecx,
			regs->edx);
		printf ("EBP=%x \t ESI=%x \t EDI=%x \t ESP=%x\n",
			regs->ebp,
			regs->esi,
			regs->edi,
			regs->esp);
		printf (" DS=%x \t  ES=%x \t  FS=%x \t  GS=%x\n",
				regs->ds,
				regs->es,
				regs->fs,
				regs->gs);
		printf ("stack=%x\tInterrupt #%u [%s,%s,%s,%s]\t",
		        regs->userESP,
		        regs->intNum,
		        (regs->intNum & 1) ? "ACC_VIOL" : "UNPAGED",
				(regs->intNum & 2) ? "WRITE" : "READ",
				(regs->intNum & 4) ? "USER" : "KERNEL",
				(regs->intNum & 8) ? "INSTR_FETCH" : "DATA"
				);


#ifdef RAY_KERNEL_DEBUGGER
	BREAKPOINT();
#endif
	}

#ifdef RAYDEBUG
	UnwindStack();
#else
	printf ("System halted.\n");
#endif
	HaltCPU();
}


static void KernelIdleLoop(void) {
	for(;;) {
		// do nothing
		asm("\t sti \n"
			"\t hlt \n");
#ifdef RAYDEBUG
		PeriodicalTests();
#endif
	}
}


static void CheckFlags(multiboot_info_t *mbi) {
	/* Are mem_* valid? */
	if (!CHECK_FLAG(mbi->flags, 0)) {
		KernelThrowException("MULTIBOOT: No Memory flag found!",EXC_INIT_NO_MEMFLAG, 0, 0, 0);
	}

	/* we don't need boot device flags
	if (!CHECK_FLAG(mbi->flags, 1)) {
		KernelThrowException ("MULTIBOOT: No boot-device flag!",EXC_INIT_NO_BOOTDEV, mbi->flags, 0, 0);
	}
	*/

	/* Is the command line passed? */
	if (!CHECK_FLAG(mbi->flags, 2)) {
		KernelThrowException ("MULTIBOOT: No parameter flag!",EXC_INIT_NO_PARAMFL, mbi->flags, 0, 0);
	}


	/* Is the section header table of ELF valid? */
	if (!CHECK_FLAG (mbi->flags, 5)) {
		KernelThrowException ("MULTIBOOT: No valid ELF file!",EXC_INIT_NOT_ELF, mbi->flags, 0, 0);
	}

	/* Are mmap_* valid? */
	if (!CHECK_FLAG (mbi->flags, 6)) {
		KernelThrowException ("MULTIBOOT: No valid mmap!",EXC_INIT_NO_MMAP, mbi->flags, 0, 0);
	}

}

static void SetupDisplay() {
	// disable write-back for display
	UINT32* virtDisplayBuffer = AllocatePhysicalMemory(kernelThread->userMemory, GetFBAddress(), 80 * 50 * 2, PAGE_SUPERV | PAGE_WRITE | PAGE_PWT);
	VideoInit((UINT32)virtDisplayBuffer);
	VideoClear();
}

static void KernelStartup(multiboot_info_t *mbi) {
	UINT32 totalMemory = mbi->mem_upper << 10;

	// set up all the ISRs for error handling
	KernelIDTRegister();

	// initialize the memory management and get rid of the trick-GDT
	BootInfo info = KernelInitMMU(mbi);



	CPlusPlusPagedMemAvailable();
	CPlusPlusInit();

	// re-initialize the display (physical access is no longer possible due to paging)
	SetupDisplay();
	KernelSetTextLayout(totalMemory);

	// install real GDT
	KernelInitGDT();

	KernelParseCommandLine(info->commandLine);
	KernelDoCommandLine();

#ifdef RAY_KERNEL_DEBUGGER
	KernelDebuggerInit();
#endif

	SchedulerInit();

	BarrierInit();

	IRQHandlerInit();
	SysCallInit();
	SCTRegisterKernelFunctions();
	KernelInitRMI();

	InitIOPermissions();

	InitThreading();

	StartModules(info->modules, info->moduleCount);

#ifdef RAYDEBUG
	printf ("Starting usermode-programs...\n");
#endif
}


void KernelInit (multiboot_info_t *mbi) {

	//AtomicSectionStart();
	KernelInterruptsDisable();

		VideoInit(GetFBAddress() + VIRTUAL_KERNEL_SPACE_START);
		VideoClear();

		// Initialize C++ support
		CPlusPlusInit();

		// if we get here, everything was ok
		printf ("Starting boot process... Please wait...\n");

		CheckFlags(mbi);
		CheckProcessorFeatures(FALSE);

		KernelStartup(mbi);

		// Spwan idle threads for each CPU and mark
		// kernel thread as non-executable

		ARGUMENTS args;
		args.count = 0;
		args.values = NULL;
		PROCESS_PTR idleThread = CreateThreadFromExisting(kernelThread, &KernelIdleLoop, NULL, PRIO_IDLE, args, "Idle Thread");
		SchedulerInsertNode(idleThread);
		SchedulerOverrideThreadState(kernelThread, T_IDLE);



	/* AtomicSectionEnd(); */

	// set the timer frequency for int0
	KernelTimerSetup(0, SCHEDULER_TIMESLICE);
	KernelInterruptsEnable();

	// Wait, until we are switched away...
	asm volatile ("sti; hlt;");


	KernelThrowException("Reached unreachable point!", 0xFFFFFF, 0, FALSE, NULL);


}
