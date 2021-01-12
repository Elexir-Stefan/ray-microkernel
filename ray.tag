<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>ioperm.c</name>
    <path>/home/stefan/src/ray/ray/src/app/</path>
    <filename>ioperm_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="tss_8h" name="tss.h" local="yes" imported="no">hal/tss.h</includes>
    <includes id="ioperm_8h" name="ioperm.h" local="yes" imported="no">app/ioperm.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <member kind="function">
      <type>void</type>
      <name>CreateThreadIOPermission</name>
      <anchorfile>ioperm_8c.html</anchorfile>
      <anchor>9a1bf3065d9fa6f6d8b15bcce5fb156e</anchor>
      <arglist>(MEM_MANAGE *userMemory)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OpenIOPort</name>
      <anchorfile>ioperm_8c.html</anchorfile>
      <anchor>5e58c118715299e0b023243e1962056b</anchor>
      <arglist>(UINT16 port)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rmi.c</name>
    <path>/home/stefan/src/ray/ray/src/app/</path>
    <filename>rmi_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="rmi_8h" name="rmi.h" local="yes" imported="no">app/rmi.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelInitRMI</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>17fae4733e590c0f05c287ddcc76ad47</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>RMISetup</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>832215f945f46b82196fd3091b1aad2e</anchor>
      <arglist>(RMISERIAL threadSerial, UINT32 count)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>RMInvoke</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>3e5577d33b8e7af96141a070d54f400d</anchor>
      <arglist>(RMISERIAL remoteSerial, RMIFUNCTION funcExport, UINT32 value)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>AllocateMessageBuffer</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>7a06dcb15c7baa4ebc1ac7e3bea3dab3</anchor>
      <arglist>(UINT32 msgSize, MSG_TYPE type)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>FreeMessageBuffer</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>b38091bdc9d728d1190e6fac9c10c74a</anchor>
      <arglist>(UINT32 msgBuffer)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SignalProcessMessage</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>cd47e53eaccfaa7695ce15b82430f449</anchor>
      <arglist>(RMISERIAL process, RMIFUNCTION funcExport, RMIMESSAGE message)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SendProcessMessage</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>ae817033762f57ff6f65908c7fe3aca5</anchor>
      <arglist>(RMISERIAL process, UINT32 msgMagic, RMIMESSAGE message)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>RegisterMsgReceiver</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>d4de57cae313555844ddb35b787874e7</anchor>
      <arglist>(UINT32 messageMagic, MSG_TYPE messageType, UINT32 maxSize, RMIMESSAGE msgHandle, PRIVLEVEL minPriv, BOOL partnership)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>RegisterSigReceiver</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>0ae63478f938e0bcc3b1f272c20ee7a4</anchor>
      <arglist>(UINT32 messageMagic, UINT32 valueHandle, PRIVLEVEL minPriv, BOOL partnership)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SendSignal</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>9731bbb4eb02eb2132d9c1224b336b4c</anchor>
      <arglist>(RMISERIAL process, UINT32 msgMagic, UINT32 value)</arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>selNewTask</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>b4446be0816ed744339d5721a1886a2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>userMemPointer</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>3d833f53f55a85d9dd0745938e18f619</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>userMemValue</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>31744b5b58bdde7ff3e465f7fd66da2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>rmi_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usermodefuncs.c</name>
    <path>/home/stefan/src/ray/ray/src/app/</path>
    <filename>usermodefuncs_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="syscall_8h" name="syscall.h" local="yes" imported="no">hal/syscall.h</includes>
    <includes id="sct_8h" name="sct.h" local="yes" imported="no">app/sct.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="usermodefuncs_8h" name="usermodefuncs.h" local="yes" imported="no">app/usermodefuncs.h</includes>
    <includes id="wrappers_8h" name="wrappers.h" local="yes" imported="no">app/wrappers.h</includes>
    <includes id="ioperm_8h" name="ioperm.h" local="yes" imported="no">app/ioperm.h</includes>
    <includes id="protection_8h" name="protection.h" local="yes" imported="no">memory/protection.h</includes>
    <includes id="callbacks_8h" name="callbacks.h" local="yes" imported="no">app/callbacks.h</includes>
    <includes id="irq__han_8h" name="irq_han.h" local="yes" imported="no">hal/irq_han.h</includes>
    <includes id="8259_8h" name="8259.h" local="yes" imported="no">hal/8259.h</includes>
    <member kind="function">
      <type>void</type>
      <name>SCTRegisterKernelFunctions</name>
      <anchorfile>usermodefuncs_8c.html</anchorfile>
      <anchor>3d0e8ac35f3ce90fe688154ecf097f36</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>usermodefuncs_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>selNewTask</name>
      <anchorfile>usermodefuncs_8c.html</anchorfile>
      <anchor>b4446be0816ed744339d5721a1886a2b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>appearance.c</name>
    <path>/home/stefan/src/ray/ray/src/appearance/</path>
    <filename>appearance_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="video__char_8h" name="video_char.h" local="yes" imported="no">video/video_char.h</includes>
    <includes id="appearance_8h" name="appearance.h" local="yes" imported="no">appearance/appearance.h</includes>
    <includes id="version_8h" name="version.h" local="yes" imported="no">version.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelVideoInit</name>
      <anchorfile>appearance_8c.html</anchorfile>
      <anchor>3974c46bc00750b44677425d233753d9</anchor>
      <arglist>(unsigned long memupper)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cmdline.c</name>
    <path>/home/stefan/src/ray/ray/src/</path>
    <filename>cmdline_8c</filename>
    <includes id="cmdline_8h" name="cmdline.h" local="yes" imported="no">cmdline.h</includes>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="strings_8h" name="strings.h" local="yes" imported="no">memory/strings/strings.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelParseCommandline</name>
      <anchorfile>cmdline_8c.html</anchorfile>
      <anchor>93811afb21820bb087bc45d2fda1dc2b</anchor>
      <arglist>(char *cmdline)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>8259.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>8259_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="io_8h" name="io.h" local="yes" imported="no">hal/io.h</includes>
    <includes id="8259_8h" name="8259.h" local="yes" imported="no">hal/8259.h</includes>
    <includes id="8259__ctrl_8h" name="8259_ctrl.h" local="yes" imported="no">hal/8259_ctrl.h</includes>
    <includes id="irq__han_8h" name="irq_han.h" local="yes" imported="no">hal/irq_han.h</includes>
    <member kind="function">
      <type>void</type>
      <name>c8259InterruptDone</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>47aa3571bc82aa92bf5e9fd88add3723</anchor>
      <arglist>(UINT8 irq)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259Init</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>b0d20f1090694eb5469041e9f654e130</anchor>
      <arglist>(UINT8 low_start, UINT8 high_start)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Master</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>78ae85ee5ce6e29cbfd504bac8f91746</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Slave</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>4fab856caaa30cca838f9bee5deb7cee</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Master</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>737b8a3843bd93328d82fad5c5eaf175</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Slave</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>9604d7636a35c39f1e63f0b591bb9783</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259UnmaskIRQ</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>1208ee9a32e51e54d280894a2cf11feb</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259MaskIRQ</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>04b7f9521ce9b0870222548edd12c338</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Mode8086</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>89b2b9f8e38091b1b3cd6cf54af6e6d1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Mode8086</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>f94417231026afdd34e120869c1afbc6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259SetIRQmaskLO</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>f5fd3dc55067bf95674eb41969e81a38</anchor>
      <arglist>(UINT8 irqMask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259SetIRQmaskHI</name>
      <anchorfile>8259_8c.html</anchorfile>
      <anchor>4d2a4aef5ef5622c58d7477fb321a040</anchor>
      <arglist>(UINT8 irqMask)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>gdt.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>gdt_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="gdt_8h" name="gdt.h" local="yes" imported="no">hal/gdt.h</includes>
    <includes id="tss_8h" name="tss.h" local="yes" imported="no">hal/tss.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelInitGDT</name>
      <anchorfile>gdt_8c.html</anchorfile>
      <anchor>a11e63ca1f13c5a9d67bcfec8bf11854</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>idt.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>idt_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="idt_8h" name="idt.h" local="yes" imported="no">hal/idt.h</includes>
    <includes id="isr_8h" name="isr.h" local="yes" imported="no">hal/isr.h</includes>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <includes id="irq_8h" name="irq.h" local="yes" imported="no">hal/irq.h</includes>
    <includes id="8259_8h" name="8259.h" local="yes" imported="no">hal/8259.h</includes>
    <includes id="gdt_8h" name="gdt.h" local="yes" imported="no">hal/gdt.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelIDTRegister</name>
      <anchorfile>idt_8c.html</anchorfile>
      <anchor>668f22b4d439078cb873e3ba8e6b9bf6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irq_han.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>irq__han_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="8259_8h" name="8259.h" local="yes" imported="no">hal/8259.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="callbacks_8h" name="callbacks.h" local="yes" imported="no">app/callbacks.h</includes>
    <includes id="irq__han_8h" name="irq_han.h" local="yes" imported="no">hal/irq_han.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <member kind="function">
      <type>void</type>
      <name>IRQHandlerInit</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>e36b635d878b023f158b98e4fb3829cd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQHandler</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>88848e6295e91715d533e46007b755cc</anchor>
      <arglist>(INT_REG *regs)</arglist>
    </member>
    <member kind="function">
      <type>RAY_WAKEUP</type>
      <name>IRQRegisterHandler</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>82e8eaac04f3691f8cf13d9b888605df</anchor>
      <arglist>(UINT8 irqNum, SCHED_NODE *thread, UINT32 funcAddress)</arglist>
    </member>
    <member kind="function">
      <type>RAY_WAKEUP</type>
      <name>IRQUnregisterHandler</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>356790c09bb779e6b39df59a91f0b6fd</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>selNewTask</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>b4446be0816ed744339d5721a1886a2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalFuncAddr</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>db91000cde1becd50af66d0ade89382a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalCaller</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>0f6bccc4ba869f8ad467633e3f866bf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalArgument</name>
      <anchorfile>irq__han_8c.html</anchorfile>
      <anchor>ca3983b5f32a654faf7900040b13eaad</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>syscall.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>syscall_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="syscall_8h" name="syscall.h" local="yes" imported="no">hal/syscall.h</includes>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <member kind="function">
      <type>void</type>
      <name>SysCallInit</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>5c8cb05efe75848f64385c8975692622</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RegisterSCTFunction</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>69653b088dfc3948261d29ee17c300ff</anchor>
      <arglist>(UINT32 kernelFuncNum, UINT8 minLevel, POINTER funcAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DeleteSCTFunction</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>bb0b9531c5f941fb4f3906a62f7ae4fa</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>GetSCTFunctionAddress</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>0a67817d0c9dcabed0e326fa2b7e50a5</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
    <member kind="function">
      <type>UINT8</type>
      <name>GetSCTminPrivilege</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>a724b7b5a1b6051e7006abcfffc63218</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>syscall_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>timer.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>timer_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="timer_8h" name="timer.h" local="yes" imported="no">hal/timer.h</includes>
    <includes id="timer__ctrl_8h" name="timer_ctrl.h" local="yes" imported="no">hal/timer_ctrl.h</includes>
    <includes id="io_8h" name="io.h" local="yes" imported="no">hal/io.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelTimerSetup</name>
      <anchorfile>timer_8c.html</anchorfile>
      <anchor>e5d366fe49ce6473fab423dd0ee199f3</anchor>
      <arglist>(UINT8 timer, UINT16 intHz)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tss.c</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>tss_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="tss_8h" name="tss.h" local="yes" imported="no">hal/tss.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="gdt_8h" name="gdt.h" local="yes" imported="no">hal/gdt.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="ioperm_8h" name="ioperm.h" local="yes" imported="no">app/ioperm.h</includes>
    <member kind="function">
      <type>TSS *</type>
      <name>CreateTSS</name>
      <anchorfile>tss_8c.html</anchorfile>
      <anchor>a46e6900bb1875e6b70631ab964abbd7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetupTSS</name>
      <anchorfile>tss_8c.html</anchorfile>
      <anchor>d167be0abe39504104e5c3c48a90a658</anchor>
      <arglist>(TSS *currTSS)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>kernelThread</name>
      <anchorfile>tss_8c.html</anchorfile>
      <anchor>24809b72298dcb80e6153601bea18530</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>callbacks.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>callbacks_8h</filename>
  </compound>
  <compound kind="file">
    <name>ioperm.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>ioperm_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>CreateThreadIOPermission</name>
      <anchorfile>ioperm_8h.html</anchorfile>
      <anchor>9a1bf3065d9fa6f6d8b15bcce5fb156e</anchor>
      <arglist>(MEM_MANAGE *userMemory)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>OpenIOPort</name>
      <anchorfile>ioperm_8h.html</anchorfile>
      <anchor>5e58c118715299e0b023243e1962056b</anchor>
      <arglist>(UINT16 port)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rmi.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>rmi_8h</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <class kind="struct">RMETHOD</class>
    <member kind="typedef">
      <type>UINT32</type>
      <name>RMISERIAL</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>5261b7cb2e9f9010ab7fc97216ff971d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>UINT32</type>
      <name>RMIMESSAGE</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>a6fa69028258b133d4d20f91fd01a456</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>UINT32</type>
      <name>RMIFUNCTION</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>b58ea736000212a672834030852324ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>RAY_RMI</name>
      <anchor>11f77591e560f740fc0ad84b10b18107</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_SUCCESS</name>
      <anchor>11f77591e560f740fc0ad84b10b181073595e8a8fa16ec1a1c92e4ddcfe2147d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_EXPORT_NOT_FOUND</name>
      <anchor>11f77591e560f740fc0ad84b10b18107f5a1f3a9cbfd79080e429b09ef9b1311</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_TRANSMIT_ERROR</name>
      <anchor>11f77591e560f740fc0ad84b10b1810700997d2b93f44a81fab6978be654094f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_GEN_ERROR</name>
      <anchor>11f77591e560f740fc0ad84b10b1810702497a3bad6218befff8a69813dcbd3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_WRONG_SETUP</name>
      <anchor>11f77591e560f740fc0ad84b10b18107e7f189c07e18c69972da940067227cff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_NO_SETUP</name>
      <anchor>11f77591e560f740fc0ad84b10b1810701280f1e3742fb3f5b90147ed444175a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_NO_SERIAL</name>
      <anchor>11f77591e560f740fc0ad84b10b18107c0ae924ec78b4f473cbf0f60a3d21a95</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_INSUFF_RIGHTS</name>
      <anchor>11f77591e560f740fc0ad84b10b1810707e59eed9743feb58fffa9f3af768dec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_NOT_SUPPORTED</name>
      <anchor>11f77591e560f740fc0ad84b10b18107ce2b9748179642db640f5cd008d176a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_OCCUPIED</name>
      <anchor>11f77591e560f740fc0ad84b10b18107eeb1f20e3b699fb7a72352accb237f55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMI_OVERLOAD</name>
      <anchor>11f77591e560f740fc0ad84b10b181077aab3c9ebbd75ec573524bb71e148f8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>KernelInitRMI</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>17fae4733e590c0f05c287ddcc76ad47</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>RMIRegister</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>b36c793e08e0661684dc1488b49b5bfe</anchor>
      <arglist>(UINT32 exportUID, UINT32 *entryPoint, PRIVLEVEL minPriv, BOOL partnership)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>RMInvoke</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>3e5577d33b8e7af96141a070d54f400d</anchor>
      <arglist>(RMISERIAL remoteSerial, RMIFUNCTION funcExport, UINT32 value)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>RMISetup</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>832215f945f46b82196fd3091b1aad2e</anchor>
      <arglist>(RMISERIAL threadSerial, UINT32 count)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>AllocateMessageBuffer</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>7a06dcb15c7baa4ebc1ac7e3bea3dab3</anchor>
      <arglist>(UINT32 msgSize, MSG_TYPE type)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>FreeMessageBuffer</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>b38091bdc9d728d1190e6fac9c10c74a</anchor>
      <arglist>(UINT32 msgBuffer)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SignalProcessMessage</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>cd47e53eaccfaa7695ce15b82430f449</anchor>
      <arglist>(RMISERIAL process, RMIFUNCTION funcExport, RMIMESSAGE message)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SendProcessMessage</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>ae817033762f57ff6f65908c7fe3aca5</anchor>
      <arglist>(RMISERIAL process, UINT32 msgMagic, RMIMESSAGE message)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>RegisterMsgReceiver</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>d4de57cae313555844ddb35b787874e7</anchor>
      <arglist>(UINT32 messageMagic, MSG_TYPE messageType, UINT32 maxSize, RMIMESSAGE msgHandle, PRIVLEVEL minPriv, BOOL partnership)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>SendSignal</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>9731bbb4eb02eb2132d9c1224b336b4c</anchor>
      <arglist>(RMISERIAL process, UINT32 msgMagic, UINT32 value)</arglist>
    </member>
    <member kind="function">
      <type>RAY_MESSAGE</type>
      <name>RegisterSigReceiver</name>
      <anchorfile>rmi_8h.html</anchorfile>
      <anchor>0ae63478f938e0bcc3b1f272c20ee7a4</anchor>
      <arglist>(UINT32 messageMagic, UINT32 valueHandle, PRIVLEVEL minPriv, BOOL partnership)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sct.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>sct_8h</filename>
  </compound>
  <compound kind="file">
    <name>usermodefuncs.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>usermodefuncs_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>SCTRegisterKernelFunctions</name>
      <anchorfile>usermodefuncs_8h.html</anchorfile>
      <anchor>3d0e8ac35f3ce90fe688154ecf097f36</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wrappers.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>wrappers_8h</filename>
    <includes id="video__char_8h" name="video_char.h" local="yes" imported="no">video/video_char.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
  </compound>
  <compound kind="file">
    <name>appearance.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/appearance/</path>
    <filename>appearance_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>KernelVideoInit</name>
      <anchorfile>appearance_8h.html</anchorfile>
      <anchor>3974c46bc00750b44677425d233753d9</anchor>
      <arglist>(unsigned long memupper)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cmdline.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>cmdline_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>KernelParseCommandline</name>
      <anchorfile>cmdline_8h.html</anchorfile>
      <anchor>93811afb21820bb087bc45d2fda1dc2b</anchor>
      <arglist>(char *cmdline)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>debug.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>debug_8h</filename>
  </compound>
  <compound kind="file">
    <name>exceptions.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>exceptions_8h</filename>
    <includes id="irq__han_8h" name="irq_han.h" local="yes" imported="no">hal/irq_han.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelThrowException</name>
      <anchorfile>exceptions_8h.html</anchorfile>
      <anchor>68d1f90f74d0823767991043525ff2c5</anchor>
      <arglist>(char *message, UINT32 errNumber, UINT32 addInfo, UINT8 showFlags, INT_REG *regs)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>8259.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>8259_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>c8259InterruptDone</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>47aa3571bc82aa92bf5e9fd88add3723</anchor>
      <arglist>(UINT8 irq)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259Init</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>b0d20f1090694eb5469041e9f654e130</anchor>
      <arglist>(UINT8 low_start, UINT8 high_start)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Master</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>78ae85ee5ce6e29cbfd504bac8f91746</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Slave</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>4fab856caaa30cca838f9bee5deb7cee</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Master</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>737b8a3843bd93328d82fad5c5eaf175</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Slave</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>9604d7636a35c39f1e63f0b591bb9783</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259UnmaskIRQ</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>1208ee9a32e51e54d280894a2cf11feb</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259MaskIRQ</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>04b7f9521ce9b0870222548edd12c338</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_1_Mode8086</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>89b2b9f8e38091b1b3cd6cf54af6e6d1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259_2_Mode8086</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>f94417231026afdd34e120869c1afbc6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259SetIRQmaskLO</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>f5fd3dc55067bf95674eb41969e81a38</anchor>
      <arglist>(UINT8 irqMask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>c8259SetIRQmaskHI</name>
      <anchorfile>8259_8h.html</anchorfile>
      <anchor>4d2a4aef5ef5622c58d7477fb321a040</anchor>
      <arglist>(UINT8 irqMask)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>8259_ctrl.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>8259__ctrl_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>CTL_8259A_1</name>
      <anchorfile>8259__ctrl_8h.html</anchorfile>
      <anchor>c5254f8f1e2f87e332f7618150265c8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CTL_INIT</name>
      <anchorfile>8259__ctrl_8h.html</anchorfile>
      <anchor>b8f917543a0d3b07232ae243034debc9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>gdt.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>gdt_8h</filename>
    <class kind="struct">gdt_entry</class>
    <class kind="struct">gdt_ptr</class>
    <member kind="typedef">
      <type>gdt_entry</type>
      <name>GDT_ENTRY</name>
      <anchorfile>gdt_8h.html</anchorfile>
      <anchor>82af4dd5aa0c010ea37dbc05f71c3663</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>gdt_ptr</type>
      <name>GDT_PTR</name>
      <anchorfile>gdt_8h.html</anchorfile>
      <anchor>d5a267538ac3548f605f46974e128cf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>KernelInitGDT</name>
      <anchorfile>gdt_8h.html</anchorfile>
      <anchor>a11e63ca1f13c5a9d67bcfec8bf11854</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>idt.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>idt_8h</filename>
    <class kind="struct">__attribute__</class>
    <class kind="struct">__attribute__</class>
    <member kind="function">
      <type>void</type>
      <name>KernelIDTRegister</name>
      <anchorfile>idt_8h.html</anchorfile>
      <anchor>668f22b4d439078cb873e3ba8e6b9bf6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>int.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>int_8h</filename>
  </compound>
  <compound kind="file">
    <name>io.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>io_8h</filename>
  </compound>
  <compound kind="file">
    <name>irq.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>irq_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>IRQ_0</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>350e99f1223f23399b2367a02d0bb972</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_1</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>f13f755fbb6aaa554d5ece36a1724cce</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_2</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>03f00228a13ec3dc7b63f8fb80b04225</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_3</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>1ac8574399b0f53113d99a826bffda54</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_4</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>13364277b1257a82b2b086ea0db5f83d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_5</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>96d7a1e9ffb4a4a83ab5d9aa7c9ffe19</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_6</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>55c8f08d41792b4766ff8aa0b25a4af4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_7</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>968dc392bdccc2a8fd7b2a9f82ea57bc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_8</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>729ea13640b2b36d8306658886e450c0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_9</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>620ceabe2c9f4fe98430e932cb594afd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_10</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>9f7e1ed68a067a3b4de04b8cb5b878d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_11</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>a1a3fa3ebdf22197fbfd8217bec53d56</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_12</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>668aa52e1620c27bca5df2997312dcf1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_13</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>4f7db0df5215e1a7faeb4707930ae03d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_14</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>a6d945edad15281ba0debdd93548a00a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQ_15</name>
      <anchorfile>irq_8h.html</anchorfile>
      <anchor>cc454f94602b8233b52b890b5ebb0c62</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irq_han.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>irq__han_8h</filename>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="callbacks_8h" name="callbacks.h" local="yes" imported="no">app/callbacks.h</includes>
    <class kind="struct">REGISTERED_HANDLER</class>
    <class kind="struct">__attribute__</class>
    <member kind="function">
      <type>void</type>
      <name>IRQHandler</name>
      <anchorfile>irq__han_8h.html</anchorfile>
      <anchor>88848e6295e91715d533e46007b755cc</anchor>
      <arglist>(INT_REG *regs)</arglist>
    </member>
    <member kind="function">
      <type>RAY_WAKEUP</type>
      <name>IRQRegisterHandler</name>
      <anchorfile>irq__han_8h.html</anchorfile>
      <anchor>82e8eaac04f3691f8cf13d9b888605df</anchor>
      <arglist>(UINT8 irqNum, SCHED_NODE *thread, UINT32 funcAddress)</arglist>
    </member>
    <member kind="function">
      <type>RAY_WAKEUP</type>
      <name>IRQUnregisterHandler</name>
      <anchorfile>irq__han_8h.html</anchorfile>
      <anchor>356790c09bb779e6b39df59a91f0b6fd</anchor>
      <arglist>(UINT8 irqNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>IRQHandlerInit</name>
      <anchorfile>irq__han_8h.html</anchorfile>
      <anchor>e36b635d878b023f158b98e4fb3829cd</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>isr.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>isr_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_DivZero</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>cd4cfd7942b7768a072df1cc3a05ea6c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_Debug</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>9861b995701998a8c30fed668c43fe42</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_NonMaskableInterrupt</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>66617a21c051a8a9205ed79166722c1f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_Breakpoint</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>d9522b3aa77c44e3096500beeb221bbf</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_Overflow</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>1277af8227082ee90944624c0c6b3d39</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_OutOfBounds</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>ae700880b427c36bb538f4c9a7366bbc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_InvalidOpCode</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>666ec50893152716f3d555c9db1f38f0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_No387</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>1e354f705726a717cf31ab4baad1aa93</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_DoubleFault</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>8722001381626cdfa5c47727ddfe0a2e</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_387SegOverrun</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>8bd3eae1dd72368aeec74189d02d79df</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_BadTSS</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>a849831fa12e56df21834f6cd85f0bb5</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_SegmentNotPresent</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>5186945ce76f9c29c3d8710d32cc06ec</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_StackFault</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>6faaf936d069fe0b14ff9fbb2739b6c8</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_GeneralProtectionFault</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>9642b2c8a6e6abd9f9122b5d595d83cc</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_PageFault</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>58ceda10ba26ea0fe3c5e5ab795bba13</anchor>
      <arglist>(UINT32 err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_UnknownInt</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>73171c9bb56dd823a0e3699a1c6a30cd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_387Fault</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>2c979db26659f76ba8a076d76258e548</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_Alignment</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>bd7711aaf13bcbc25c787b906679c381</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_Machine</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>999872591090df2f812abd36cbe48335</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED19</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>6aea0b212c3ccb649717b60df4654701</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED20</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>8ab1ccb04f3a4aa84524f68a9b8f1a09</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED21</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>bbff845b2ac3f90860359a3800247f7c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED22</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>2920a63337c11ca4f0d0576ac4d0bf70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED23</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>04c0ac5c966bc1aea4ec20ebd90b06f7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED24</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>8226f93f3d693d7b8d430a61aeaa9f89</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED25</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>8bdcc1f17db191bfad4dc40aff09eb1f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED26</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>584deb85e8f91a292f50d6782a1a56d6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED27</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>dca7ace7a92e4835b2e5cd8adaa9502c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED28</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>c0ef5c87d5236f89b88f3bdac6d2d169</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED29</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>e8f27d56a54e1518ad89e2e80bfd91f4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED30</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>d5206d60323c3a226fb764ba6726cb28</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ISR_exc_RESERVED31</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>d3fe60503711c42807ce19cf412edc1c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SysCall</name>
      <anchorfile>isr_8h.html</anchorfile>
      <anchor>7967e317093523e82c8090b0b96055cc</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>processor.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>processor_8h</filename>
    <class kind="struct">CPU_REGISTERS</class>
    <member kind="typedef">
      <type>CPU_REGISTERS</type>
      <name>cpu_registers</name>
      <anchorfile>processor_8h.html</anchorfile>
      <anchor>159cc57fdc12c7ed3aa9c446791dd288</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>syscall.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>syscall_8h</filename>
    <class kind="struct">__attribute__</class>
    <member kind="function">
      <type>void</type>
      <name>SysCallInit</name>
      <anchorfile>syscall_8h.html</anchorfile>
      <anchor>5c8cb05efe75848f64385c8975692622</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RegisterSCTFunction</name>
      <anchorfile>syscall_8h.html</anchorfile>
      <anchor>69653b088dfc3948261d29ee17c300ff</anchor>
      <arglist>(UINT32 kernelFuncNum, UINT8 minLevel, POINTER funcAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DeleteSCTFunction</name>
      <anchorfile>syscall_8h.html</anchorfile>
      <anchor>bb0b9531c5f941fb4f3906a62f7ae4fa</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>GetSCTFunctionAddress</name>
      <anchorfile>syscall_8h.html</anchorfile>
      <anchor>0a67817d0c9dcabed0e326fa2b7e50a5</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
    <member kind="function">
      <type>UINT8</type>
      <name>GetSCTminPrivilege</name>
      <anchorfile>syscall_8h.html</anchorfile>
      <anchor>a724b7b5a1b6051e7006abcfffc63218</anchor>
      <arglist>(UINT32 kernelFuncNum)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>timer.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>timer_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>KernelTimerSetup</name>
      <anchorfile>timer_8h.html</anchorfile>
      <anchor>e5d366fe49ce6473fab423dd0ee199f3</anchor>
      <arglist>(UINT8 timer, UINT16 intHz)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>timer_ctrl.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>timer__ctrl_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>PIT_FREQU</name>
      <anchorfile>timer__ctrl_8h.html</anchorfile>
      <anchor>85deb09db107eb266b5d2a77a2d3f46a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIT_CTRL_PORT</name>
      <anchorfile>timer__ctrl_8h.html</anchorfile>
      <anchor>4783245711b65423e7e18962d1ee46c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIT_MODE_TERMINAL</name>
      <anchorfile>timer__ctrl_8h.html</anchorfile>
      <anchor>c1da1b206e7eb854c7c6735c3bdea7e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIT_USE_BCD</name>
      <anchorfile>timer__ctrl_8h.html</anchorfile>
      <anchor>a7e2c37bf0e748ae3e29cf13dc8a1754</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tss.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>tss_8h</filename>
    <class kind="struct">__attribute__</class>
    <member kind="function">
      <type>void</type>
      <name>SetupTSS</name>
      <anchorfile>tss_8h.html</anchorfile>
      <anchor>d167be0abe39504104e5c3c48a90a658</anchor>
      <arglist>(TSS *currTSS)</arglist>
    </member>
    <member kind="function">
      <type>TSS *</type>
      <name>CreateTSS</name>
      <anchorfile>tss_8h.html</anchorfile>
      <anchor>a46e6900bb1875e6b70631ab964abbd7</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>kernel.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>kernel_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>RAY_ENTRY</name>
      <anchorfile>kernel_8h.html</anchorfile>
      <anchor>56392244651e03d963dfa1cac77aa295</anchor>
      <arglist>(multiboot_info_t *mbi)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>kernel_private.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>kernel__private_8h</filename>
  </compound>
  <compound kind="file">
    <name>mathfuncs.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/math/</path>
    <filename>mathfuncs_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>MathSquareRoot</name>
      <anchorfile>mathfuncs_8h.html</anchorfile>
      <anchor>57d8c277d3041de856cd6c3641427963</anchor>
      <arglist>(int value)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hash.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>hash_8h</filename>
    <class kind="struct">__attribute__</class>
    <class kind="struct">HASH</class>
    <member kind="function">
      <type>void</type>
      <name>HashInit</name>
      <anchorfile>hash_8h.html</anchorfile>
      <anchor>366bf5ab2b6c28560a39fd32c678e1c6</anchor>
      <arglist>(HASH *hashTable, UINT32 entries)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HashInsert</name>
      <anchorfile>hash_8h.html</anchorfile>
      <anchor>e2aa5467695bff872843445a5d2f85c7</anchor>
      <arglist>(HASH *hashTable, UINT32 key, UINT32 value)</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>HashRetrieve</name>
      <anchorfile>hash_8h.html</anchorfile>
      <anchor>83f0f8ca75b90e6b17bdfec704371122</anchor>
      <arglist>(HASH *hashTable, UINT32 hashKey)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HashDelete</name>
      <anchorfile>hash_8h.html</anchorfile>
      <anchor>e8e6504af13d56223504813da0114c9c</anchor>
      <arglist>(HASH *hashTable, UINT32 hashKey)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mem_flat.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>mem__flat_8h</filename>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <class kind="struct">_M_ALLOC</class>
    <member kind="define">
      <type>#define</type>
      <name>M_ADDRESS_START</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>35b54f50e8d38e3dcd54bce2910da916</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_ALLOC_GRANUL</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>6b1efffd2d80122d0f0d5f14f9217130</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_M_ALLOC</type>
      <name>M_ALLOC</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>2502ca1130e10f8dbb5a6677818302bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>KernelInitFlatMem</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>8106edce141c02f73c1b32b5c248cbde</anchor>
      <arglist>(UINT32 memStart, UINT32 memLength)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>FlatMalloc</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>77b4c41a6db5cc6f6ace768cf501159b</anchor>
      <arglist>(UINT32 length)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>FlatFree</name>
      <anchorfile>mem__flat_8h.html</anchorfile>
      <anchor>4d0d0d3f037292f9cbd1f130d231dcc3</anchor>
      <arglist>(POINTER memDescriptor)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>memfunc.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>memfunc_8h</filename>
    <member kind="typedef">
      <type>void *</type>
      <name>POINTER</name>
      <anchorfile>memfunc_8h.html</anchorfile>
      <anchor>e51a81000f343b8ec43bca1f6a723d7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>memcpy</name>
      <anchorfile>memfunc_8h.html</anchorfile>
      <anchor>62014c323d9ce9a9e39fe58a1149cb84</anchor>
      <arglist>(POINTER dest, const POINTER src, UINT32 count)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>memset</name>
      <anchorfile>memfunc_8h.html</anchorfile>
      <anchor>b7d6a3fd72734268d7c3abbcbcc03bc5</anchor>
      <arglist>(POINTER dest, UINT8 c, UINT32 count)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>memsetw</name>
      <anchorfile>memfunc_8h.html</anchorfile>
      <anchor>b5022f56953d0aab8ad523a9c70ed882</anchor>
      <arglist>(POINTER dest, UINT16 c, UINT32 count)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>memmove</name>
      <anchorfile>memfunc_8h.html</anchorfile>
      <anchor>c4b8625a1c07db95ca036fbe5bf12c48</anchor>
      <arglist>(POINTER dst, const POINTER src, UINT32 count)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>memory.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>memory_8h</filename>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <member kind="function">
      <type>void</type>
      <name>PrepareKernelMAlloc</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>aa6b6539ea33d5b6ee13adb9460fee6b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>__attribute__</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>738d18723520e1260c055473304b24ba</anchor>
      <arglist>((malloc)) KMalloc(UINT32 size)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>KFree</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>069df69f8435f38e48281b3263b11300</anchor>
      <arglist>(POINTER address)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>UserReleaseKernelSpace</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>6ae6bf30ea876d6152228d77d121868a</anchor>
      <arglist>(UINT32 virtAddress, UINT32 pageCount)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>UserMapKernelSpace</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>074b8523ff4a39cfa2164e4c9050ed29</anchor>
      <arglist>(UINT32 virtAddress, UINT32 physAddress, UINT32 Attributes)</arglist>
    </member>
    <member kind="function">
      <type>UINT32 *</type>
      <name>OpenPhysMemory</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>1ecf6d826ebc283b4556e9c8096ed724</anchor>
      <arglist>(UINT32 *physAddr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>paging.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>paging_8h</filename>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <class kind="struct">MEM_MANAGE</class>
    <member kind="define">
      <type>#define</type>
      <name>PROC_ENABLE_PAGING</name>
      <anchorfile>paging_8h.html</anchorfile>
      <anchor>401341734fa8b3276ad1ea46e2861bcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>KernelInitMem</name>
      <anchorfile>paging_8h.html</anchorfile>
      <anchor>db36968ae165f16ce7e80cb884962f8b</anchor>
      <arglist>(UINT32 length)</arglist>
    </member>
    <member kind="function">
      <type>MEM_MANAGE *</type>
      <name>PrepareUserPaging</name>
      <anchorfile>paging_8h.html</anchorfile>
      <anchor>3116b5b969ce6908ab43ba7fceeddd3f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>protection.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>protection_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>SetupKernelProtection</name>
      <anchorfile>protection_8h.html</anchorfile>
      <anchor>e44a67a54145cc8251d325f2c171271d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RestoreKernelProtection</name>
      <anchorfile>protection_8h.html</anchorfile>
      <anchor>776a3656a6166d1389412e257937a1f6</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strings.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/strings/</path>
    <filename>strings_8h</filename>
    <member kind="function">
      <type>char *</type>
      <name>strcpy</name>
      <anchorfile>strings_8h.html</anchorfile>
      <anchor>7a82515b5d377be04817715c5465b647</anchor>
      <arglist>(char *dest, const char *src)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strncpy</name>
      <anchorfile>strings_8h.html</anchorfile>
      <anchor>dab0254785e38acafc969185715b657a</anchor>
      <arglist>(char *dest, const char *src, UINT32 num)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>strcmp</name>
      <anchorfile>strings_8h.html</anchorfile>
      <anchor>04ee58caa7e5071bb02f298a1921a5f4</anchor>
      <arglist>(const char *string1, const char *string2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>strlen</name>
      <anchorfile>strings_8h.html</anchorfile>
      <anchor>ce2e8f8b5db1119196eee36974d8b536</anchor>
      <arglist>(const char *string)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usermem.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>usermem_8h</filename>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <member kind="function">
      <type>POINTER</type>
      <name>__attribute__</name>
      <anchorfile>usermem_8h.html</anchorfile>
      <anchor>d5539758a651fdd7d015b94208baae84</anchor>
      <arglist>((malloc)) UserMalloc(MEM_MANAGE *usersMem</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>UserFree</name>
      <anchorfile>usermem_8h.html</anchorfile>
      <anchor>1ab4bc71acbd5e0cf3fd9042368797db</anchor>
      <arglist>(MEM_MANAGE *usersMem, POINTER address)</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>AllocToVirt</name>
      <anchorfile>usermem_8h.html</anchorfile>
      <anchor>f517431f5f23f22b8d5a224b957a6243</anchor>
      <arglist>(MEM_MANAGE *userMemory, UINT32 virtAddress, UINT32 length, UINT32 attributes)</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>MemoryGift</name>
      <anchorfile>usermem_8h.html</anchorfile>
      <anchor>25ffde5e3922b33e3f539af04fc68de5</anchor>
      <arglist>(UINT32 virtAddress, SCHED_NODE *fromThread, SCHED_NODE *toThread, UINT32 newAttributes)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>elf.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/modules/</path>
    <filename>elf_8h</filename>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <class kind="struct">__attribute__</class>
    <class kind="struct">__attribute__</class>
    <member kind="function">
      <type>UINT32</type>
      <name>elfRelocationInit</name>
      <anchorfile>elf_8h.html</anchorfile>
      <anchor>550e976f678bd9db5c7c0010823d046e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>elf386LoadAndRelocateModule</name>
      <anchorfile>elf_8h.html</anchorfile>
      <anchor>8087b18400d1f5cc0461e9fd4214a89d</anchor>
      <arglist>(char *image, UINT32 size, SCHED_NODE *child)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>elf386DoRelocation</name>
      <anchorfile>elf_8h.html</anchorfile>
      <anchor>f55e81771ae9a1ab284e8ffa9bdbff01</anchor>
      <arglist>(char *image, elf386Section *section, elf386Relocation *relocation, SCHED_NODE *thread)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>elf_relocate.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/modules/</path>
    <filename>elf__relocate_8h</filename>
    <class kind="struct">__attribute__</class>
    <class kind="struct">__attribute__</class>
    <class kind="struct">__attribute__</class>
  </compound>
  <compound kind="file">
    <name>loadmodule.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/modules/</path>
    <filename>loadmodule_8h</filename>
  </compound>
  <compound kind="file">
    <name>mutex.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>mutex_8h</filename>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelMutexInit</name>
      <anchorfile>mutex_8h.html</anchorfile>
      <anchor>ca69c117345db339351b379648648e39</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>KernelMutexAcquire</name>
      <anchorfile>mutex_8h.html</anchorfile>
      <anchor>4223a0bf51304d7b4de45c2df478dc50</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
    <member kind="function">
      <type>MUTEX_STATE</type>
      <name>KernelMutexRelease</name>
      <anchorfile>mutex_8h.html</anchorfile>
      <anchor>422f20331ff31221bbb1d4e43859119d</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
    <member kind="function">
      <type>MUTEX_STATE</type>
      <name>KernelMutexStatus</name>
      <anchorfile>mutex_8h.html</anchorfile>
      <anchor>8ece524aabcf2fe21930a6ca1f3c076c</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mutex_types.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>mutex__types_8h</filename>
    <member kind="typedef">
      <type>unsigned long</type>
      <name>MUTEX</name>
      <anchorfile>mutex__types_8h.html</anchorfile>
      <anchor>9cefbc152797f07fc9c27de1bcea499d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scheduler.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>scheduler_8h</filename>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <member kind="function">
      <type>SCHED_NODE *</type>
      <name>SchedulerNextTask</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>cca9f65c62c95a67df4ddc3e88281a8d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Schedule</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>3f4dc5fcea4efe91c75e4760d804828e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerSelectThread</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>096ad9b9f6910e50a67ddfaeec7e1e02</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerInsertNode</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>622507361357e9b5b108efef8682b072</anchor>
      <arglist>(SCHED_NODE *newTask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRemoveNode</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>cef6e5f0fc226c052bc48fccede119e2</anchor>
      <arglist>(SCHED_NODE *taskNode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerInit</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>c10e2415498c3aef7fb7b549547919ac</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerSleepThread</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>60f6354519cd71a288068759b6c2bf44</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRunThread</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>54b6732037873905d2946aa077954f91</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRelinquish</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>f6251e0842a535481d8c95a0b2535592</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerDontInterrupt</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>4c1bc8894bf4d8d28f17ad10ad4314fd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerContinue</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>ae8a8870ea96ead8bd54b96bd972155f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>SchedulerSignalThread</name>
      <anchorfile>scheduler_8h.html</anchorfile>
      <anchor>35619b247d81db0d5fb347f4bb6510f8</anchor>
      <arglist>(SCHED_NODE *remoteThread, UINT32 *functionAddress, UINT32 sender, UINT32 argument)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>task_types.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>task__types_8h</filename>
    <includes id="rmi_8h" name="rmi.h" local="yes" imported="no">app/rmi.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <class kind="struct">schedNode</class>
  </compound>
  <compound kind="file">
    <name>thread.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>thread_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>SpawnThread</name>
      <anchorfile>thread_8h.html</anchorfile>
      <anchor>61f13fcdb84300a0d07ed2dcfa78f101</anchor>
      <arglist>(SCHED_NODE *newThread, POINTER functionAddress, SINT8 priority)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CreateKernelThread</name>
      <anchorfile>thread_8h.html</anchorfile>
      <anchor>c0f9e6bdebf01eeeea7091967d81108c</anchor>
      <arglist>(SINT8 priority)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>multiboot.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>multiboot_8h</filename>
    <class kind="struct">multiboot_header</class>
    <class kind="struct">aout_symbol_table</class>
    <class kind="struct">elf_section_header_table</class>
    <class kind="struct">multiboot_info</class>
    <class kind="struct">module</class>
    <class kind="struct">memory_map</class>
  </compound>
  <compound kind="file">
    <name>typedefs.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>typedefs_8h</filename>
  </compound>
  <compound kind="file">
    <name>version.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>version_8h</filename>
  </compound>
  <compound kind="file">
    <name>video_char.h</name>
    <path>/home/stefan/src/ray/ray/src/Include/video/</path>
    <filename>video__char_8h</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>COLUMNS</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>06c6c391fc11d106e9909f0401b255b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LINES</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>321ae946de24c36489276616d13c46cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VGA_TEXT_BASE</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>d6f7cb152057c7b77724d330ac731e55</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoInit</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>cc38a3dbbd69cb2944f24c4cb1fdea9e</anchor>
      <arglist>(UINT32 address)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoClear</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>5547402816c7a440c531a45d41ce303d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoSetFCol</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>91b574715e77119a07ae7de32f5bda10</anchor>
      <arglist>(UINT8 color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoSetBCol</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>9ca4377bcb10f939f2b2b37e9cf21756</anchor>
      <arglist>(UINT8 color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoGotoX</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>3e9529ce8e1dca3c1dec9d071f24bb95</anchor>
      <arglist>(UINT16 x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoGotoY</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>d877f7539359084bd5b572c4673e708a</anchor>
      <arglist>(UINT16 y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printf</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>133c04c35a1c14c6f8d8078831705661</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoFillLine</name>
      <anchorfile>video__char_8h.html</anchorfile>
      <anchor>9aaf76f2dba60a5857ecc6564ccd22b2</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>kernel.c</name>
    <path>/home/stefan/src/ray/ray/src/</path>
    <filename>kernel_8c</filename>
    <includes id="multiboot_8h" name="multiboot.h" local="yes" imported="no">multiboot.h</includes>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="video__char_8h" name="video_char.h" local="yes" imported="no">video/video_char.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="gdt_8h" name="gdt.h" local="yes" imported="no">hal/gdt.h</includes>
    <includes id="idt_8h" name="idt.h" local="yes" imported="no">hal/idt.h</includes>
    <includes id="int_8h" name="int.h" local="yes" imported="no">hal/int.h</includes>
    <includes id="processor_8h" name="processor.h" local="yes" imported="no">hal/processor.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="kernel_8h" name="kernel.h" local="yes" imported="no">kernel.h</includes>
    <includes id="timer_8h" name="timer.h" local="yes" imported="no">hal/timer.h</includes>
    <includes id="rmi_8h" name="rmi.h" local="yes" imported="no">app/rmi.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="callbacks_8h" name="callbacks.h" local="yes" imported="no">app/callbacks.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="thread_8h" name="thread.h" local="yes" imported="no">mtask/thread.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="appearance_8h" name="appearance.h" local="yes" imported="no">appearance/appearance.h</includes>
    <includes id="cmdline_8h" name="cmdline.h" local="yes" imported="no">cmdline.h</includes>
    <includes id="kernel__private_8h" name="kernel_private.h" local="yes" imported="no">kernel_private.h</includes>
    <includes id="syscall_8h" name="syscall.h" local="yes" imported="no">hal/syscall.h</includes>
    <includes id="irq__han_8h" name="irq_han.h" local="yes" imported="no">hal/irq_han.h</includes>
    <includes id="usermodefuncs_8h" name="usermodefuncs.h" local="yes" imported="no">app/usermodefuncs.h</includes>
    <includes id="loadmodule_8h" name="loadmodule.h" local="yes" imported="no">modules/loadmodule.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelThrowException</name>
      <anchorfile>kernel_8c.html</anchorfile>
      <anchor>68d1f90f74d0823767991043525ff2c5</anchor>
      <arglist>(char *message, UINT32 errNumber, UINT32 addInfo, UINT8 showFlags, INT_REG *regs)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RAY_ENTRY</name>
      <anchorfile>kernel_8c.html</anchorfile>
      <anchor>56392244651e03d963dfa1cac77aa295</anchor>
      <arglist>(multiboot_info_t *mbi)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>kernel_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalFuncAddr</name>
      <anchorfile>kernel_8c.html</anchorfile>
      <anchor>db91000cde1becd50af66d0ade89382a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>userMemPointer</name>
      <anchorfile>kernel_8c.html</anchorfile>
      <anchor>3d833f53f55a85d9dd0745938e18f619</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mathfuncs.c</name>
    <path>/home/stefan/src/ray/ray/src/math/</path>
    <filename>mathfuncs_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
  </compound>
  <compound kind="file">
    <name>hash.c</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>hash_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="mathfuncs_8h" name="mathfuncs.h" local="yes" imported="no">math/mathfuncs.h</includes>
    <member kind="function">
      <type>void</type>
      <name>HashInit</name>
      <anchorfile>hash_8c.html</anchorfile>
      <anchor>366bf5ab2b6c28560a39fd32c678e1c6</anchor>
      <arglist>(HASH *hashTable, UINT32 entries)</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>HashRetrieve</name>
      <anchorfile>hash_8c.html</anchorfile>
      <anchor>83f0f8ca75b90e6b17bdfec704371122</anchor>
      <arglist>(HASH *hashTable, UINT32 hashKey)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HashDelete</name>
      <anchorfile>hash_8c.html</anchorfile>
      <anchor>e8e6504af13d56223504813da0114c9c</anchor>
      <arglist>(HASH *hashTable, UINT32 hashKey)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HashInsert</name>
      <anchorfile>hash_8c.html</anchorfile>
      <anchor>e2aa5467695bff872843445a5d2f85c7</anchor>
      <arglist>(HASH *hashTable, UINT32 key, UINT32 value)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>memory.c</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>memory_8c</filename>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" imported="no">debug.h</includes>
    <includes id="processor_8h" name="processor.h" local="yes" imported="no">hal/processor.h</includes>
    <member kind="function">
      <type>void</type>
      <name>PrepareKernelMAlloc</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>aa6b6539ea33d5b6ee13adb9460fee6b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>KFree</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>069df69f8435f38e48281b3263b11300</anchor>
      <arglist>(POINTER address)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>UserMapKernelSpace</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>074b8523ff4a39cfa2164e4c9050ed29</anchor>
      <arglist>(UINT32 virtAddress, UINT32 physAddress, UINT32 Attributes)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>UserReleaseKernelSpace</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>6ae6bf30ea876d6152228d77d121868a</anchor>
      <arglist>(UINT32 virtAddress, UINT32 pageCount)</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>UserFree</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>1ab4bc71acbd5e0cf3fd9042368797db</anchor>
      <arglist>(MEM_MANAGE *usersMem, POINTER address)</arglist>
    </member>
    <member kind="function">
      <type>UINT32 *</type>
      <name>OpenPhysMemory</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>1ecf6d826ebc283b4556e9c8096ed724</anchor>
      <arglist>(UINT32 *physAddr)</arglist>
    </member>
    <member kind="function">
      <type>MEM_MANAGE *</type>
      <name>PrepareUserPaging</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>3116b5b969ce6908ab43ba7fceeddd3f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>AllocToVirt</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>f517431f5f23f22b8d5a224b957a6243</anchor>
      <arglist>(MEM_MANAGE *userMemory, UINT32 virtAddress, UINT32 length, UINT32 attributes)</arglist>
    </member>
    <member kind="function">
      <type>UINT32</type>
      <name>MemoryGift</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>25ffde5e3922b33e3f539af04fc68de5</anchor>
      <arglist>(UINT32 virtAddress, SCHED_NODE *fromThread, SCHED_NODE *toThread, UINT32 newAttributes)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>kernelThread</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>24809b72298dcb80e6153601bea18530</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>paging.c</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>paging_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="processor_8h" name="processor.h" local="yes" imported="no">hal/processor.h</includes>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="gdt_8h" name="gdt.h" local="yes" imported="no">hal/gdt.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" imported="no">debug.h</includes>
    <includes id="video__char_8h" name="video_char.h" local="yes" imported="no">video/video_char.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelInitMem</name>
      <anchorfile>paging_8c.html</anchorfile>
      <anchor>eb15752f338fdb457ea5f1d540631edd</anchor>
      <arglist>(UINT32 memTotal)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>kernelThread</name>
      <anchorfile>paging_8c.html</anchorfile>
      <anchor>24809b72298dcb80e6153601bea18530</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>protection.c</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>protection_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="protection_8h" name="protection.h" local="yes" imported="no">memory/protection.h</includes>
    <member kind="function">
      <type>void</type>
      <name>SetupKernelProtection</name>
      <anchorfile>protection_8c.html</anchorfile>
      <anchor>e44a67a54145cc8251d325f2c171271d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RestoreKernelProtection</name>
      <anchorfile>protection_8c.html</anchorfile>
      <anchor>776a3656a6166d1389412e257937a1f6</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strings.c</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>strings_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="strings_8h" name="strings.h" local="yes" imported="no">memory/strings/strings.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>strcpy</name>
      <anchorfile>strings_8c.html</anchorfile>
      <anchor>7a82515b5d377be04817715c5465b647</anchor>
      <arglist>(char *dest, const char *src)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strncpy</name>
      <anchorfile>strings_8c.html</anchorfile>
      <anchor>dab0254785e38acafc969185715b657a</anchor>
      <arglist>(char *dest, const char *src, UINT32 num)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>strcmp</name>
      <anchorfile>strings_8c.html</anchorfile>
      <anchor>04ee58caa7e5071bb02f298a1921a5f4</anchor>
      <arglist>(const char *string1, const char *string2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>strlen</name>
      <anchorfile>strings_8c.html</anchorfile>
      <anchor>ce2e8f8b5db1119196eee36974d8b536</anchor>
      <arglist>(const char *string)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>elf_relocate.c</name>
    <path>/home/stefan/src/ray/ray/src/modules/</path>
    <filename>elf__relocate_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="strings_8h" name="strings.h" local="yes" imported="no">memory/strings/strings.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="elf__relocate_8h" name="elf_relocate.h" local="yes" imported="no">modules/elf_relocate.h</includes>
    <includes id="elf_8h" name="elf.h" local="yes" imported="no">modules/elf.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <member kind="function">
      <type>UINT32</type>
      <name>elfRelocationInit</name>
      <anchorfile>elf__relocate_8c.html</anchorfile>
      <anchor>550e976f678bd9db5c7c0010823d046e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>POINTER</type>
      <name>elf386LoadAndRelocateModule</name>
      <anchorfile>elf__relocate_8c.html</anchorfile>
      <anchor>8087b18400d1f5cc0461e9fd4214a89d</anchor>
      <arglist>(char *image, UINT32 size, SCHED_NODE *child)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>elf386DoRelocation</name>
      <anchorfile>elf__relocate_8c.html</anchorfile>
      <anchor>f55e81771ae9a1ab284e8ffa9bdbff01</anchor>
      <arglist>(char *image, elf386Section *section, elf386Relocation *relocation, SCHED_NODE *thread)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>elf__relocate_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>loadmodule.c</name>
    <path>/home/stefan/src/ray/ray/src/modules/</path>
    <filename>loadmodule_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="multiboot_8h" name="multiboot.h" local="yes" imported="no">multiboot.h</includes>
    <includes id="loadmodule_8h" name="loadmodule.h" local="yes" imported="no">modules/loadmodule.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="usermem_8h" name="usermem.h" local="yes" imported="no">memory/usermem.h</includes>
    <includes id="elf_8h" name="elf.h" local="yes" imported="no">modules/elf.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="thread_8h" name="thread.h" local="yes" imported="no">mtask/thread.h</includes>
    <includes id="strings_8h" name="strings.h" local="yes" imported="no">memory/strings/strings.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" imported="no">debug.h</includes>
    <includes id="ioperm_8h" name="ioperm.h" local="yes" imported="no">app/ioperm.h</includes>
  </compound>
  <compound kind="file">
    <name>mutex.c</name>
    <path>/home/stefan/src/ray/ray/src/mtask/</path>
    <filename>mutex_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="exceptions_8h" name="exceptions.h" local="yes" imported="no">exceptions.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <member kind="function">
      <type>void</type>
      <name>KernelMutexAcquire</name>
      <anchorfile>mutex_8c.html</anchorfile>
      <anchor>4223a0bf51304d7b4de45c2df478dc50</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
    <member kind="function">
      <type>MUTEX_STATE</type>
      <name>KernelMutexRelease</name>
      <anchorfile>mutex_8c.html</anchorfile>
      <anchor>422f20331ff31221bbb1d4e43859119d</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
    <member kind="function">
      <type>MUTEX_STATE</type>
      <name>KernelMutexStatus</name>
      <anchorfile>mutex_8c.html</anchorfile>
      <anchor>8ece524aabcf2fe21930a6ca1f3c076c</anchor>
      <arglist>(MUTEX *mutex)</arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>mutex_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>scheduler.c</name>
    <path>/home/stefan/src/ray/ray/src/mtask/</path>
    <filename>scheduler_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="tss_8h" name="tss.h" local="yes" imported="no">hal/tss.h</includes>
    <includes id="hash_8h" name="hash.h" local="yes" imported="no">memory/hash.h</includes>
    <includes id="paging_8h" name="paging.h" local="yes" imported="no">memory/paging.h</includes>
    <includes id="mutex__types_8h" name="mutex_types.h" local="yes" imported="no">mtask/mutex_types.h</includes>
    <includes id="task__types_8h" name="task_types.h" local="yes" imported="no">mtask/task_types.h</includes>
    <includes id="scheduler_8h" name="scheduler.h" local="yes" imported="no">mtask/scheduler.h</includes>
    <includes id="mutex_8h" name="mutex.h" local="yes" imported="no">mtask/mutex.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory/memory.h</includes>
    <includes id="mem__flat_8h" name="mem_flat.h" local="yes" imported="no">memory/mem_flat.h</includes>
    <includes id="processor_8h" name="processor.h" local="yes" imported="no">hal/processor.h</includes>
    <includes id="strings_8h" name="strings.h" local="yes" imported="no">memory/strings/strings.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" imported="no">debug.h</includes>
    <member kind="function">
      <type>void</type>
      <name>SchedulerInit</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>c10e2415498c3aef7fb7b549547919ac</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerDontInterrupt</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>4c1bc8894bf4d8d28f17ad10ad4314fd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerContinue</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>ae8a8870ea96ead8bd54b96bd972155f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>SCHED_NODE *</type>
      <name>SchedulerNextTask</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>cca9f65c62c95a67df4ddc3e88281a8d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>RAY_RMI</type>
      <name>SchedulerSignalThread</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>35619b247d81db0d5fb347f4bb6510f8</anchor>
      <arglist>(SCHED_NODE *remoteThread, UINT32 *functionAddress, UINT32 sender, UINT32 argument)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerInsertNode</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>622507361357e9b5b108efef8682b072</anchor>
      <arglist>(SCHED_NODE *newTask)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRemoveNode</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>cef6e5f0fc226c052bc48fccede119e2</anchor>
      <arglist>(SCHED_NODE *taskNode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Schedule</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>3f4dc5fcea4efe91c75e4760d804828e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerSelectThread</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>096ad9b9f6910e50a67ddfaeec7e1e02</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRunThread</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>54b6732037873905d2946aa077954f91</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerSleepThread</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>60f6354519cd71a288068759b6c2bf44</anchor>
      <arglist>(SCHED_NODE *thread)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SchedulerRelinquish</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>f6251e0842a535481d8c95a0b2535592</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>selNewTask</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>b4446be0816ed744339d5721a1886a2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalFuncAddr</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>db91000cde1becd50af66d0ade89382a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalCaller</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>0f6bccc4ba869f8ad467633e3f866bf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>jalArgument</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>ca3983b5f32a654faf7900040b13eaad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>userMemPointer</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>3d833f53f55a85d9dd0745938e18f619</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>userMemValue</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>31744b5b58bdde7ff3e465f7fd66da2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>kernelThread</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>24809b72298dcb80e6153601bea18530</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SCHED_NODE *</type>
      <name>currTask</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>c2498cfd09f80133d7e45e69123497c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>atomicWishes</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>fc569434ed3a0edc1a29dc944eff53f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>cESP</name>
      <anchorfile>scheduler_8c.html</anchorfile>
      <anchor>6ef91e432037e386a52fa9cffd167e2f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>video_char.c</name>
    <path>/home/stefan/src/ray/ray/src/video/</path>
    <filename>video__char_8c</filename>
    <includes id="typedefs_8h" name="typedefs.h" local="yes" imported="no">typedefs.h</includes>
    <includes id="video__char_8h" name="video_char.h" local="yes" imported="no">video/video_char.h</includes>
    <includes id="memfunc_8h" name="memfunc.h" local="yes" imported="no">memory/memfunc.h</includes>
    <includes id="io_8h" name="io.h" local="yes" imported="no">hal/io.h</includes>
    <member kind="function">
      <type>void</type>
      <name>VideoSetFCol</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>91b574715e77119a07ae7de32f5bda10</anchor>
      <arglist>(UINT8 color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoSetBCol</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>9ca4377bcb10f939f2b2b37e9cf21756</anchor>
      <arglist>(UINT8 color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoGotoX</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>3e9529ce8e1dca3c1dec9d071f24bb95</anchor>
      <arglist>(UINT16 x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoGotoY</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>d877f7539359084bd5b572c4673e708a</anchor>
      <arglist>(UINT16 y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoInit</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>cc38a3dbbd69cb2944f24c4cb1fdea9e</anchor>
      <arglist>(UINT32 address)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoClear</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>5547402816c7a440c531a45d41ce303d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printf</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>133c04c35a1c14c6f8d8078831705661</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>VideoFillLine</name>
      <anchorfile>video__char_8c.html</anchorfile>
      <anchor>9aaf76f2dba60a5857ecc6564ccd22b2</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>__attribute__</name>
    <filename>struct____attribute____.html</filename>
    <member kind="variable">
      <type>UINT16</type>
      <name>baseLow</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>ddd057f4c4eac5abf18ee98c35659cdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT16</type>
      <name>sel</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>dd41a46bf87d1c1995084388f1024a39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>always0</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>21c8eaf96c39720255063d839bf77f3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>flags</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>6bf90a7acdbb6af5d7614f61c91b9a5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT16</type>
      <name>baseHigh</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>f1a7a489c471b4194761930d84aa58ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT16</type>
      <name>limit</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>0d2e1d5a8acdef76b83b8dd6ac9aaa9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>base</name>
      <anchorfile>struct____attribute____.html</anchorfile>
      <anchor>4e0ba65769a3c5a6f0c056520c385bc4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>_M_ALLOC</name>
    <filename>struct__M__ALLOC.html</filename>
    <member kind="variable">
      <type>BOOL</type>
      <name>used</name>
      <anchorfile>struct__M__ALLOC.html</anchorfile>
      <anchor>8b6b596b5213c8d247c058bce89528dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>length</name>
      <anchorfile>struct__M__ALLOC.html</anchorfile>
      <anchor>3f56d7e1fb6d1957636760f64233a2c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_M_ALLOC *</type>
      <name>self</name>
      <anchorfile>struct__M__ALLOC.html</anchorfile>
      <anchor>990cbbdefba7a4f6cf35d09794e2c64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_M_ALLOC *</type>
      <name>prev</name>
      <anchorfile>struct__M__ALLOC.html</anchorfile>
      <anchor>2dfab4cb764c036cf23ce6e4176e5303</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_M_ALLOC *</type>
      <name>next</name>
      <anchorfile>struct__M__ALLOC.html</anchorfile>
      <anchor>4e42a2dc41ef1ac3808b80f4855a2ef4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CPU_REGISTERS</name>
    <filename>structCPU__REGISTERS.html</filename>
  </compound>
  <compound kind="struct">
    <name>gdt_entry</name>
    <filename>structgdt__entry.html</filename>
    <member kind="variable">
      <type>UINT16</type>
      <name>limitLow</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>e14536cd5ee1c376e78ff5cd21c7c242</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT16</type>
      <name>baseLow</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>18795bc2650ca84658bd7470cadac56a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>baseMiddle</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>3b7ae65cf58b55ee5a15fb87594ee048</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>access</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>281a6cc2b0bde0b60fa0d37b1893c007</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>granularity</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>3822319a128020810db312ddd8761df3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT8</type>
      <name>baseHigh</name>
      <anchorfile>structgdt__entry.html</anchorfile>
      <anchor>ecdae3882332563c9ac60e7fbdef65e3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gdt_ptr</name>
    <filename>structgdt__ptr.html</filename>
    <member kind="variable">
      <type>UINT16</type>
      <name>limit</name>
      <anchorfile>structgdt__ptr.html</anchorfile>
      <anchor>beaac06c2d065b4f43b38f6d112310ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32</type>
      <name>base</name>
      <anchorfile>structgdt__ptr.html</anchorfile>
      <anchor>6658ab62cfa5a1b5c920ba1a2ba496ab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MEM_MANAGE</name>
    <filename>structMEM__MANAGE.html</filename>
    <member kind="variable">
      <type>UINT32 *</type>
      <name>pageDirVirtAddress</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>8a2fa24a9b9b3bff0c19be343e9bb858</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32 *</type>
      <name>pageDirPhysAddress</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>fed9cdd352456cc1e55a4300e4b6adc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32 *</type>
      <name>physChannel</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>76d70b800684426f106b2bf656419e05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32 *</type>
      <name>alterTableVirtAddress</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>31a0e12cbe7fd2779f0adfb1a6854f5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UINT32 *</type>
      <name>alterTablePhysAddress</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>c3bb2776a1aa29155a21ff77b98b92ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>HASH *</type>
      <name>memAllocs</name>
      <anchorfile>structMEM__MANAGE.html</anchorfile>
      <anchor>123bc11cffd1f0f8d6e434c5afc5ae62</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/app/</name>
    <path>/home/stefan/src/ray/ray/src/Include/app/</path>
    <filename>dir_24f5c83a85b82a32c8a80433af6c7b3d.html</filename>
    <file>callbacks.h</file>
    <file>ioperm.h</file>
    <file>rmi.h</file>
    <file>sct.h</file>
    <file>usermodefuncs.h</file>
    <file>wrappers.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/app/</name>
    <path>/home/stefan/src/ray/ray/src/app/</path>
    <filename>dir_b434a135b7248bfa306621a02bd395ec.html</filename>
    <file>ioperm.c</file>
    <file>rmi.c</file>
    <file>usermodefuncs.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/appearance/</name>
    <path>/home/stefan/src/ray/ray/src/Include/appearance/</path>
    <filename>dir_75a21fe8a3ad767836be5fde967f259c.html</filename>
    <file>appearance.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/appearance/</name>
    <path>/home/stefan/src/ray/ray/src/appearance/</path>
    <filename>dir_62aeb36ba470b819f3ce8d5305080ca7.html</filename>
    <file>appearance.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/c/</name>
    <path>/home/stefan/src/ray/ray/src/Include/c/</path>
    <filename>dir_d5b912f5efe0bfae6761cbf65d416cfe.html</filename>
    <file>stdarg.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/save/hal/</name>
    <path>/home/stefan/src/ray/ray/src/save/hal/</path>
    <filename>dir_16630282208223cef46af20ef4b3ed61.html</filename>
    <file>irq.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/hal/</name>
    <path>/home/stefan/src/ray/ray/src/Include/hal/</path>
    <filename>dir_9de5cd524f3e9d159bee3d6ff7c7213c.html</filename>
    <file>8259.h</file>
    <file>8259_ctrl.h</file>
    <file>gdt.h</file>
    <file>idt.h</file>
    <file>int.h</file>
    <file>io.h</file>
    <file>irq.h</file>
    <file>irq_han.h</file>
    <file>isr.h</file>
    <file>processor.h</file>
    <file>syscall.h</file>
    <file>timer.h</file>
    <file>timer_ctrl.h</file>
    <file>tss.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/hal/</name>
    <path>/home/stefan/src/ray/ray/src/hal/</path>
    <filename>dir_5ca09bbc12ce4712b1627d4258254ec6.html</filename>
    <file>8259.c</file>
    <file>gdt.c</file>
    <file>idt.c</file>
    <file>io.c</file>
    <file>irq_han.c</file>
    <file>isr.c</file>
    <file>syscall.c</file>
    <file>timer.c</file>
    <file>tss.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/</name>
    <path>/home/stefan/src/ray/ray/src/Include/</path>
    <filename>dir_73a6e912de387fe087ae377ccb8c6d6d.html</filename>
    <dir>/home/stefan/src/ray/ray/src/Include/app/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/appearance/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/c/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/hal/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/math/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/memory/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/modules/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/mtask/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/video/</dir>
    <file>cmdline.h</file>
    <file>debug.h</file>
    <file>exceptions.h</file>
    <file>kernel.h</file>
    <file>kernel_private.h</file>
    <file>multiboot.h</file>
    <file>typedefs.h</file>
    <file>version.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/math/</name>
    <path>/home/stefan/src/ray/ray/src/math/</path>
    <filename>dir_1a1c2c1a81968ff2b4c839b79d0d9638.html</filename>
    <file>mathfuncs.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/math/</name>
    <path>/home/stefan/src/ray/ray/src/Include/math/</path>
    <filename>dir_22a0c7a3d75fc5a2813bb412bd8fd454.html</filename>
    <file>mathfuncs.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/memory/</name>
    <path>/home/stefan/src/ray/ray/src/memory/</path>
    <filename>dir_07cf2dd6cf083bfffa6f5409803f662f.html</filename>
    <file>hash.c</file>
    <file>mem_flat.c</file>
    <file>memfunc.c</file>
    <file>memory.c</file>
    <file>paging.c</file>
    <file>protection.c</file>
    <file>strings.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/memory/</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/</path>
    <filename>dir_05a316b2e2f20d587ce98a405f401e98.html</filename>
    <dir>/home/stefan/src/ray/ray/src/Include/memory/strings/</dir>
    <file>hash.h</file>
    <file>mem_flat.h</file>
    <file>memfunc.h</file>
    <file>memory.h</file>
    <file>paging.h</file>
    <file>protection.h</file>
    <file>usermem.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/modules/</name>
    <path>/home/stefan/src/ray/ray/src/modules/</path>
    <filename>dir_fa0249341e2d7d0eedf15e266bf03479.html</filename>
    <file>elf_relocate.c</file>
    <file>loadmodule.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/modules/</name>
    <path>/home/stefan/src/ray/ray/src/Include/modules/</path>
    <filename>dir_d5aad37831dd20d62cb3e3e867538f03.html</filename>
    <file>elf.h</file>
    <file>elf_relocate.h</file>
    <file>loadmodule.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/mtask/</name>
    <path>/home/stefan/src/ray/ray/src/mtask/</path>
    <filename>dir_82c671ac356ed5a939949d33515db6c2.html</filename>
    <file>mutex.c</file>
    <file>scheduler.c</file>
    <file>thread.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/mtask/</name>
    <path>/home/stefan/src/ray/ray/src/Include/mtask/</path>
    <filename>dir_91f8e8327e6fbe040ed33671a4cda89d.html</filename>
    <file>mutex.h</file>
    <file>mutex_types.h</file>
    <file>scheduler.h</file>
    <file>task_types.h</file>
    <file>thread.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/</name>
    <path>/home/stefan/src/ray/ray/</path>
    <filename>dir_5cff6b02e0507ec4385fd34e6a4c1faa.html</filename>
    <dir>/home/stefan/src/ray/ray/src/</dir>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/save/</name>
    <path>/home/stefan/src/ray/ray/src/save/</path>
    <filename>dir_008f730bb695a726a2f1b4241dbdb53d.html</filename>
    <dir>/home/stefan/src/ray/ray/src/save/hal/</dir>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/</name>
    <path>/home/stefan/src/ray/ray/src/</path>
    <filename>dir_5e59c1e6927fd884640d5eadf5fdbb25.html</filename>
    <dir>/home/stefan/src/ray/ray/src/app/</dir>
    <dir>/home/stefan/src/ray/ray/src/appearance/</dir>
    <dir>/home/stefan/src/ray/ray/src/hal/</dir>
    <dir>/home/stefan/src/ray/ray/src/Include/</dir>
    <dir>/home/stefan/src/ray/ray/src/math/</dir>
    <dir>/home/stefan/src/ray/ray/src/memory/</dir>
    <dir>/home/stefan/src/ray/ray/src/modules/</dir>
    <dir>/home/stefan/src/ray/ray/src/mtask/</dir>
    <dir>/home/stefan/src/ray/ray/src/save/</dir>
    <dir>/home/stefan/src/ray/ray/src/video/</dir>
    <file>cmdline.c</file>
    <file>kernel.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/memory/strings/</name>
    <path>/home/stefan/src/ray/ray/src/Include/memory/strings/</path>
    <filename>dir_e2928957e2c1256100da2a7a87296add.html</filename>
    <file>strings.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/video/</name>
    <path>/home/stefan/src/ray/ray/src/video/</path>
    <filename>dir_474a886338db440940ef705d5903c04d.html</filename>
    <file>video_char.c</file>
  </compound>
  <compound kind="dir">
    <name>/home/stefan/src/ray/ray/src/Include/video/</name>
    <path>/home/stefan/src/ray/ray/src/Include/video/</path>
    <filename>dir_0e1c9c3ade6b1a4aee56ac98db90ebc1.html</filename>
    <file>video_char.h</file>
  </compound>
</tagfile>
