#ifndef _TIMER_H
#define _TIMER_H

/**
 * @file timer.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Commands for the PIT (Programmable Interval Timer)
 */

/**
 * @brief Frequency of the timer chip (PIC) in Hz
 * @note You can only set a divisor for the PIC, the main frequency is 1193180 Hz
 */
#define PIT_FREQU	1193180

/**
 * sets up the timer to the specified interval in Hz
 * @param timer Wich timer shoud be set (0,1,2)
 * @param int_Hz The timer frequency in Hertz
 */
void KernelTimerSetup(UINT8 timer, UINT32 intHz);

/**
 * Reads the current counter of the PIT
 * @param channel Channel to read ticks from (0, 1 or 2)
 * @return number of ticks of the reference clock
 */
UINT16 TimerReadCounter(UINT8 channel);

UINT64 ReadTSC();

void ManualTimerInterrupt();

#endif
