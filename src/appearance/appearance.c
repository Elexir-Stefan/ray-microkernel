
/**
 * @file appearance.c
 * @author Stefan Nuernberger
 * @date Mi Sep 13 2006
 * @brief Defines the appearance of the kernel messages (colors, size etc...)
 */

#include "typedefs.h"
#include "video/video_char.h"
#include "video/video_mode.h"
#include "appearance/appearance.h"
#include "version.h"
#include "verdate.h"

#define TEXTFOREGROUND	15
#define TEXTBACKGROUND	7

void KernelSetTextLayout(UINT32 memupper) {
	UINT32 memtotal;
	UINT32 memtotal_mb;

	memtotal = 655360 + memupper;
	memtotal_mb = (memtotal >> 20) + 1;

	VideoSetBCol(TEXTBACKGROUND);
	VideoClear();

#ifdef RAYDEBUG
	VideoSetBCol(4);
	VideoSetFCol(15);
	/* fill first line */
	VideoFillLine();
	/* print some infos */
	printf ("\rRAY DEBUG Microkernel (v%u.%u Build %u, %s %s) [%u MB RAM]\n\n",
			RAY_VERSION_MAJOR,
			RAY_VERSION_MINOR,
			RAY_VERSION_BUILD,
			RAY_VERSION_DATE,
			RAY_VERSION_TIME,
memtotal_mb);
#ifdef RAYPROFILING
	VideoSetBCol(14);
	VideoSetFCol(8);
	VideoFillLine();
	printf ("                            !!! PROFILING SUPPORTED !!!\n");
#endif
#else
	VideoSetBCol(1);
	VideoSetFCol(14);
	/* fill first line */
	VideoFillLine();
	/* print some infos */
	printf ("\rNURNware RAY Microkernel, Build %u [Machine has %u MB RAM]\n\n",
			RAY_VERSION_BUILD,
			memtotal_mb);
#endif
	VideoSetFCol(TEXTFOREGROUND);
	VideoSetBCol(TEXTBACKGROUND);
}
