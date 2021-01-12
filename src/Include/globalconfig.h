/**
 * @file globalconfig.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Okt 3 2007
 * @brief Manages global variables for kernel runtime-configuration (normally via command-line parameters)
 */

#ifndef _globalconfig_H
#define _globalconfig_H

#ifdef USEGLOBALCONFIG
	#define DefineGlobalDefault(name, type, defaultvalue) extern type GCONFIG_##name
#else
	#define DefineGlobalDefault(name, type, defaultvalue) type GCONFIG_##name = defaultvalue
#endif



DefineGlobalDefault(UNEXPECTED_IRQS, BOOL, FALSE);
DefineGlobalDefault(USE_CANARIES, BOOL, TRUE);

#endif
