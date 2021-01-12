/**
 * @file debugbuffer.c
 *
 * @date Sep 23, 2012
 * @author Stefan
 * @brief Implements a debug string buffer that can be read - Current default is to write debug message to screen
 */

#include <typedefs.h>
#include <kdebug/debugbuffer.h>
#include <video/video_char.h>

void DebugWriteMessage(String message)
{
	// FIXME: Temporary solution: print to screen
	KernelDebugMsgString(message);
}
