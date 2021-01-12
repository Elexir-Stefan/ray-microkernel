/**
 * @file bitmap.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Aug 20 2007
 * @see bitmap.c
 * @brief Summary
 */

#ifndef _bitmap_H
#define _bitmap_H

#define NO_FITTING_SPACE	0xFFFFFFFF
#define BITMAP_ENTRY_SIZE	32

#define BITMAPBYTES(n)		((n + 7) / 8)

typedef struct {
	UINT32 *bitmapSpace;
	UINT32 bitmapLength;
	UINT32 lastFit;
} MEMORY_BITMAP;

void InitBitmap(MEMORY_BITMAP *bitmap, UINT32 entries);

BOOL MarkFreeSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 position, UINT32 length);
BOOL MarkUsedSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 position, UINT32 length);
UINT32 FindFreeSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 length);

#endif
