/**
 * @file cmdline.c
 * @author Stefan Nuernberger
 * @date Mi Sep 13 2006
 * @brief Checks and analyses the command line
 */

#include "typedefs.h"
#include "cmdline.h"
#include "exceptions.h"
#include "memory/strings/strings.h"
#include "hal/features.h"
#include "memory/memory.h"
#include "video/video_char.h"
#include "debug.h"
#include <profiler/useprofile.h>

#define USEGLOBALCONFIG
#include "globalconfig.h"

static String KernelParameters[64];
static UINT32 KernelParamCount;


#define isParam(string) (strcmp(KernelParameters[curr], string) == 0)
#define isParamArg(string) (strncmp(KernelParameters[curr], string, strlen(string)) == 0)
#define Argument() (strchr(KernelParameters[curr], '=') + 1)

void KernelParseCommandLine(String cmdline) {
	String CommandLine;

	CommandLine = (String)KMalloc(256, MEMTYPE_DATA);

	if (CommandLine == NULL) {
		KernelThrowException("parse_commandline: Out of Memory", EXC_OUT_OF_MEMORY, 0, 0, 0);
	}

	strcpy(CommandLine, cmdline);

	KernelParamCount = 0;
	// iterate through the string
	while (*CommandLine) {
		// and split at every space
		if (*CommandLine == ' ') {
			// combine spaces...
			while (*CommandLine == ' ') {
				*CommandLine = '\0';
				CommandLine++;
			}

			// open new command line pointer when not end of string
			if (*CommandLine) {
				KernelParameters[KernelParamCount] = CommandLine;
				KernelParamCount++;
			}
		} else {
			CommandLine++;
		}
	}
}

void KernelDoCommandLine() {
	UINT32 curr;

	for (curr = 0; curr < KernelParamCount; curr++) {

		if (isParam("immediatecrash")) {
			UNIMPLEMENTED();
#ifdef RAYPROFILING
		} else if (isParamArg("profile")) {
			String argument = Argument();
			if (argument != NULL) {
				ProfilingEnable(atoi(argument) * 1024);
				ProfilingStart();
			} else {
				KernelThrowException("'profile=xxx' argument is missing", EXC_ILLEGAL_STATE, 0, FALSE, NULL);
			}
#endif
		} else if (isParam("irqcheck")) {
			GCONFIG_UNEXPECTED_IRQS = TRUE;
		} else if (isParam("nocanaries")) {
			GCONFIG_USE_CANARIES = FALSE;
		} else {
			printf ("WTF??: '%s'.\n", KernelParameters[curr]);
			KernelThrowException("Unknown kernel parameter supplied.", EXC_ILLEGAL_STATE, 0, 0, 0);
		}
	}
}
