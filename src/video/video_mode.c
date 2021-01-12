/**
 * @file video_mode.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Fr Nov 30 2007
 * @see video_mode.h
 * @brief Summary
 */

#include "typedefs.h"
#include "video/video_mode.h"
#include "video/vga_io.h"
#include "hal/io.h"
#include "memory/memfunc.h"

static void WriteFont (UINT8 *buf, UINT8 fontHeight, UINT16 *frameBuffer);
static void SetPlane(UINT32 p);

void SetVideoMode(VideoMode mode, UINT16 *frameBuffer) {
	UINT32 i;
	UINT8 *regs;
	UINT8 *font;
	UINT8 fontHeight;

	switch (mode) {
	case MODE_80X25:
		regs = Dump80x25;
		font = Font8x16;
		fontHeight = 16;
		break;
	case MODE_80X50:
	default:
		regs = Dump80x50;
		font = Font8x8;
		fontHeight = 8;
		break;
	}

	/* write MISCELLANEOUS reg */
	OutPortB(VGA_MISC_WRITE, *regs);
	regs++;

	/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		OutPortB(VGA_SEQ_INDEX, i);
		OutPortB(VGA_SEQ_DATA, *regs);
		regs++;
	}

	/* unlock CRTC registers */
	OutPortB(VGA_CRTC_INDEX, 0x03);
	OutPortB(VGA_CRTC_DATA, InPortB(VGA_CRTC_DATA) | 0x80);
	OutPortB(VGA_CRTC_INDEX, 0x11);
	OutPortB(VGA_CRTC_DATA, InPortB(VGA_CRTC_DATA) & ~0x80);

	/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;

	/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++) 	{
		OutPortB(VGA_CRTC_INDEX, i);
		OutPortB(VGA_CRTC_DATA, *regs);
		regs++;
	}

	/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++) {
		OutPortB(VGA_GC_INDEX, i);
		OutPortB(VGA_GC_DATA, *regs);
		regs++;
	}

	/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++) {
		(void)InPortB(VGA_INSTAT_READ);
		OutPortB(VGA_AC_INDEX, i);
		OutPortB(VGA_AC_WRITE, *regs);
		regs++;
	}

	/* lock 16-color palette and unblank display */
	InPortB(VGA_INSTAT_READ);
	OutPortB(VGA_AC_INDEX, 0x20);

	/* write according font */
	WriteFont(font, fontHeight, frameBuffer);
	//UpdateScreenSize(cols, rows, fontHeight, vgaBIOS);
}

static void SetPlane(UINT32 p) {
	UINT8 pmask;

	p &= 3;
	pmask = 1 << p;

	/* set read plane */
	OutPortB(VGA_GC_INDEX, 4);
	OutPortB(VGA_GC_DATA, p);

	/* set write plane */
	OutPortB(VGA_SEQ_INDEX, 2);
	OutPortB(VGA_SEQ_DATA, pmask);
}

static void WriteFont (UINT8 *buf, UINT8 fontHeight, UINT16 *frameBuffer) {

	UINT8 seq2, seq4, gc4, gc5, gc6;
	UINT32 i;

	/* save registers
	set_plane() modifies GC 4 and SEQ 2, so save them as well */
	OutPortB(VGA_SEQ_INDEX, 2);
	seq2 = InPortB(VGA_SEQ_DATA);

	OutPortB(VGA_SEQ_INDEX, 4);
	seq4 = InPortB(VGA_SEQ_DATA);

	/* turn off even-odd addressing (set flat addressing)
	assume: chain-4 addressing already off */
	OutPortB(VGA_SEQ_DATA, seq4 | 0x04);

	OutPortB(VGA_GC_INDEX, 4);
	gc4 = InPortB(VGA_GC_DATA);

	OutPortB(VGA_GC_INDEX, 5);
	gc5 = InPortB(VGA_GC_DATA);

	/* turn off even-odd addressing */
	OutPortB(VGA_GC_DATA, gc5 & ~0x10);

	OutPortB(VGA_GC_INDEX, 6);
	gc6 = InPortB(VGA_GC_DATA);

	/* turn off even-odd addressing */
	OutPortB(VGA_GC_DATA, gc6 & ~0x02);

	/* write font to plane P4 */
	SetPlane(2);

	/* write font 0 */
	for(i = 0; i < 256; i++) {
		memcpy(frameBuffer + (i << 4), buf, fontHeight);
		buf += fontHeight;
	}

	/* restore registers */
	OutPortB(VGA_SEQ_INDEX, 2);
	OutPortB(VGA_SEQ_DATA, seq2);
	OutPortB(VGA_SEQ_INDEX, 4);
	OutPortB(VGA_SEQ_DATA, seq4);
	OutPortB(VGA_GC_INDEX, 4);
	OutPortB(VGA_GC_DATA, gc4);
	OutPortB(VGA_GC_INDEX, 5);
	OutPortB(VGA_GC_DATA, gc5);
	OutPortB(VGA_GC_INDEX, 6);
	OutPortB(VGA_GC_DATA, gc6);
}

void UpdateCursor(UINT32 absPos) {
	OutPortB(VGA_CRTC_INDEX, 14);
	OutPortB(VGA_CRTC_INDEX + 1, absPos >> 8);
	OutPortB(VGA_CRTC_INDEX, 15);
	OutPortB(VGA_CRTC_INDEX + 1, absPos & 0xFF);
}


UINT32 GetFBAddress() {
	UINT32 addr;

	OutPortB(VGA_GC_INDEX, 6);
	addr = InPortB(VGA_GC_DATA);
	addr >>= 2;
	addr &= 3;
	switch(addr) {
		case 0:
		case 1:
			addr = 0xA0000;
			break;
		case 2:
			addr = 0xB0000;
			break;
		case 3:
			addr = 0xB8000;
			break;
	}
	return addr;
}
