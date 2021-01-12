/**
 * @file symboltable.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Feb 12, 2009
 */

/**
 * Returns the name of a function (symbol) by its address
 */
CString GetSymbolByAddress(UINT32 address, UINT32* functionStart);
