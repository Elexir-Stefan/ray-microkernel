OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
KERNEL_LOAD_ADDR = 0x00100000;
KERNEL_VIRTUAL_ADDRESS = 0xC0000000;

ENTRY(start)


SECTIONS
{
	. = (KERNEL_LOAD_ADDR + KERNEL_VIRTUAL_ADDRESS);

	.text ALIGN(4096) : AT(KERNEL_LOAD_ADDR)
	{
		__KERNEL_ADDR_VIRT_CODE_START = .;
		KEEP(*(.text.start))
		KEEP (*(.text*))
		*(.gnu.linkonce.t.*)
		*(*.rodata*)
		__KERNEL_ADDR_VIRT_CODE_END = .;

	}
	
	. = ALIGN(4096);
	__KERNEL_ADDR_VIRT_DATA_START = .;
	.data ALIGN(4096) : AT(ADDR(.data) - KERNEL_VIRTUAL_ADDRESS)
	{

		
		__CTOR_LIST = .;
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		__CTOR_END = .;
		
		__DTOR_LIST = .;
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		__DTOR_END__ = .;
		

		__EH_FRAME_BEGIN = .;
		KEEP (*(.eh_frame))
		QUAD(0)
		*(.gcc_except_table*)
		*(.got.plt)
		*(.gnu.linkonce.*)
		*(.jcr)
		*(.data*)
		__KERNEL_ADDR_VIRT_DATA_END = .;
		
	}

	
	.bss ALIGN(4096) : AT(ADDR(.bss) - KERNEL_VIRTUAL_ADDRESS)
	{
		__KERNEL_ADDR_VIRT_BSS_START = .;
		*(COMMON*)
		*(.bss*)
		__KERNEL_ADDR_VIRT_BSS_END = .;
	}
	

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
	Symbols in the DWARF debugging sections are relative to the beginning
	of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }	
}
