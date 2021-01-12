#ifndef _ELF_H
#define _ELF_H

#include "mtask/task_types.h"

/**
 * @file elf.h
 * @author Stefan Nuernberger
 * @date 09-14-2006
 * @brief Definitions needed to load elf binaries
 */

typedef struct {
	UINT32        sectionName;
	UINT32        sectionType;
	UINT32        sectionFlags;
	UINT32        sectionAddress;
	UINT32        sectionOffset;
	UINT32        sectionSize;
	UINT32        sectionLink;
	UINT32        sectionInfo;
	UINT32        sectionAlign;
	UINT32        sectionEntrySize;
} __attribute__ ((packed)) elf386Section;

typedef struct {
	UINT32        relocOffset;
	UINT32        relocInfo;
	SINT32        relocAddend;
} __attribute__ ((packed)) elf386Relocation;

UINT32 elfRelocationInit();
POINTER elf386LoadAndRelocateModule(char* image, UINT32 size, PROCESS_PTR child);
UINT32 elf386Relocate(char *image, PROCESS_PTR currThread);
void elf386DoRelocation(char *image, elf386Section *section, elf386Relocation *relocation, PROCESS_PTR thread);
UINT32 elf386GetSymbol(char *image, UINT32 symbolTable, UINT32 symbolID);
POINTER elf386GetSymbolOffset(char *image, char *name);

#define SECTION_TYPE_BSS	0x08

#endif
