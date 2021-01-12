#ifndef _PAGING_H
#define _PAGING_H

/**
 * @file paging.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Paging support (part of memory management)
 */

#include <memory/adt/hashmap.h>
#include <memory/adt/bitmap.h>
#include <memory/memfunc.h>
#include <memory/mmu.h>

typedef enum {
	MEM_ATTR_OVERWRITE,
	MEM_ATTR_DISABLE,
	MEM_ATTR_ENABLE
} MEM_ALTER;

/**
 * initializes the paging
 * @param startAddr where memory starts
 * @param length length of memory
 */
void KernelInitMem(UINT32 length);

/**
 * Data type definitions.
 * Note: They have to start at value 0, as they are used
 * to index the MEMORY_BITMAPS arrays.
 */
typedef enum {
	MEMTYPE_DATA = 0,//!< Type assiciated with data used by executable code
	MEMTYPE_CODE = 1,//!< Executable instructions (always read-only!)
} MEMORY_KIND;

typedef struct {
	MEMORY_BITMAP space[2];
	UINT32 start[2];
	UINT32 stop[2];
} SEGMENT_BITMAPS;

/**
 * Structure to manage virtual and physical mappings of
 * memory of threads.
 * Also holds a reference to allocated memory for each
 * thread in a hash map
 */
typedef struct {
	UINT32 *pageDirVirtAddress;		/**< virtual (from kernel's thread) address of page table */
	UINT32 *pageDirPhysAddress;		/**< real physical address where page table is stored */

	HASH *memAllocs;			/**< hash map that saves all memory a thread has allocated */
	UINT32 threadMemUsage;
	UINT32 threadMemPages;
	UINT32 allocatedPageTables;

	SEGMENT_BITMAPS bitmaps;

	UINT32 *usedPageTables;
} MEM_MANAGE;


UINT32 UnMapVirt(MEM_MANAGE *memManage, UINT32 virtAddress, BOOL instananious);
BOOL MapPhysToVirt(MEM_MANAGE *memManage, UINT32 virtAddress, UINT32 physAddress, UINT32 attributes);

/**
 * Searches for free virtual memory and allocates new free physical memory to the found
 * location. Provides support for shifted start of allocation in order to mark free
 * space as used but not actually allocate memory (padding protection)
 * @param memManage
 * @param kind Memory type (data, code)
 * @param pageCount Number of all(!) the pages, including padding
 * @param allocPagesSkip Number of pages to skip at beginning before actually allocating space
 * @param allocPagesPadd Number of pages to skip at end after allocation
 * @param attributes
 */
UINT32 VirtualAlloc(MEM_MANAGE *memManage, MEMORY_KIND kind, UINT32 pageCount, UINT32 allocPagesSkip, UINT32 allocPagesPadd, UINT32 attributes);

void VirtualFree(UINT32 *pageDirectory, UINT32 virtAddress, UINT32 length);

BOOL MarkAbsoluteAddressAsFree(SEGMENT_BITMAPS *bitmaps, UINT32 from, UINT32 pageCount, MEMORY_KIND kind);
BOOL MarkAbsoluteAddressAsUsed(SEGMENT_BITMAPS *bitmaps, UINT32 from, UINT32 pageCount, MEMORY_KIND kind);
UINT32 GetFreeSpaceStart(SEGMENT_BITMAPS *bitmaps, UINT32 pageCount, MEMORY_KIND kind);


/**
 * Frees all page tables listed in the page directory and last but not least the page-directory itself
 * @param virtPageDir to free
 */
void FreePaging(UINT32 *virtPageDir);

/**
 * Returns the physical address according to the virtual address in the particual page directory
 * @param pageDirectory for the virt/phys. mapping
 * @param address to translate
 * @return physical address corresponding to the virtual one, 0 if not present
 */
UINT32 GetPhysicalAddressOf(UINT32 *pageDirectory, POINTER virtAddress);

/**
 * Informs allocation processes about data allocation (default)
 * Will allocate in code-segment if set to FALSE
 * @param setData
 */
void PrepareDataAllocation(BOOL setData);

/**
 * Allocates physical memory and maps it to the specified address
 * @param userMemory thread's memory structure
 * @param virtAddress virtual address where the newly allocated memory will be mapped to
 * @param pageCount Number of pages
 * @param attributes in the page table
 * @return pointer to the last physical address, NULL if unsuccessfull
 */
UINT32 AllocToVirt(MEM_MANAGE *userMemory, UINT32 virtAddress, UINT32 pageCount, UINT32 attributes);

/**
 * Releases virtual memory and may free physical, too
 */
void ReleaseVirtualMemChannel(MEM_MANAGE *memManage, UINT32 virtAddress, UINT32 pageCount, BOOL setFree, BOOL instantanious);

/**
 * Allocates virtual memory and maps it to the specified physical address
 * @note The start of the physical address will be rounded down the the last page and the end will be rounded up to the next page
 * @param memManage MEM_MANAGE structure of the thread for which the channel will be created
 * @param physicalAddress pointer where memory will be mapped to
 * @param length in bytes
 * @param attributes MMU attributes (write, read, kernel etc.)
 * @return
 */
UINT32* CreateVirtualChannel(MEM_MANAGE *memManage, UINT32 physicalAddress, UINT32 length, UINT32 attributes);

/**
 * Changes the attributes of a virtual memory pages (either adds, removes, or resets bits)
 * @param pageDirectory Page directory of the process to look for the virtuall address
 * @param virtAddress Address that is within a particular page
 * @param newAttributes New attributes that should be set/deleted or set as the only bits
 * @param alter Defines whether to set (without altering other) bits, unset (without altering other) bits or to reset bits (other get unset)
 */
BOOL MemoryChangePageAttribute(UINT32 *pageDirectory, POINTER virtAddress, UINT32 newAttribute, MEM_ALTER alter);

/**
 * Changes the attributes of a series of memory pages
 * @param memManage
 * @param virtualAddress
 * @param pageCount
 * @param attributes
 * @param alterType
 */
BOOL MemoryChangeAttribute(MEM_MANAGE* memManage, POINTER virtualAddress, UINT32 pageCount, UINT32 attributes, MEM_ALTER alterType);

/**
 * Moves memory from one thread to another, only paying attention to the memory bitmaps. No allocation
 * hash maps or whatsoever
 * @param fromAddress Address to move memory from
 * @param toAddress Address to memory to
 * @param fromThread Thread that gives memory
 * @param toThread Thread that is memory donated to
 * @param pageCount Number of pages to move (minus padded ones, s.u.)
 * @param allocPagesSkip Number of pages to mark, but not to move at the start
 * @param allocPagesPadd Number of pages to mark, but not to move at the end
 * @param fromKind old kind
 * @param toKind Destination kind
 * @param UINT32 newAttributes);
 */
BOOL MoveThreadMemory(UINT32 fromAddress, UINT32 toAddress, MEM_MANAGE* fromThread, MEM_MANAGE* toThread, UINT32 pageCount, UINT32 allocPagesSkip, UINT32 allocPagesPadd, MEMORY_KIND fromKind, MEMORY_KIND toKind, UINT32 newAttributes);


/** makros for pages */
//#define NEEDEDPAGES(len) (((len) >> PAGE_BITS) + (((len) % PAGE_SIZE) != 0))
#define NEEDEDPAGES(len) 	(ROUNDPAGEUP(len) >> PAGE_BITS)
#define ALIGNTOPAGEEND(size) 	(PAGE_SIZE - (size & PAGE_OFFSET))& PAGE_OFFSET
#define ROUNDPAGEDOWN(address)	((address) & ~PAGE_OFFSET)
#define ROUNDPAGEUP(address)	(ROUNDPAGEDOWN((address) + PAGE_SIZE - 1))

#define SETnthBIT(array, bits, position) array[position / bits] |= (1 << (position % bits))
#define CLRnthBIT(array, bits, position) array[position / bits] &= ~(1 << (position % bits))
#define ISnthBITSET(array, bits, position) (array[position / bits] & (1 << (position % bits)))

#endif

