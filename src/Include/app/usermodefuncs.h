#ifndef _USERMODEFUNCS_H
#define _USERMODEFUNCS_H

/**
 * @file usermodefuncs.h
 * @author Stefan Nuernberger
 * @date 09-11-2006
 * @brief Prototypes to register kernel mode functions and make them available to usermode
 */

/**
 * Registeres functions that can be called by the SCT
 */
void SCTRegisterKernelFunctions(void);

#endif
