/**
 * @file video_char.c
 * @author NURNware Technologies [Stefan Nuernberger - stefan@nurnware.de]
 * @date 02-25-2006
 * @brief Implementation of standard C functions like printf but also some special ones used in kernel mode only
 */

#include "typedefs.h"
#include "video/video_char.h"
#include "memory/memfunc.h"
#include "hal/io.h"
#include "c/stdarg.h"
#include "video/video_mode.h"
#include <debug.h>

// Do not scroll the first two lines
#define SKIPHEADER	2

#define USE_VIDEO_MODE_80X50

#ifdef USE_VIDEO_MODE_80X50
	#define ScreenCols 80
	#define ScreenLines 50
#else
	#define ScreenCols 80
	#define ScreenLines 25
#endif

#define USEGLOBALCONFIG
#include "globalconfig.h"

static inline void itoa (char* buf, int base, int d);
static void putchar (char c);
static void ScrollUp(void);
static void NewLine(void);

/* text attribute */
static UINT8 ATTRIBUTE = 0x07;
/* x-position */
static UINT8 xpos;
/* y-position */
static UINT8 ypos;
/* start of video memory */
static UINT16* frameBuffer;

#ifdef RAYDEBUG

#include <memory/mmu.h>
#define DBG_MEM_CHARS	8
char MEMBUFFER[DBG_MEM_CHARS];
UINT32 currThreadCol = 0;

void ThreadSchedule() {
	UINT16 *screenLine = frameBuffer + ScreenCols;
	UINT16 *carret = screenLine + currThreadCol;
	*carret = 0x6F00 + (((*carret & 0xFF) + 1) & 0xFF);

	itoa(MEMBUFFER, 'd', SystemMemoryUsed());
	int i;
	carret = screenLine + ScreenCols - DBG_MEM_CHARS;
	for (i = 0; i < DBG_MEM_CHARS; i++) {
		if (MEMBUFFER[i] == 0) break;
		*carret = 0x7000 + (MEMBUFFER[i] & 0xFF);
		carret++;
	}
}

void ThreadAdvance() {
	UINT16 *screenLine = frameBuffer + ScreenCols;
	UINT16 *carret = screenLine + currThreadCol;
	*carret = 0x2800 + (*carret & 0xFF);

	currThreadCol++;
	if (currThreadCol + DBG_MEM_CHARS >= ScreenCols ) {
		currThreadCol = 0;
	}
}
#endif


UINT32 currDebugMsgPos = 0;
void KernelDebugMsgString(char *string)
{
	while (*string) {
		KernelDebugMsg(*string++);
	}

}

void KernelDebugMsg(char c) {
	UINT16 *carret = frameBuffer + ScreenCols + currDebugMsgPos;
	*carret = 0x4E00 + c;

	currDebugMsgPos++;
	if (currDebugMsgPos >= ScreenCols) currDebugMsgPos = 0;
}


/**
 * sets the foreground color to use for printf
 * @param color Color to use as foreground color for printf
 */
void VideoSetFCol(UINT8 color) {
	ATTRIBUTE = (ATTRIBUTE & 0xF0) + (color & 0xF);
}

/**
 * sets the background color to use for printf
 * @param color Color to use as background color for printf
 */
void VideoSetBCol (UINT8 color) {
	ATTRIBUTE = (ATTRIBUTE & 0xF) + ((color & 0xF) << 4);
}

void VideoGotoX(UINT16 x) {
	if (x > ScreenCols) return;
	xpos = x;
}

void VideoGotoY(UINT16 y) {
	if (y > ScreenLines) return;
	ypos = y;
}

/**
 * Initialize the video card output routines
 * @param address Virtual address to use as shadowCopy
 */
void VideoInit(UINT32 address) {
	frameBuffer = (UINT16*) address;

#ifdef USE_VIDEO_MODE_80X50
	SetVideoMode(MODE_80X50, frameBuffer);
#else
	SetVideoMode(MODE_80X25, frameBuffer);
#endif
}


void VideoClear (void) {
	UINT16 new_attr;
	new_attr = ATTRIBUTE & 0xff;
	new_attr <<= 8;
	new_attr += ' ';

	memsetw(frameBuffer, new_attr, ScreenLines * ScreenCols);

	xpos = 0;
	ypos = 0;
}

void VideoWriteAttribute(UINT8 col, UINT8 row, UINT16 value) {
	ASSERT((col < ScreenCols) && (row < ScreenLines));
	UINT16 *address = frameBuffer + (ScreenCols * row) + col;

	*address = value;
}

/**
 * implements the itoa function which converts an integer/short/... to a string
 */
inline static void itoa (char* buf, int base, int d) {
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	UINT8 counter = 0;

	/* If %d is specified and D is minus, put `-' in the head. */
	if ((base == 'd') && (d < 0)) {
		*p++ = '-';
		buf++;
		ud = -d;
	} else {
		if (base == 'x') divisor = 16;
	}
	/* Divide UD by DIVISOR until UD == 0. */
	do {
		int remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		counter++;
	} while ( (ud /= divisor) || ((base == 'x') && (counter != 8)) );

	/* Terminate BUF. */
	*p = 0;

	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

/**
 * used by NewLine() to scroll the video screen up
 */
inline static void ScrollUp(void) {
	UINT16 newAttribute;
	UINT16 notLastLine = (ScreenLines - 1) * ScreenCols;

	/* move lines upwards */
	memcpy(frameBuffer + ScreenCols * SKIPHEADER, frameBuffer + ScreenCols * (SKIPHEADER + 1), (notLastLine - ScreenCols * SKIPHEADER) * sizeof(UINT16));

	/* clear last line with attribute and space */
	newAttribute = (ATTRIBUTE<<8)  | ' ';
	memsetw(frameBuffer + notLastLine, newAttribute, ScreenCols);
}

/**
 * enters a new line on the screen (next characters printed will be
 * on a new line)
 */
inline static void NewLine (void) {
	xpos = 0;
	ypos++;
	if (ypos > ScreenLines - 1) {
		ScrollUp();
		ypos--;
	}
}

/**
 * put one character on the screen
 * also implements some escape characters
 * @param c character
 */
inline static void putchar (char c) {

	switch (c) {
	case  '\n':
		NewLine();
		return;
	case '\r' :
		xpos = 0;
		return;
	case '\t' :
		xpos = ((xpos >> 3) + 1) << 3;
		return;
	default:
		*(frameBuffer + (ypos * ScreenCols + xpos)) = (ATTRIBUTE<<8) | c;
		xpos++;
		if (xpos >= ScreenCols) NewLine();
	}
}

void print (const char *string) {
	char c;
	while ((c = *string++) != 0) {
			putchar (c);
	}
	UpdateCursor(ypos * ScreenCols + xpos);
}


void printf (const char *format, ...) {
	va_list arg_ptr;
	int c;
	char buf[20];
	char *p;

	va_start(arg_ptr, format);

	while ((c = *format++) != 0) {
		if (c == '%') {
			c = *format++;
			switch (c) {
			case 'd':
			case 'u':
			case 'x':
				itoa(buf, c, va_arg(arg_ptr, int));
				p = buf;
				goto string;
				break;

			case 's':
				p = va_arg(arg_ptr, char*);
				string:
					while (*p) {
						putchar (*p++);
					}
				break;

			case '%':
				putchar('%');
				break;
			case 'c':
			default:
				putchar (va_arg(arg_ptr, int));
				break;
			}
		} else {
			putchar (c);
		}
	}
	UpdateCursor(ypos * ScreenCols + xpos);
	va_end(arg_ptr);
}

/**
 * fill the current line with the current background color
 */
void VideoFillLine(void){
	int i;
	int old_y = ypos;
	int old_x = xpos;

	for (i = xpos; i < ScreenCols; i++) putchar (' ');
	ypos = old_y;
	xpos = old_x;
}
