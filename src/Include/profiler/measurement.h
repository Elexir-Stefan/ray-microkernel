/**
 * @file measurement.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Jan 1 2008
 * @see measurement.c
 * @brief Summary
 */

#ifndef _measurement_H
#define _measurement_H

#define READTSC_LO(var) __asm__ __volatile__("rdtsc":"=a"(var)::"edx")


#endif
