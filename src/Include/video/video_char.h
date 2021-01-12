#ifndef _VIDEO_CHAR_H
#define _VIDEO_CHAR_H

#include "typedefs.h"

/**
 * @file video_char.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Video-specific functions
 */

/**
 * initializes the functions in this file
 * \note must be called _BEFORE_ you can use the functions in this file
 */
void VideoInit(UINT32 address);

/**
 * clears the screen
 */
void VideoClear (void);

/**
 * Sets the foreground color (use printf to use it)
 * @param color Color
 * @sa printf
 */
void VideoSetFCol (UINT8 color);

/**
 * Sets the background color (use printf to use it)
 * @param color Color
 * @sa printf
 */
void VideoSetBCol (UINT8 color);

/**
 * Sets the cursor x-position
 * @param x x-position
 */
void VideoGotoX(UINT16 x);

/**
 * Sets the cursor y-position
 * @param y y-position
 */
void VideoGotoY(UINT16 y);

/**
 * Print a formatted string on the screen
 * @param format String with standard C-like %variables and \escapecharacters
 * @param ... Variables to insert in the formatted string
 */
void printf (const char *format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * Print an unformatted string on the screen
 * @param string to print
 */
void print (const char *string);

/**
 * fill the current line with the background color
 */
void VideoFillLine(void);

void VideoWriteAttribute(UINT8 col, UINT8 row, UINT16 value);

#ifdef RAYDEBUG
void ThreadAdvance();
void ThreadSchedule();
#endif


void KernelDebugMsgString(char *c);
void KernelDebugMsg(char c);

#endif
