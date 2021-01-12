/**
 * @file hash.c
 * @author Stefan Nuernberger
 * @date Mo Sep 11 2006
 * @brief Hashing functions used to store and retrieve information about memory allocation
 */

#include "typedefs.h"
#include "debug.h"
#include "memory/memfunc.h"
#include "memory/memory.h"
#include "memory/adt/hashmap.h"
#include "exceptions.h"
#include "math/mathfuncs.h"



/**
 * initialize hash size (prime!)
 * @param hashTable to use
 * @param entries Number of hash-table entries to be "rounded" to next prime
 * @param typeSize Size of the type to be used in the hash-table
 */
void _HashCreate(HASH *hashTable, UINT32 entries, UINT32 typeSize) {
	UINT32 hashPrime = GetNextPrime(entries);

	// calculate the total length of management structure overhead + payload
	UINT32 entryLength = sizeof(HASH_ENTRY) + typeSize;
	hashTable->entryStep = CeilDiv(entryLength, sizeof(HASH_ENTRY));
	hashTable->primeSize = hashPrime;
	hashTable->warnSize = (hashPrime >> 1) + (hashPrime >> 2);	// 75%
	hashTable->numEntries = 0;
	hashTable->entrySize = typeSize;
}

/**
 * Returns an initialized hash-table (using KMalloc)
 * @param entries Number of hash-table entries to be "rounded" to next prime
 * @param typeSize Size of the type to be used in the hash-table
 */
MEMPROFILE HASH* _HashCreateAndInit(UINT32 entries, UINT32 typeSize) {
	HASH hash;

	// initialize the structures and set the size to a prime
	_HashCreate(&hash, entries, typeSize);

	// allocate space for structure + map
	UINT32 tableBytes = HashTableSize(&hash);
	HASH* realHash = (HASH*)KMalloc(sizeof(HASH) + tableBytes, MEMTYPE_DATA);
	// copy old temporary structure to new one
	memcpy(realHash, &hash, sizeof(HASH));

	realHash->hashTabPtr = (HASH_ENTRY*)(realHash + 1);
	if (UNLIKELY(!realHash->hashTabPtr)) {
		ASSERT(FALSE);
		return NULL;
	}

	memset(realHash->hashTabPtr, 0, tableBytes);
	return realHash;
}

/**
 * Returns the size of the hashTable in bytes (Usefull to allocate hashTabPtr
 * outside of HashInit() )
 */
MEMPROFILE UINT32 HashTableSize(HASH* hashTable) {
	return hashTable->primeSize * hashTable->entryStep * sizeof(HASH_ENTRY);
}

/**
 * Releases the spaces occupied by the hash
 * @param hashTable to release
 */
MEMPROFILE void HashDestroy(HASH *hashTable) {
	KFree(hashTable, MEMTYPE_DATA, TRUE);
}

/**
 * retrieves a value associated with a key
 * @param hashTable to use
 * @param hashKey to find
 * @return a pointer to the payload
 */
MEMPROFILE POINTER HashRetrieve(HASH *hashTable, UINT32 hashKey) {
	HASH_ENTRY* entry = HashItemRetrieve(hashTable, hashKey);
	if (entry) {
		return ENTRYDATA(entry);
	} else {
		// not found
		return NULL;
	}
}

/**
 * Retrieves the HASH_ENTRY item associated with the hashKey or
 * NULL of not found
 * @param hashTable to look in
 * @param hashKey to look for
 */
MEMPROFILE HASH_ENTRY* HashItemRetrieve(HASH *hashTable, UINT32 hashKey) {
	UINT32 mainHash;

	mainHash = HASHFUNCTION(hashKey, hashTable);
	SINT32 nextStep;

	// search as long as it's linked (not necessarily occupied!)
	do {
		// Linked. Is it the item we are looking for?
		if (TABLEINDEX(hashTable, mainHash)->hashKey == hashKey) {
			return TABLEINDEX(hashTable, mainHash);
		}

		// No, it's not -> go to next
		nextStep = STEPWIDTH(hashTable, mainHash);
		mainHash += nextStep;
	} while (nextStep);

	// not found
	return NULL;
}

/**
 * Updates a value of the specified key
 * @param hashTable to use
 * @param hashKey to find
 * @param payload to copy(!) to the hash table
 * @return TRUE on success, FALSE of key not found
 */
MEMPROFILE BOOL HashUpdate(HASH *hashTable, UINT32 hashKey, POINTER newValue) {
	HASH_ENTRY *entry = HashRetrieve(hashTable, hashKey);

	if (entry != NULL) {
		// update it
		POINTER payLoad = ENTRYDATA(entry);
		memcpy(payLoad, newValue, hashTable->entrySize);
		return TRUE;
	} else {
		// not found
		return FALSE;
	}

}

/**
 * deletes a value in the hash-table according to it's key
 * @param hashTable to use
 * @param hashKey to delete
 * @return TRUE on success (deleted), FALSE otherwise
 */
MEMPROFILE BOOL HashDelete(HASH *hashTable, UINT32 hashKey) {
	HASH_ENTRY *found = HashItemRetrieve(hashTable, hashKey);

	if (found) {
		// Implementation of lazy deletion

		found->hashKey = 0;
		// removes the H_OCCUPIED bit but leaves the distance
		found->chunkInfo &= ~H_OCCUPIED;
		hashTable->numEntries--;
		return TRUE;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}

}

/**
 * inserts a value into the hash-table
 * @param hashTable to use
 * @param key to retrieve/delete later
 * @param payload to copy(!) to the hash table
 */
MEMPROFILE void HashInsert(HASH *hashTable, UINT32 key, POINTER payload) {
	UINT32 mainHash;
	UINT32 lastPos;

	if (hashTable->numEntries >= hashTable->warnSize) {
		KernelThrowException("HashMap size exceeded 75%.", 0, 0, FALSE, NULL);
	}

	mainHash = HASHFUNCTION(key, hashTable);

	// as long as it's occupied
	while (STEPWIDTH(hashTable, mainHash) && OCCUPIED(hashTable, mainHash)) {
		// follow row
		mainHash += STEPWIDTH(hashTable, mainHash);
	}
	lastPos = mainHash;

	SINT32 nextStep = STEPWIDTH(hashTable, lastPos);

	// linear probing (clusters but good cache performance due to minimal cache misses)
	while (OCCUPIED(hashTable, mainHash)) {
		mainHash = COLLISION(hashTable, mainHash);
	}

	// Finally found free space, set stepwitdh to next entry
	TABLEINDEX(hashTable, lastPos)->chunkInfo = DISTANCE(lastPos, mainHash);

	TABLEINDEX(hashTable, mainHash)->hashKey = key;
	TABLEINDEX(hashTable, mainHash)->chunkInfo = SETDISTANCE(nextStep);

	// copy payload
	POINTER data = ENTRYDATA(TABLEINDEX(hashTable, mainHash));
	memcpy(data, payload, hashTable->entrySize);

	hashTable->numEntries++;
}

