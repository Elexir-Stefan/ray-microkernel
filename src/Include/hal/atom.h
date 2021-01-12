/**
 * @file atom.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Mär 11 2008
 * @see atom.c
 * @brief Summary
 */

#ifndef _atom_H
#define _atom_H

/**
 * Call this function before code that must not be interrupted
 */
void AtomicSectionStart();

/**
 * Call this function to release the atomic execution
 */
void AtomicSectionEnd();

#endif
