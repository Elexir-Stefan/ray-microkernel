/**
 * @file gdt.c
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief Global descritpor table - Initializes the GDT
 */


#include <typedefs.h>
#include <hal/gdt.h>
#include <hal/tss.h>
#include <memory/memory.h>
#include <memory/addresses.h>
#include <memory/mmu.h>

// external resources
extern UINT32 KERNEL_ADDR_VIRT_CODE_END;

static GDT_ENTRY gdt[NUM_GDTs];
static GDT_PTR gp;

// global
volatile TSS *kernelTSS;

static void GDTsetEntry(UINT32 num, UINT32 base, UINT32 limit, UINT8 access, UINT8 gran);
static void GDTrefresh(void);
static inline void LoadTSS(UINT32 offset);

DONTPROFILE static inline void GDTrefresh(void) {
	__asm__ __volatile__ ("lgdtl (%%eax)\n"
	                      "movw $0x10, %%ax\n"
	                      "movw %%ax, %%ds\n"
	                      "movw %%ax, %%es\n"
	                      "movw %%ax, %%fs\n"
	                      "movw %%ax, %%gs\n"
	                      "movw %%ax, %%ss\n"
	                      "jmp $0x08,$enableGDT\n"
	                      "enableGDT:\n"
	: :"a"(&gp));
	// TODO: 0x08 is not generic enough. Normally it would be KERNEL_SEG_CODE
	// TODO: 0x10 is not generic enough. Normally it would be KERNEL_SEG_DATA

	return;
}

DONTPROFILE static inline void LoadTSS(UINT32 offset) {
	offset *=  sizeof(GDT_ENTRY);
	__asm__ __volatile__ ("ltr %%ax" : : "a"(offset));
}

/**
 * set up a descriptor for the GDT
 */
DONTPROFILE static void GDTsetEntry(UINT32 num, UINT32 base, UINT32 limit, UINT8 access, UINT8 gran) {

	/* setup the descriptor base address */
	gdt[num].baseLow = (base & 0xFFFF);
	gdt[num].baseMiddle = (base >> 16) & 0xFF;
	gdt[num].baseHigh = (base >> 24) & 0xFF;

	/* descriptor limits */
	gdt[num].limitLow = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	/* granularity and access flags */
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void KernelInitGDT() {

	/* setup the GDT pointer and limit */
	gp.limit = (sizeof(GDT_ENTRY) * NUM_GDTs) - 1;
	gp.base = (UINT32)&gdt;

	UINT32 kernelCodeStart = 0x00000000;
	UINT32 kernelCodeEnd   = KERNEL_ADDR_VIRT_CODE_END;
	if (kernelCodeEnd % PAGE_SIZE) {
		// it's not aligned to the page boundary
		kernelCodeEnd += PAGE_SIZE - (kernelCodeEnd % PAGE_SIZE);
	}
	UINT32 kernelDataStart = 0x00000000;
	UINT32 kernelDataEnd   = 0xFFFFFFFF;

	UINT32 userCodeStart = VIRTUAL_USER_SPACE_START;
	UINT32 userCodeEnd   = VIRTUAL_USER_CODE_END;
	UINT32 userDataStart = VIRTUAL_USER_SPACE_START;
	UINT32 userDataEnd   = VIRTUAL_USER_SPACE_END;

	/* null-pointers will NOT be allowed! */
	GDTsetEntry(GDT_NULLPOINTER, 0, 0, 0, 0);

	/* set up the code segment */
	GDTsetEntry(GDT_CODE_RING0, kernelCodeStart, kernelCodeEnd >> 12, G_SEGMENT | G_RING0 | G_DESCR | G_CODE_S | G_READEXEC, G_GRAN4K | G_OPER32);

	/* set up the data segment */
	GDTsetEntry(GDT_DATA_RING0, kernelDataStart, kernelDataEnd >> 12, G_SEGMENT | G_RING0 | G_DESCR | G_DATA_S | G_READEXEC, G_GRAN4K | G_OPER32);

	/* set up the code segment */
	GDTsetEntry(GDT_CODE_RING3, userCodeStart, userCodeEnd >> 12, G_SEGMENT | G_RING3 | G_DESCR | G_CODE_S | G_READEXEC, G_GRAN4K | G_OPER32);

	/* set up the data segment */
	GDTsetEntry(GDT_DATA_RING3, userDataStart, userDataEnd >> 12, G_SEGMENT | G_RING3 | G_DESCR | G_DATA_S | G_READEXEC, G_GRAN4K | G_OPER32);

	kernelTSS = CreateTSS();

	/* 12288 - 1 byte 0xff @see 24593.pdf AMD64 Documentation Part 2
	("An extra byte must be present after the last IOPB byte. This byte must have
	all bits set to 1 (0FFh).") */
	GDTsetEntry(GDT_TSS, (UINT32)kernelTSS , 12287 - 1 , G_SEGMENT | G_RING0 | TSS_386_FREE, 0);

	GDTsetEntry(GDT_TIB, STATIC_TIB_VADDR, PAGE_SIZE, G_SEGMENT | G_RING3 | G_DESCR | G_DATA_S | G_READEXEC, G_OPER32);

	/* commit changes */
	GDTrefresh();
	LoadTSS(GDT_TSS);
}
