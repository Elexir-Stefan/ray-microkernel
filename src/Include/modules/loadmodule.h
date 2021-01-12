#ifndef _LOADMODULE_H
#define _LOADMODULE_H

/**
 * @file loadmodule.h
 * @author Stefan Nuernberger
 * @date 09-14-2006
 * @brief Starts all modules loaded by grub as threads.
 */

#ifdef __cplusplus
extern "C" {
#endif


	#include <memory/mmu.h>

	void StartModules(ModuleLoadInfo module, UINT32 moduleCount);

#ifdef __cplusplus
}

namespace Executables
{
	class Modules
	{
	public:
		static void Start(ModuleLoadInfo module, UINT32 moduleCount);

	private:
		static void ExtractFileName(char *fileName, char *source);
	};
}
#endif

#endif
