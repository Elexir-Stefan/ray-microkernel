# NURNware Ray Kernel
# (For use with Cold iGnition operating system)
# Copyright (C) 2007 - NURNware Technologies


CC=clang
C_ARGS=-DRAY_VERSION_BUILD=1407 -m32 -DRAYDEBUG -DDBG_SECTIONS=0 -gdwarf-2 -ggdb -I./Include -O0 -m32 -march=i586 -Wall -ffreestanding -Wwrite-strings -Wunused -Wextra -Werror -fno-stack-protector -pipe
CPPC=clang++
CPP_ARGS=-DRAY_VERSION_BUILD=1407 -m32 -DRAYDEBUG -DDBG_SECTIONS=0 -gdwarf-2 -ggdb -I./Include -O0 -m32 -march=i586 -Wall -ffreestanding -Wwrite-strings -Wunused -Wextra -Werror -fno-stack-protector -pipe
LD=i686-pc-ray-g++


all: ray


clean:
	@-rm `find . -iname '*.o'`


rebuild: clean ray


kernel.o: kernel.c Include/typedefs.h Include/compilerspec.h \
  Include/multiboot.h Include/video/video_char.h \
  Include/video/video_mode.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/memory/addresses.h \
  Include/memory/strings/strings.h Include/hal/gdt.h Include/hal/idt.h \
  Include/hal/int.h Include/hal/processor.h Include/hal/features.h \
  Include/kernel.h Include/hal/timer.h Include/app/rmi.h \
  Include/memory/arguments.h Include/app/callbacks.h \
  Include/app/ioperm.h Include/app/tdm.h Include/mtask/scheduler.h \
  Include/hal/syscall.h Include/mtask/thread.h Include/exceptions.h \
  Include/hal/irq_han.h Include/appearance/appearance.h \
  Include/cmdline.h Include/kernel_private.h Include/app/usermodefuncs.h \
  Include/modules/loadmodule.h Include/hal/atom.h \
  Include/kdebug/symboltable.h Include/cpp/cppsupport.h \
  Include/globalconfig.h Include/test/unittest.h
	@echo -e "MAIN [kernel.c]"
	@$(CC) -c $(C_ARGS)  kernel.c -o kernel.o


boot.o: boot.S Include/multiboot.h Include/memory/addresses.h
	@echo -e "MAIN [boot.S]"
	@$(CC) -c $(C_ARGS)  boot.S -o boot.o


cmdline.o: cmdline.c Include/typedefs.h Include/compilerspec.h \
  Include/cmdline.h Include/exceptions.h Include/hal/irq_han.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h \
  Include/memory/strings/strings.h Include/hal/features.h \
  Include/memory/memory.h Include/flags.h Include/video/video_char.h \
  Include/debug.h Include/profiler/useprofile.h \
  Include/profiler/profiling.h Include/profiler/measurement.h \
  Include/globalconfig.h
	@echo -e "MAIN [cmdline.c]"
	@$(CC) -c $(C_ARGS)  cmdline.c -o cmdline.o


app/ioperm.o: app/ioperm.c Include/typedefs.h Include/compilerspec.h \
  Include/mtask/scheduler.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/hal/syscall.h Include/hal/tss.h Include/app/ioperm.h \
  Include/app/tdm.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/memory/memory.h Include/flags.h \
  Include/memory/usermem.h Include/memory/addresses.h Include/debug.h \
  Include/video/video_char.h
	@echo -e "SUBPART [app/ioperm.c]"
	@$(CC) -c $(C_ARGS) app/ioperm.c  -o app/ioperm.o


app/ipc.o: app/ipc.c Include/typedefs.h Include/compilerspec.h \
  Include/app/ipc.h Include/app/ipc_types.h Include/mtask/privileges.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_user.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/app/rmi.h Include/memory/arguments.h \
  Include/app/processnames.h Include/mtask/pid.h \
  Include/memory/strings/strings.h Include/debug.h \
  Include/video/video_char.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h Include/memory/memory.h \
  Include/flags.h Include/memory/usermem.h Include/memory/adt/hashing.h
	@echo -e "SUBPART [app/ipc.c]"
	@$(CC) -c $(C_ARGS) app/ipc.c  -o app/ipc.o


app/processnames.o: app/processnames.c Include/typedefs.h \
  Include/compilerspec.h Include/mtask/pid_types.h \
  Include/memory/strings/strings.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h Include/flags.h \
  Include/debug.h Include/video/video_char.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h
	@echo -e "SUBPART [app/processnames.c]"
	@$(CC) -c $(C_ARGS) app/processnames.c  -o app/processnames.o


app/rmi.o: app/rmi.c Include/typedefs.h Include/compilerspec.h \
  Include/memory/adt/hashmap.h Include/app/ipc_types.h \
  Include/mtask/privileges.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_user.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/app/rmi.h Include/memory/arguments.h \
  Include/app/rmi_internals.h Include/memory/memory.h Include/flags.h \
  Include/memory/usermem.h Include/memory/strings/strings.h \
  Include/memory/threadmemory.h Include/memory/addresses.h \
  Include/memory/protection.h Include/mtask/thread.h \
  Include/mtask/scheduler.h Include/hal/syscall.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/debug.h Include/video/video_char.h
	@echo -e "SUBPART [app/rmi.c]"
	@$(CC) -c $(C_ARGS) app/rmi.c  -o app/rmi.o


app/syscall.o: app/syscall.c Include/typedefs.h Include/compilerspec.h \
  Include/memory/memory.h Include/memory/memfunc.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h Include/flags.h \
  Include/hal/syscall.h Include/hal/tss.h Include/hal/processor.h \
  Include/hal/msr.h Include/hal/gdt.h Include/hal/abi.h \
  Include/mtask/scheduler.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h Include/hal/timer.h \
  Include/debug.h Include/video/video_char.h
	@echo -e "SUBPART [app/syscall.c]"
	@$(CC) -c $(C_ARGS) app/syscall.c  -o app/syscall.o


app/usermodefuncs.o: app/usermodefuncs.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/hal/syscall.h \
  Include/app/sct.h Include/app/rmi.h Include/memory/arguments.h \
  Include/mtask/scheduler.h Include/mtask/thread.h Include/mtask/pid.h \
  Include/app/usermodefuncs.h Include/app/wrappers.h \
  Include/video/video_char.h Include/memory/usermem.h \
  Include/app/ioperm.h Include/app/tdm.h Include/app/ipc.h \
  Include/memory/protection.h Include/app/callbacks.h \
  Include/hal/irq_han.h Include/hal/8259.h Include/kdebug/i386-stub.h \
  Include/kdebug/debugbuffer.h Include/profiler/useprofile.h \
  Include/profiler/profiling.h Include/profiler/measurement.h \
  Include/hal/system.h Include/debug.h Include/exceptions.h
	@echo -e "SUBPART [app/usermodefuncs.c]"
	@$(CC) -c $(C_ARGS) app/usermodefuncs.c  -o app/usermodefuncs.o


memory/memory.o: memory/memory.c Include/flags.h Include/typedefs.h \
  Include/compilerspec.h Include/mtask/semaphore.h \
  Include/memory/adt/hashmap.h Include/exceptions.h \
  Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/memory/memory.h \
  Include/memory/memory_types.h Include/memory/addresses.h \
  Include/memory/memtune.h Include/hal/atom.h Include/memory/usermem.h \
  Include/debug.h Include/video/video_char.h Include/hal/processor.h \
  Include/hal/tss.h Include/hal/gdt.h Include/app/ioperm.h
	@echo -e "SUBPART [memory/memory.c]"
	@$(CC) -c $(C_ARGS) memory/memory.c  -o memory/memory.o


memory/mmu.o: memory/mmu.c Include/typedefs.h Include/compilerspec.h \
  Include/memory/mmu.h Include/multiboot.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/mtask/semaphore.h \
  Include/mtask/semaphore_types.h Include/mtask/task_types.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/hal/processor.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h Include/memory/addresses.h \
  Include/memory/memory_types.h Include/memory/strings/strings.h \
  Include/memory/memory.h Include/flags.h Include/memory/memtune.h \
  Include/memory/stackmem.h Include/video/video_char.h \
  Include/math/mathfuncs.h Include/debug.h
	@echo -e "SUBPART [memory/mmu.c]"
	@$(CC) -c $(C_ARGS) memory/mmu.c  -o memory/mmu.o


memory/paging.o: memory/paging.c Include/flags.h Include/typedefs.h \
  Include/compilerspec.h Include/memory/adt/hashmap.h \
  Include/memory/paging.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/memory/memory.h Include/mtask/task_types.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/memory/addresses.h Include/hal/tss.h \
  Include/mtask/scheduler.h Include/hal/syscall.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/hal/gdt.h Include/hal/processor.h Include/debug.h \
  Include/video/video_char.h Include/video/video_mode.h \
  Include/hal/isr.h
	@echo -e "SUBPART [memory/paging.c]"
	@$(CC) -c $(C_ARGS) memory/paging.c  -o memory/paging.o


memory/protection.o: memory/protection.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/adt/hashmap.h \
  Include/memory/paging.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/memory/protection.h Include/mtask/task_types.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h
	@echo -e "SUBPART [memory/protection.c]"
	@$(CC) -c $(C_ARGS) memory/protection.c  -o memory/protection.o


memory/stackmem.o: memory/stackmem.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/addresses.h Include/exceptions.h \
  Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/video/video_char.h
	@echo -e "SUBPART [memory/stackmem.c]"
	@$(CC) -c $(C_ARGS) memory/stackmem.c  -o memory/stackmem.o


memory/threadmemory.o: memory/threadmemory.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/threadmemory.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/memory/memory.h Include/flags.h \
  Include/mtask/scheduler.h Include/hal/syscall.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/debug.h Include/video/video_char.h
	@echo -e "SUBPART [memory/threadmemory.c]"
	@$(CC) -c $(C_ARGS) memory/threadmemory.c  -o memory/threadmemory.o


memory/strings/memcpy.o: memory/strings/memcpy.S
	@echo -e "SUBPART [memory/strings/memcpy.S]"
	@$(CC) -c $(C_ARGS) memory/strings/memcpy.S  -o memory/strings/memcpy.o


memory/strings/memmove.o: memory/strings/memmove.S
	@echo -e "SUBPART [memory/strings/memmove.S]"
	@$(CC) -c $(C_ARGS) memory/strings/memmove.S  -o memory/strings/memmove.o


memory/strings/memset.o: memory/strings/memset.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/memset.c]"
	@$(CC) -c $(C_ARGS) memory/strings/memset.c  -o memory/strings/memset.o


memory/strings/numbers.o: memory/strings/numbers.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/numbers.c]"
	@$(CC) -c $(C_ARGS) memory/strings/numbers.c  -o memory/strings/numbers.o


memory/strings/strchr.o: memory/strings/strchr.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/strchr.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strchr.c  -o memory/strings/strchr.o


memory/strings/strcmp.o: memory/strings/strcmp.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/strcmp.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strcmp.c  -o memory/strings/strcmp.o


memory/strings/strcpy.o: memory/strings/strcpy.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/strings/strings.h
	@echo -e "SUBPART [memory/strings/strcpy.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strcpy.c  -o memory/strings/strcpy.o


memory/strings/strfuncs.o: memory/strings/strfuncs.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/strings/strings.h
	@echo -e "SUBPART [memory/strings/strfuncs.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strfuncs.c  -o memory/strings/strfuncs.o


memory/strings/strlen.o: memory/strings/strlen.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/strlen.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strlen.c  -o memory/strings/strlen.o


memory/strings/strncmp.o: memory/strings/strncmp.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/strncmp.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strncmp.c  -o memory/strings/strncmp.o


memory/strings/strncpy.o: memory/strings/strncpy.c Include/typedefs.h \
  Include/compilerspec.h
	@echo -e "SUBPART [memory/strings/strncpy.c]"
	@$(CC) -c $(C_ARGS) memory/strings/strncpy.c  -o memory/strings/strncpy.o


memory/adt/bitmap.o: memory/adt/bitmap.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/adt/bitmap.h \
  Include/exceptions.h Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h \
  Include/debug.h Include/video/video_char.h
	@echo -e "SUBPART [memory/adt/bitmap.c]"
	@$(CC) -c $(C_ARGS) memory/adt/bitmap.c  -o memory/adt/bitmap.o


memory/adt/hashing.o: memory/adt/hashing.c Include/typedefs.h \
  Include/compilerspec.h Include/memory/adt/hashing.h
	@echo -e "SUBPART [memory/adt/hashing.c]"
	@$(CC) -c $(C_ARGS) memory/adt/hashing.c  -o memory/adt/hashing.o


memory/adt/hashmap.o: memory/adt/hashmap.c Include/typedefs.h \
  Include/compilerspec.h Include/debug.h Include/video/video_char.h \
  Include/exceptions.h Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/memory/memory.h \
  Include/flags.h Include/math/mathfuncs.h
	@echo -e "SUBPART [memory/adt/hashmap.c]"
	@$(CC) -c $(C_ARGS) memory/adt/hashmap.c  -o memory/adt/hashmap.o


modules/elf32.o: modules/elf32.cpp Include/typedefs.h \
  Include/compilerspec.h Include/modules/elf_types.h \
  Include/modules/elf_common.h Include/memory/mmu.h Include/multiboot.h \
  Include/memory/addresses.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h Include/flags.h \
  Include/memory/strings/strings.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/memory/usermem.h Include/debug.h Include/video/video_char.h \
  Include/modules/elf32.h Include/modules/load_types.h \
  Include/memory/arguments.h Include/cpp/staticlist.h \
  Include/cpp/exception.h /usr/include/c++/4.4/map \
  /usr/include/c++/4.4/bits/stl_tree.h \
  /usr/include/c++/4.4/bits/stl_algobase.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
  /usr/include/features.h /usr/include/bits/predefs.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
  /usr/include/c++/4.4/cstddef /usr/lib/clang/2.8/include/stddef.h \
  /usr/include/c++/4.4/bits/functexcept.h \
  /usr/include/c++/4.4/exception_defines.h \
  /usr/include/c++/4.4/bits/cpp_type_traits.h \
  /usr/include/c++/4.4/ext/type_traits.h \
  /usr/include/c++/4.4/ext/numeric_traits.h \
  /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
  /usr/include/c++/4.4/bits/concept_check.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.4/bits/stl_iterator.h \
  /usr/include/c++/4.4/debug/debug.h \
  /usr/include/c++/4.4/bits/allocator.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
  /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
  /usr/include/c++/4.4/exception \
  /usr/include/c++/4.4/bits/stl_function.h \
  /usr/include/c++/4.4/backward/binders.h \
  /usr/include/c++/4.4/bits/stl_map.h \
  /usr/include/c++/4.4/initializer_list \
  /usr/include/c++/4.4/bits/stl_multimap.h \
  Include/modules/loadexecutable.h Include/cpp/specialmem.h \
  Include/cpp/specialmem_int.h
	@echo -e "SUBPART [modules/elf32.cpp]"
	@$(CPPC) -c $(CPP_ARGS) modules/elf32.cpp  -o modules/elf32.o


modules/loadexecutable.o: modules/loadexecutable.cpp Include/typedefs.h \
  Include/compilerspec.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/memory/usermem.h \
  Include/memory/arguments.h Include/memory/addresses.h \
  Include/memory/strings/strings.h Include/mtask/thread.h \
  Include/mtask/scheduler.h Include/hal/syscall.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/debug.h Include/video/video_char.h \
  Include/profiler/useprofile.h Include/profiler/profiling.h \
  Include/profiler/measurement.h Include/modules/loadexecutable.h \
  Include/modules/load_types.h Include/cpp/staticlist.h \
  Include/cpp/exception.h /usr/include/c++/4.4/map \
  /usr/include/c++/4.4/bits/stl_tree.h \
  /usr/include/c++/4.4/bits/stl_algobase.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
  /usr/include/features.h /usr/include/bits/predefs.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
  /usr/include/c++/4.4/cstddef /usr/lib/clang/2.8/include/stddef.h \
  /usr/include/c++/4.4/bits/functexcept.h \
  /usr/include/c++/4.4/exception_defines.h \
  /usr/include/c++/4.4/bits/cpp_type_traits.h \
  /usr/include/c++/4.4/ext/type_traits.h \
  /usr/include/c++/4.4/ext/numeric_traits.h \
  /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
  /usr/include/c++/4.4/bits/concept_check.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.4/bits/stl_iterator.h \
  /usr/include/c++/4.4/debug/debug.h \
  /usr/include/c++/4.4/bits/allocator.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
  /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
  /usr/include/c++/4.4/exception \
  /usr/include/c++/4.4/bits/stl_function.h \
  /usr/include/c++/4.4/backward/binders.h \
  /usr/include/c++/4.4/bits/stl_map.h \
  /usr/include/c++/4.4/initializer_list \
  /usr/include/c++/4.4/bits/stl_multimap.h
	@echo -e "SUBPART [modules/loadexecutable.cpp]"
	@$(CPPC) -c $(CPP_ARGS) modules/loadexecutable.cpp  -o modules/loadexecutable.o


modules/loadmodule.o: modules/loadmodule.cpp Include/typedefs.h \
  Include/compilerspec.h Include/multiboot.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/memory/usermem.h \
  Include/memory/arguments.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h Include/mtask/thread.h \
  Include/mtask/scheduler.h Include/hal/syscall.h \
  Include/memory/strings/strings.h Include/debug.h \
  Include/video/video_char.h Include/modules/loadmodule.h \
  Include/modules/elf32.h Include/modules/elf_types.h \
  Include/modules/elf_common.h Include/modules/load_types.h \
  Include/cpp/staticlist.h Include/cpp/exception.h \
  /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
  /usr/include/c++/4.4/bits/stl_algobase.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
  /usr/include/features.h /usr/include/bits/predefs.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
  /usr/include/c++/4.4/cstddef /usr/lib/clang/2.8/include/stddef.h \
  /usr/include/c++/4.4/bits/functexcept.h \
  /usr/include/c++/4.4/exception_defines.h \
  /usr/include/c++/4.4/bits/cpp_type_traits.h \
  /usr/include/c++/4.4/ext/type_traits.h \
  /usr/include/c++/4.4/ext/numeric_traits.h \
  /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
  /usr/include/c++/4.4/bits/concept_check.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.4/bits/stl_iterator.h \
  /usr/include/c++/4.4/debug/debug.h \
  /usr/include/c++/4.4/bits/allocator.h \
  /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
  /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
  /usr/include/c++/4.4/exception \
  /usr/include/c++/4.4/bits/stl_function.h \
  /usr/include/c++/4.4/backward/binders.h \
  /usr/include/c++/4.4/bits/stl_map.h \
  /usr/include/c++/4.4/initializer_list \
  /usr/include/c++/4.4/bits/stl_multimap.h \
  Include/modules/loadexecutable.h
	@echo -e "SUBPART [modules/loadmodule.cpp]"
	@$(CPPC) -c $(CPP_ARGS) modules/loadmodule.cpp  -o modules/loadmodule.o


mtask/pid.o: mtask/pid.c Include/typedefs.h Include/compilerspec.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/debug.h Include/video/video_char.h \
  Include/exceptions.h Include/hal/irq_han.h Include/app/callbacks.h \
  Include/app/tdm.h Include/mtask/pid.h
	@echo -e "SUBPART [mtask/pid.c]"
	@$(CC) -c $(C_ARGS) mtask/pid.c  -o mtask/pid.o


mtask/scheduler.o: mtask/scheduler.c Include/typedefs.h \
  Include/compilerspec.h Include/hal/tss.h Include/hal/msr.h \
  Include/memory/adt/hashmap.h Include/memory/paging.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/task_types.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/mtask/event_types.h \
  Include/mtask/thread.h Include/memory/arguments.h \
  Include/mtask/scheduler.h Include/hal/syscall.h \
  Include/memory/memory.h Include/flags.h Include/hal/processor.h \
  Include/hal/8259.h Include/hal/timer.h \
  Include/memory/strings/strings.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h \
  Include/profiler/measurement.h Include/hal/atom.h Include/debug.h \
  Include/video/video_char.h
	@echo -e "SUBPART [mtask/scheduler.c]"
	@$(CC) -c $(C_ARGS) mtask/scheduler.c  -o mtask/scheduler.o


mtask/semaphore.o: mtask/semaphore.c Include/typedefs.h \
  Include/compilerspec.h Include/exceptions.h Include/hal/irq_han.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h \
  Include/memory/memory.h Include/flags.h Include/memory/adt/hashing.h \
  Include/mtask/semaphore_types.h Include/mtask/scheduler.h \
  Include/hal/syscall.h Include/hal/atom.h \
  Include/memory/strings/strings.h Include/debug.h \
  Include/video/video_char.h
	@echo -e "SUBPART [mtask/semaphore.c]"
	@$(CC) -c $(C_ARGS) mtask/semaphore.c  -o mtask/semaphore.o


mtask/thread.o: mtask/thread.c Include/typedefs.h Include/compilerspec.h \
  Include/memory/memory.h Include/memory/memfunc.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h Include/flags.h \
  Include/memory/arguments.h Include/memory/threadmemory.h \
  Include/mtask/scheduler.h Include/hal/syscall.h Include/mtask/thread.h \
  Include/mtask/pid.h Include/app/rmi.h Include/memory/usermem.h \
  Include/hal/gdt.h Include/hal/atom.h Include/hal/x86flags.h \
  Include/hal/abi.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h \
  Include/memory/strings/strings.h Include/debug.h \
  Include/video/video_char.h
	@echo -e "SUBPART [mtask/thread.c]"
	@$(CC) -c $(C_ARGS) mtask/thread.c  -o mtask/thread.o


hal/8259.o: hal/8259.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/io.h Include/hal/8259.h Include/hal/8259_ctrl.h \
  Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/exceptions.h
	@echo -e "SUBPART [hal/8259.c]"
	@$(CC) -c $(C_ARGS) hal/8259.c  -o hal/8259.o


hal/abi.o: hal/abi.c Include/typedefs.h Include/compilerspec.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h
	@echo -e "SUBPART [hal/abi.c]"
	@$(CC) -c $(C_ARGS) hal/abi.c  -o hal/abi.o


hal/atom.o: hal/atom.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/atom.h Include/hal/int.h
	@echo -e "SUBPART [hal/atom.c]"
	@$(CC) -c $(C_ARGS) hal/atom.c  -o hal/atom.o


hal/features.o: hal/features.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/features.h Include/exceptions.h Include/hal/irq_han.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h \
  Include/video/video_char.h Include/hal/timer.h \
  Include/hal/timer_ctrl.h Include/hal/syscall.h
	@echo -e "SUBPART [hal/features.c]"
	@$(CC) -c $(C_ARGS) hal/features.c  -o hal/features.o


hal/gdt.o: hal/gdt.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/gdt.h Include/hal/tss.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/memory/addresses.h
	@echo -e "SUBPART [hal/gdt.c]"
	@$(CC) -c $(C_ARGS) hal/gdt.c  -o hal/gdt.o


hal/idt.o: hal/idt.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/idt.h Include/hal/isr.h Include/memory/memfunc.h \
  Include/hal/irq.h Include/hal/8259.h Include/hal/gdt.h \
  Include/hal/systrap.h
	@echo -e "SUBPART [hal/idt.c]"
	@$(CC) -c $(C_ARGS) hal/idt.c  -o hal/idt.o


hal/irq_han.o: hal/irq_han.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/8259.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/task_types.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/mtask/scheduler.h Include/hal/syscall.h \
  Include/app/callbacks.h Include/hal/irq_han.h Include/app/tdm.h \
  Include/memory/threadmemory.h Include/memory/memory.h Include/flags.h \
  Include/exceptions.h Include/debug.h Include/video/video_char.h \
  Include/globalconfig.h
	@echo -e "SUBPART [hal/irq_han.c]"
	@$(CC) -c $(C_ARGS) hal/irq_han.c  -o hal/irq_han.o


hal/irq.o: hal/irq.S Include/hal/msr.h Include/hal/systrap.h
	@echo -e "SUBPART [hal/irq.S]"
	@$(CC) -c $(C_ARGS) hal/irq.S  -o hal/irq.o


hal/isr.o: hal/isr.c Include/typedefs.h Include/compilerspec.h \
  Include/exceptions.h Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/hal/int.h \
  Include/hal/processor.h
	@echo -e "SUBPART [hal/isr.c]"
	@$(CC) -c $(C_ARGS) hal/isr.c  -o hal/isr.o


hal/system.o: hal/system.c Include/typedefs.h Include/compilerspec.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/tdm.h Include/hal/system.h \
  Include/hal/barekbd.h Include/hal/io.h Include/hal/idt.h \
  Include/debug.h Include/video/video_char.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h
	@echo -e "SUBPART [hal/system.c]"
	@$(CC) -c $(C_ARGS) hal/system.c  -o hal/system.o


hal/timer.o: hal/timer.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/timer.h Include/hal/timer_ctrl.h Include/hal/io.h \
  Include/exceptions.h Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h
	@echo -e "SUBPART [hal/timer.c]"
	@$(CC) -c $(C_ARGS) hal/timer.c  -o hal/timer.o


hal/tss.o: hal/tss.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/tss.h Include/memory/memory.h Include/memory/memfunc.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h Include/flags.h \
  Include/memory/addresses.h Include/exceptions.h Include/hal/irq_han.h \
  Include/app/callbacks.h Include/app/tdm.h Include/hal/gdt.h \
  Include/memory/usermem.h Include/app/ioperm.h
	@echo -e "SUBPART [hal/tss.c]"
	@$(CC) -c $(C_ARGS) hal/tss.c  -o hal/tss.o


hal/uart.o: hal/uart.c Include/typedefs.h Include/compilerspec.h \
  Include/hal/uart.h Include/hal/io.h Include/exceptions.h \
  Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h Include/video/video_char.h \
  Include/hal/8259.h
	@echo -e "SUBPART [hal/uart.c]"
	@$(CC) -c $(C_ARGS) hal/uart.c  -o hal/uart.o


appearance/appearance.o: appearance/appearance.c Include/typedefs.h \
  Include/compilerspec.h Include/video/video_char.h \
  Include/video/video_mode.h Include/appearance/appearance.h \
  Include/version.h Include/verdate.h
	@echo -e "SUBPART [appearance/appearance.c]"
	@$(CC) -c $(C_ARGS) appearance/appearance.c  -o appearance/appearance.o


video/video_char.o: video/video_char.c Include/typedefs.h \
  Include/compilerspec.h Include/video/video_char.h \
  Include/memory/memfunc.h Include/hal/io.h Include/c/stdarg.h \
  Include/video/video_mode.h Include/debug.h Include/exceptions.h \
  Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h \
  Include/globalconfig.h
	@echo -e "SUBPART [video/video_char.c]"
	@$(CC) -c $(C_ARGS) video/video_char.c  -o video/video_char.o


video/video_mode.o: video/video_mode.c Include/typedefs.h \
  Include/compilerspec.h Include/video/video_mode.h \
  Include/video/vga_io.h Include/video/fonts/font8.h \
  Include/video/fonts/font16.h Include/hal/io.h Include/memory/memfunc.h
	@echo -e "SUBPART [video/video_mode.c]"
	@$(CC) -c $(C_ARGS) video/video_mode.c  -o video/video_mode.o


math/mathfuncs.o: math/mathfuncs.c Include/typedefs.h \
  Include/compilerspec.h Include/math/mathfuncs.h
	@echo -e "SUBPART [math/mathfuncs.c]"
	@$(CC) -c $(C_ARGS) math/mathfuncs.c  -o math/mathfuncs.o


math/lib/udiv3.o: math/lib/udiv3.c Include/typedefs.h \
  Include/compilerspec.h Include/math/udiv3.h
	@echo -e "SUBPART [math/lib/udiv3.c]"
	@$(CC) -c $(C_ARGS) math/lib/udiv3.c  -o math/lib/udiv3.o


kdebug/debugbuffer.o: kdebug/debugbuffer.c Include/typedefs.h \
  Include/compilerspec.h Include/kdebug/debugbuffer.h \
  Include/video/video_char.h
	@echo -e "SUBPART [kdebug/debugbuffer.c]"
	@$(CC) -c $(C_ARGS) kdebug/debugbuffer.c  -o kdebug/debugbuffer.o


kdebug/i386-stub.o: kdebug/i386-stub.c
	@echo -e "SUBPART [kdebug/i386-stub.c]"
	@$(CC) -c $(C_ARGS) kdebug/i386-stub.c  -o kdebug/i386-stub.o


kdebug/kdebug.o: kdebug/kdebug.c
	@echo -e "SUBPART [kdebug/kdebug.c]"
	@$(CC) -c $(C_ARGS) kdebug/kdebug.c  -o kdebug/kdebug.o


kdebug/stackcheck.o: kdebug/stackcheck.c Include/typedefs.h \
  Include/compilerspec.h Include/exceptions.h Include/hal/irq_han.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/app/callbacks.h Include/app/tdm.h
	@echo -e "SUBPART [kdebug/stackcheck.c]"
	@$(CC) -c $(C_ARGS) kdebug/stackcheck.c  -o kdebug/stackcheck.o


kdebug/symboltable.o: kdebug/symboltable.c Include/typedefs.h \
  Include/compilerspec.h Include/debug.h Include/video/video_char.h \
  Include/exceptions.h Include/hal/irq_han.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/memfunc.h \
  Include/memory/mmu.h Include/multiboot.h Include/mtask/semaphore.h \
  Include/mtask/privileges.h Include/hal/x86regs.h \
  Include/mtask/pid_types.h Include/memory/adt/linkedlist.h \
  Include/app/ipc_types.h Include/app/rmi_types.h \
  Include/mtask/threadqueue.h Include/app/ipc_user.h \
  Include/app/callbacks.h Include/app/tdm.h kdebug/symbolmap.gen
	@echo -e "SUBPART [kdebug/symboltable.c]"
	@$(CC) -c $(C_ARGS) kdebug/symboltable.c  -o kdebug/symboltable.o


test/unittest.o: test/unittest.c Include/typedefs.h \
  Include/compilerspec.h Include/test/unittest.h \
  Include/mtask/task_types.h Include/memory/paging.h \
  Include/memory/adt/hashmap.h Include/memory/adt/bitmap.h \
  Include/memory/memfunc.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/mtask/scheduler.h Include/hal/syscall.h \
  Include/debug.h Include/video/video_char.h Include/exceptions.h \
  Include/hal/irq_han.h Include/app/callbacks.h Include/app/tdm.h
	@echo -e "SUBPART [test/unittest.c]"
	@$(CC) -c $(C_ARGS) test/unittest.c  -o test/unittest.o


cpp/cppsupport.o: cpp/cppsupport.cpp Include/typedefs.h \
  Include/compilerspec.h Include/memory/memory.h \
  Include/memory/memfunc.h Include/mtask/task_types.h \
  Include/memory/paging.h Include/memory/adt/hashmap.h \
  Include/memory/adt/bitmap.h Include/memory/mmu.h Include/multiboot.h \
  Include/mtask/semaphore.h Include/mtask/privileges.h \
  Include/hal/x86regs.h Include/mtask/pid_types.h \
  Include/memory/adt/linkedlist.h Include/app/ipc_types.h \
  Include/app/rmi_types.h Include/mtask/threadqueue.h \
  Include/app/ipc_user.h Include/flags.h Include/memory/stackmem.h \
  Include/exceptions.h Include/hal/irq_han.h Include/app/callbacks.h \
  Include/app/tdm.h Include/video/video_char.h Include/cpp/cppsupport.h \
  Include/cpp/specialmem_int.h
	@echo -e "SUBPART [cpp/cppsupport.cpp]"
	@$(CPPC) -c $(CPP_ARGS) cpp/cppsupport.cpp  -o cpp/cppsupport.o


cpp/icxxabi.o: cpp/icxxabi.cpp Include/typedefs.h Include/compilerspec.h \
  cpp/./icxxabi.h
	@echo -e "SUBPART [cpp/icxxabi.cpp]"
	@$(CPPC) -c $(CPP_ARGS) cpp/icxxabi.cpp  -o cpp/icxxabi.o


cpp/list.o: cpp/list.cpp Include/typedefs.h Include/compilerspec.h \
  Include/cpp/staticlist.h Include/cpp/exception.h
	@echo -e "SUBPART [cpp/list.cpp]"
	@$(CPPC) -c $(CPP_ARGS) cpp/list.cpp  -o cpp/list.o


profiler/instrument.o: profiler/instrument.c
	@echo -e "SUBPART [profiler/instrument.c]"
	@$(CC) -c $(C_ARGS) profiler/instrument.c  -o profiler/instrument.o


profiler/useprofile.o: profiler/useprofile.c
	@echo -e "SUBPART [profiler/useprofile.c]"
	@$(CC) -c $(C_ARGS) profiler/useprofile.c  -o profiler/useprofile.o


ray: Include/verdate.h  kernel.o boot.o cmdline.o app/ioperm.o app/ipc.o app/processnames.o app/rmi.o app/syscall.o app/usermodefuncs.o memory/memory.o memory/mmu.o memory/paging.o memory/protection.o memory/stackmem.o memory/threadmemory.o memory/strings/memcpy.o memory/strings/memmove.o memory/strings/memset.o memory/strings/numbers.o memory/strings/strchr.o memory/strings/strcmp.o memory/strings/strcpy.o memory/strings/strfuncs.o memory/strings/strlen.o memory/strings/strncmp.o memory/strings/strncpy.o memory/adt/bitmap.o memory/adt/hashing.o memory/adt/hashmap.o modules/elf32.o modules/loadexecutable.o modules/loadmodule.o mtask/pid.o mtask/scheduler.o mtask/semaphore.o mtask/thread.o hal/8259.o hal/abi.o hal/atom.o hal/features.o hal/gdt.o hal/idt.o hal/irq_han.o hal/irq.o hal/isr.o hal/system.o hal/timer.o hal/tss.o hal/uart.o appearance/appearance.o video/video_char.o video/video_mode.o math/mathfuncs.o math/lib/udiv3.o kdebug/debugbuffer.o kdebug/i386-stub.o kdebug/kdebug.o kdebug/stackcheck.o kdebug/symboltable.o test/unittest.o cpp/cppsupport.o cpp/icxxabi.o cpp/list.o profiler/instrument.o profiler/useprofile.o
	@echo -e "Building kernel image ray..."
	@$(LD) -nostdlib -nostartfiles -ffreestanding -fno-ident -T raylink.ld -o ray  kernel.o boot.o cmdline.o app/ioperm.o app/ipc.o app/processnames.o app/rmi.o app/syscall.o app/usermodefuncs.o memory/memory.o memory/mmu.o memory/paging.o memory/protection.o memory/stackmem.o memory/threadmemory.o memory/strings/memcpy.o memory/strings/memmove.o memory/strings/memset.o memory/strings/numbers.o memory/strings/strchr.o memory/strings/strcmp.o memory/strings/strcpy.o memory/strings/strfuncs.o memory/strings/strlen.o memory/strings/strncmp.o memory/strings/strncpy.o memory/adt/bitmap.o memory/adt/hashing.o memory/adt/hashmap.o modules/elf32.o modules/loadexecutable.o modules/loadmodule.o mtask/pid.o mtask/scheduler.o mtask/semaphore.o mtask/thread.o hal/8259.o hal/abi.o hal/atom.o hal/features.o hal/gdt.o hal/idt.o hal/irq_han.o hal/irq.o hal/isr.o hal/system.o hal/timer.o hal/tss.o hal/uart.o appearance/appearance.o video/video_char.o video/video_mode.o math/mathfuncs.o math/lib/udiv3.o kdebug/debugbuffer.o kdebug/i386-stub.o kdebug/kdebug.o kdebug/stackcheck.o kdebug/symboltable.o test/unittest.o cpp/cppsupport.o cpp/icxxabi.o cpp/list.o profiler/instrument.o profiler/useprofile.o -lgcc -lsupc++
	@echo -e "Updating symbol table..."
	@./updatesymboltable
	@$(LD) -nostdlib -nostartfiles -ffreestanding -fno-ident -T raylink.ld -o ray  kernel.o boot.o cmdline.o app/ioperm.o app/ipc.o app/processnames.o app/rmi.o app/syscall.o app/usermodefuncs.o memory/memory.o memory/mmu.o memory/paging.o memory/protection.o memory/stackmem.o memory/threadmemory.o memory/strings/memcpy.o memory/strings/memmove.o memory/strings/memset.o memory/strings/numbers.o memory/strings/strchr.o memory/strings/strcmp.o memory/strings/strcpy.o memory/strings/strfuncs.o memory/strings/strlen.o memory/strings/strncmp.o memory/strings/strncpy.o memory/adt/bitmap.o memory/adt/hashing.o memory/adt/hashmap.o modules/elf32.o modules/loadexecutable.o modules/loadmodule.o mtask/pid.o mtask/scheduler.o mtask/semaphore.o mtask/thread.o hal/8259.o hal/abi.o hal/atom.o hal/features.o hal/gdt.o hal/idt.o hal/irq_han.o hal/irq.o hal/isr.o hal/system.o hal/timer.o hal/tss.o hal/uart.o appearance/appearance.o video/video_char.o video/video_mode.o math/mathfuncs.o math/lib/udiv3.o kdebug/debugbuffer.o kdebug/i386-stub.o kdebug/kdebug.o kdebug/stackcheck.o kdebug/symboltable.o test/unittest.o cpp/cppsupport.o cpp/icxxabi.o cpp/list.o profiler/instrument.o profiler/useprofile.o -lgcc -lsupc++
	@echo -e "Done."


.PHONY: clean Include/verdate.h
