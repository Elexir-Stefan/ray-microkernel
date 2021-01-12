/**
 * @file elf.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Sa Dez 22 2007
 * @see elf.h
 * @brief Summary
 */

extern "C" {
	#include <typedefs.h>
	#include <modules/elf_types.h>
	#include <memory/mmu.h>
	#include <memory/addresses.h>
	#include <memory/memory.h>
	#include <memory/strings/strings.h>
	#include <exceptions.h>
	#include <memory/usermem.h>
	#include <debug.h>
}
#include <modules/elf32.h>
#include <modules/load_types.h>
#include <modules/loadexecutable.h>
#include <cpp/specialmem.h>

using namespace Executables;

ElfFormatException::ElfFormatException(const ElfFailureReason& reason) :
	_reason(reason)
{

}

const ElfFormatException::ElfFailureReason& ElfFormatException::GetReason()
{
	return _reason;
}


/*** Elf32 **/

bool Elf32::IsELF32(UINT8 *image, UINT32 length) {

	// cannot be possible if file is smaller than header
	if (length < sizeof(Elf32_Ehdr)) {
		ASSERT(FALSE);
		throw new ElfFormatException(ElfFormatException::InvalidFormat);
		return false;
	}

	Elf32_Ehdr* elf_header = (Elf32_Ehdr*) image;

	// check for valid ELF header
	if (elf_header->e_magic != ELF_MAGIC) {
		ASSERT(FALSE);
		throw new ElfFormatException(ElfFormatException::InvalidFormat);
		return false;
	}

	// check for 32 bit elf file
	if (elf_header->e_ident[EI_CLASS] != ELFCLASS32) {
		ASSERT(FALSE);
		throw new ElfFormatException(ElfFormatException::InvalidFormat);
		return false;
	}

	return true;
}

UINT16 Elf32::GetNumLoadableSegments() {
	UINT16 count = 0;

	UINT32 i;
	for (i = 0; i < _programHeaders.Size; i++) {
		// if loadable, only
		if ((_programHeaders[i].p_type == PT_LOAD) && (_programHeaders[i].p_memsz > 0)) {
			count++;
		}
	}

	return count;
}

/**
 * Called, when program header item is loadable:
 * Takes the ELF32 specific data and converts it to LOAD_MEM_SEGMENT information with
 * according memory attributes (read/write/execute) and copies the appropriate section
 * from the ELF image to a newly allocated buffer.
 * This function takes care of padding as well (for .bss for example)
 * @param image Linear byte image of ELF32 file
 * @param program_header Current program header (must be loadable!)
 * @param currSegment Segment to convert to
 */
void Elf32::ConvertToSegment(Elf32_Phdr& programHeader, LOAD_MEM_SEGMENT* currSegment) {
					// check attributes
					currSegment->memoryAttributes  = 0;
					if (programHeader.p_flags & PF_W) {
						currSegment->memoryAttributes  |= MEM_ATTRIB_WRITABLE;
					}

					if (programHeader.p_flags & PF_R) {
						currSegment->memoryAttributes  |= MEM_ATTRIB_READABLE;
					}

					if (programHeader.p_flags & PF_X) {
						currSegment->memoryAttributes  |= MEM_ATTRIB_EXECUTABLE;
					}

					// Debugger needs write support
					#ifdef RAY_KERNEL_DEBUGGER
					currSegment->memoryAttributes  |= MEM_ATTRIB_WRITABLE;
					#endif

					// Fill segment structure accordingly
					currSegment->loadAddress = programHeader.p_vaddr;
					currSegment->segmentLength = programHeader.p_memsz;
					UINT32 offset = programHeader.p_vaddr % PAGE_SIZE;

					// calculate pages needed for the current section
					UINT32 pageCount = NEEDEDPAGES(programHeader.p_memsz + offset);

					UINT8* loadedImage = new(NoMemProtection) UINT8[pageCount * PAGE_SIZE];

					memcpy(&loadedImage[offset], _image + programHeader.p_offset, programHeader.p_filesz);
					memset(&loadedImage[offset +  programHeader.p_filesz], 0, programHeader.p_memsz - programHeader.p_filesz);

					currSegment->segmentStart = loadedImage;
					currSegment->payloadStart = loadedImage + offset;
}


/**
 * Creates the LoadProcessInfo struct necessary for acutally loading
 * the Elf32 file (or object, respectively) as a process
 * @return
 */
LoadProcessInfo Elf32::CreateLoadInfo()
{
	LoadProcessInfo newProcess = LoadProcessInfo(_loadableSegments, NULL);

	newProcess.processInfo.priority = PRIO_NORMAL;
	newProcess.processInfo.privilegeLevel = PRIV_PRIORITY;
	newProcess.processInfo.programEntryPoint = _entryPoint;
	newProcess.processInfo.arguments.count = 0;
	newProcess.processInfo.arguments.values = NULL;

	return newProcess;
}


/**
 * Loads an ELF image and parses it to this object representation.
 * Provides access as LoadProcessInfo to be loadable by LoadProcess()
 * @param image Raw data file containing ELF x86 executable
 * @param length Length (in bytes) of the raw data buffer
 * @return Loadably process information
 */
Elf32::Elf32(UINT8 *image, UINT32 length) :
 _isElf32(IsELF32(image, length)),
 _ElfHeader((Elf32_Ehdr*) image),
 _programHeaders((Elf32_Phdr*) (image + _ElfHeader->e_phoff), _ElfHeader->e_phnum),
 _sections ((Elf32_Shdr*) (image + _ElfHeader->e_shoff), _ElfHeader->e_shnum),
 _loadableSegments(GetNumLoadableSegments()),
 _image(image),
 _length(length)
{
		// load each program header (if necessary)
		LOAD_MEM_SEGMENT* progHdrPointer[_programHeaders.Size];
		for (UINT32 i = 0, lS = 0; i < _programHeaders.Size; i++) {
			// if loadable, only
			if ((_programHeaders[i].p_type == PT_LOAD) && (_programHeaders[i].p_memsz > 0)) {
				ConvertToSegment(_programHeaders[i], &_loadableSegments[lS]);
				progHdrPointer[i] = &_loadableSegments[lS];
				lS++;
			}
		}

		// check for valid entry point
		if ((_ElfHeader->e_entry < USER_TEXT_SEG_START) || (_ElfHeader->e_entry > VIRTUAL_USER_CODE_END)) {
			// Entry point not inside code segment!
			ASSERT(FALSE);
			throw new ElfFormatException(ElfFormatException::InvalidEntryPoint);
		}

		// set entry point
		_entryPoint = _ElfHeader->e_entry;
}


Elf32::~Elf32()
{

}


