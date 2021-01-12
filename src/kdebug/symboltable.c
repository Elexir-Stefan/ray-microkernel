/**
 * @file symboltable.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Feb 12, 2009
 * @brief Saves a reference to every kernel function and its name
 */

#include <typedefs.h>
#include <debug.h>

typedef struct {
	UINT32 funcLinearAddress;
	CString name;
} SYMBOL_MAP;

static SYMBOL_MAP symbols[] = {
	#include "symbolmap.gen"
	{0xFFFFFFFF, ""}
};

CString GetSymbolByAddress(UINT32 address, UINT32* functionStart) {
	UINT32 numEntries = sizeof(symbols) / sizeof(SYMBOL_MAP);

	UINT32 max = 0;
	SYMBOL_MAP* maxEntry = NULL;
	UINT32 i;

	for(i = 0; i < numEntries; i++) {
		// find maximum that is still smaller than address
		if ((symbols[i].funcLinearAddress > max) && (symbols[i].funcLinearAddress < address)) {
			max = symbols[i].funcLinearAddress;
			maxEntry = symbols + i;
		}
	}

	if (maxEntry)
	{
		*functionStart = maxEntry->funcLinearAddress;
		return maxEntry->name;
	}
	else
	{
		//ASSERT(FALSE);
		return "??";
	}
}
