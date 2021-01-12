/**
 * @file tdm.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Do Mär 15 2007
 * @brief Trusted Driver Model
 */

#ifndef _tdm_H
#define _tdm_H

typedef enum {
	TDM_SUCCESS = 0,
	TDM_INSUFFICIENT_RIGHTS = 1,
	TDM_TRUSTED_ONLY = 2,
	TDM_OUT_OF_MEMORY = 3,
	TDM_PARTNERSHIP_ONLY = 4,
	TDM_ILLEGAL_ARGUMENT = 5,
	TDM_IMPOSSIBLE = 6
} RAY_TDM;

#endif
