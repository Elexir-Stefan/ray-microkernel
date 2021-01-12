#ifndef _APPEARANCE_H
#define _APPEARANCE_H

/**
 * @file appearance.h
 * @author Stefan Nuernberger
 * @date 09-11-2006
 * @brief Appearance header file
 */

/**
 * Displays information of the currently running kernel and the system and sets up
 * console colors etc.
 * @param memupper Upper memory of the system to display
 */
void KernelSetTextLayout(UINT32 memupper);

#endif
