#ifndef _IOPERM_H
#define _IOPERM_H

/**
 * @file ioperm.h
 * @author Stefan Nuernberger
 * @date Fr Okt 27 2006
 * @brief Controls I/O permissions
 */

#include "app/tdm.h"
#include "mtask/task_types.h"

#define PORTMAPSIZE	8192

/**
 * Creates an I/O permission bitmap
 * @param userMemory thread where the bitmap will be mapped to
 */
BOOL CreateThreadIOPermission(PROCESS_PTR thread);

/**
 * Flips according bit in IO-permission bitmap from allow to deny and vice versa
 * @param port to be flipped (allow/deny)
 */
RAY_TDM FlipIOPort(UINT16 port);

/**
 * Creates structures needed to store IO-ports that are in use by a driver to avoid collision
 */
void InitIOPermissions();

#endif
