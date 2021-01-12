/**
 * @file hash.h
 * @author Stefan Nuernberger
 * @date 09-11-2006
 * @brief Prototypes and structures needed for hashing
 */

#ifndef _HASH_H
#define _HASH_H

#define H_OCCUPIED 0x00000001

#define HASHFUNCTION(key, table) (key % table->primeSize)
#define COLLISION(table, var) ((var + 1) % table->primeSize)
#define TABLEINDEX(table, index) ( (index <= table->primeSize) ? &table->hashTabPtr[table->entryStep * index] : NULL)
#define ENTRYDATA(item) ((POINTER)((char*)(item) + sizeof(HASH_ENTRY)))
#define STEPWIDTH(table, index) (STEPOF(TABLEINDEX(table, index)))
#define STEPOF(element) (element->chunkInfo >> 1)
#define SETDISTANCE(dist) (((dist) << 1) | H_OCCUPIED)
#define DISTANCE(from, to) SETDISTANCE(to - from)
#define OCCUPIED(table, index) OCCUPIED_E(TABLEINDEX(table, index))
#define OCCUPIED_E(entry) (entry->chunkInfo & H_OCCUPIED)

typedef struct {
	UINT32 hashKey;
	SINT32 chunkInfo;
} __attribute__((packed)) HASH_ENTRY;

typedef struct {
	HASH_ENTRY *hashTabPtr;
	UINT32 numEntries;
	UINT32 entrySize;
	UINT32 entryStep;
	UINT32 primeSize;
	UINT32 warnSize;
} HASH;

#define HashCreate(hashTable, entries, type) _HashCreate(hashTable, entries, sizeof(type))

/**
 * Returns an initialized hash-table (using KMalloc)
 * @param entries Number of hash-table entries to be "rounded" to next prime
 * @param type Type of the entries in the hash-table
 */
#define HashCreateAndInit(entries, type) _HashCreateAndInit(entries, sizeof(type))


HASH* _HashCreateAndInit(UINT32 entries, UINT32 typeSize);
void _HashCreate(HASH *hashTable, UINT32 entries, UINT32 typeSize);
void HashInsert(HASH *hashTable, UINT32 key, POINTER payload);
POINTER HashRetrieve(HASH *hashTable, UINT32 hashKey);
HASH_ENTRY* HashItemRetrieve(HASH *hashTable, UINT32 hashKey);
BOOL HashDelete(HASH *hashTable, UINT32 hashKey);
void HashDestroy(HASH *hashTable);
BOOL HashUpdate(HASH *hashTable, UINT32 hashKey, POINTER payload);
UINT32 HashTableSize(HASH* hashTable);

#endif
