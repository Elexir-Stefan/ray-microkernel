/**
 * @file bitmap.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mo Aug 20 2007
 * @see bitmap.h
 * @brief Summary
 */

#include <typedefs.h>
#include <memory/adt/bitmap.h>
#include <exceptions.h>
#include <debug.h>


#define ULENGTH				BITMAP_ENTRY_SIZE
#define UMAX				(ULENGTH - 1)
#define LeftInUnit(pos, length)		((pos + length) > ULENGTH ? ULENGTH - pos : length)
#define ONES(n)				(0xFFFFFFFF >> (32 - n))

void InitBitmap(MEMORY_BITMAP *bitmap, UINT32 entries) {
	bitmap->bitmapLength = (entries + UMAX) / ULENGTH;
	memset(bitmap->bitmapSpace, 0x00000000, BITMAPBYTES(entries));
	bitmap->lastFit = 0;
}

MEMPROFILE BOOL MarkFreeSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 position, UINT32 mLength) {
	UINT32 bitmapPos = position / ULENGTH;
	UINT32 *currBitmap = bitmap->bitmapSpace + bitmapPos;
	UINT32 posInUnit = (position % ULENGTH);
	UINT32 length = mLength;

	if (bitmapPos + (length / ULENGTH) >= bitmap->bitmapLength) {
		KernelThrowException("MarkFreeSpaceInBitmap: Write outside of bitmap boundaries!", EXC_OUT_OF_MEMORY, position, 0, 0);
	}

	if (!length) {
		KernelThrowException("MarkFreeSpaceInBitmap: Length was 0!", EXC_OUT_OF_MEMORY ,0 ,0 ,0);
	}

	UINT32 left;
	UINT32 pattern;

	do {
		left = LeftInUnit(posInUnit, length);
		pattern = ONES(left) << posInUnit;

		// check if space is really occupied
		ASSERT((*currBitmap & pattern) == pattern);
		*currBitmap &= ~pattern;

		length -= left;
		currBitmap++;
		posInUnit = 0;

	} while (length);

	// set lastFit to the first free position
	if (bitmap->lastFit > position / ULENGTH) {
		bitmap->lastFit = position / ULENGTH;
	}
	return TRUE;
}


MEMPROFILE BOOL MarkUsedSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 position, UINT32 mLength) {
	UINT32 bitmapPos = position / ULENGTH;
	UINT32 *currBitmap = bitmap->bitmapSpace + bitmapPos;
	UINT32 posInUnit = (position % ULENGTH);
	UINT32 length = mLength;

	if (bitmapPos + (length / ULENGTH) >= bitmap->bitmapLength) {
		KernelThrowException("MarkUsedSpaceInBitmap: Write outside of bitmap boundaries!", EXC_OUT_OF_MEMORY, position, 0, 0);
	}

	if (!length) {
		KernelThrowException("MarkUsedSpaceInBitmap: Length was 0!", EXC_OUT_OF_MEMORY ,0 ,0 ,0);
	}


	do {
		UINT32 left = LeftInUnit(posInUnit, length);
		UINT32 pattern = ONES(left) << posInUnit;

		// check if space is really empty
		ASSERT((*currBitmap & pattern) == 0);

		*currBitmap |= pattern;

		length -= left;
		currBitmap++;
		posInUnit = 0;

	} while (length);

	return TRUE;
}

MEMPROFILE UINT32 FindFreeSpaceInBitmap(MEMORY_BITMAP *bitmap, UINT32 length) {
	UINT32 pos = bitmap->lastFit;
	UINT32 contiguousFree = 0;
	UINT32 *space = bitmap->bitmapSpace + pos;
	BOOL everythingUsed = TRUE;
	UINT32 i;

	if (!length) {
		KernelThrowException("FindFreeSpaceInBitmap: Length was 0!", EXC_OUT_OF_MEMORY ,0 ,0 ,0);
	}

	do {
		if (*space != 0xFFFFFFFF) {
			everythingUsed = FALSE;
			if (*space == 0x00000000) {
				// everything's free
				contiguousFree += ULENGTH;
				// sufficient ?
				if (contiguousFree >= length) {
					return (pos + 1) * ULENGTH - contiguousFree;
				} else {
					// more than 32 pages are needed
					space++;
					pos++;
				}
			} else {
				// min 1 is used, max 31
				for (i = 0; i < ULENGTH; i++) {
					if (*space & (1 << i)) {
						// occupied
						contiguousFree = 0;
					} else {
						// free
						contiguousFree++;
						if (contiguousFree >= length) {
							return (pos * ULENGTH) + 1 + i - contiguousFree;
						}
					}
				}

				// still not enough
				space++;
				pos++;
			}
		} else {
			// completely occupied - move 32 bits further
			space++;
			pos++;
			contiguousFree = 0;
			// update lastfit pointer if necessary
			if (everythingUsed) {
				if (bitmap->lastFit < pos) {
					bitmap->lastFit = pos;
				}
			}
		}
	} while (pos < bitmap->bitmapLength);

	KernelThrowException("FindFreeSpaceInBitmap: No fitting space found!", EXC_OUT_OF_MEMORY, length ,0, 0);
	return NO_FITTING_SPACE;
}
