/**
 * @file hashing.c
 * @author Stefan Nuernberger
 * @brief Mathematical hashing functions
 */

#include <typedefs.h>
#include <memory/adt/hashing.h>

/**
 * Generates a 32 bit hash of the input string
 * @param string to hash
 * @return hash of the string (32 bits only!)
 */
UINT32 SimpleStringHash(CString string) {
	UINT32 lastHash = 0x9D8A7C34;
	UINT32 hash = 0x4A2E7BD9;
	UINT16 value;

	while(*string) {
		value = (UINT16)*string++;
		hash = (hash << 1) | ((hash>>31) & 1);
		hash += (hash & 0xFFFF) * value + (value & (hash & 0xFFFF));
		hash ^= lastHash;
		lastHash = hash;
	}

	return hash;

}
