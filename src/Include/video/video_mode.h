/**
 * @file video_mode.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Fr Nov 30 2007
 * @see video_mode.c
 * @brief Summary
 */

#ifndef _video_mode_H
#define _video_mode_H

#include "typedefs.h"

typedef enum {
	MODE_80X25,
	MODE_80X50
} VideoMode;

void SetVideoMode(VideoMode mode, UINT16 *frameBuffer);
UINT32 GetFBAddress();
inline void UpdateCursor(UINT32 absPos);

#endif
