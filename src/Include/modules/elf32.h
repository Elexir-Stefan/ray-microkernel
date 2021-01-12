/**
 * @file elf32.h
 * @date 2010-07-21
 * @author Stefan
 *
 * @brief Elf32 executable format support
 */
#ifndef _ELF_H
#define _ELF_H

extern "C" {
	#include <mtask/task_types.h>
	#include <modules/elf_types.h>
}
#include <modules/load_types.h>
#include <cpp/staticlist.h>
#include <cpp/exception.h>

namespace Executables {

class ElfFormatException : public RayKernel::Exception
{
public:
	enum ElfFailureReason
	{
		InvalidFormat,
		InvalidEntryPoint,
		InvalidRelocationInfo,
		NoSymbolTableFound,
		RelocationAboutNonLoadedSection,
		InvalidUsage
	};

	ElfFormatException(const ElfFailureReason& reason);
	const ElfFailureReason& GetReason();
private:
	const ElfFailureReason& _reason;


};

class Elf32
{
public:
	Elf32(UINT8 *image, UINT32 length);
	~Elf32();

	bool IsELF32(UINT8 *image, UINT32 length);
	LoadProcessInfo CreateLoadInfo();

private:
	bool _isElf32;
	Elf32_Ehdr* _ElfHeader;

	StaticList<Elf32_Phdr> _programHeaders;
	StaticList<Elf32_Shdr> _sections;

	StaticList<LOAD_MEM_SEGMENT> _loadableSegments;

	UINT8* _image;
	UINT32 _length;

	UINT32 _entryPoint;

	void ConvertToSegment(Elf32_Phdr& programHeader, LOAD_MEM_SEGMENT* currSegment);
	UINT16 GetNumLoadableSegments();

};

}
#endif
