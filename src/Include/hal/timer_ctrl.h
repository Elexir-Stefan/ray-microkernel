#ifndef _TIMER_CTRL_H
#define _TIMER_CTRL_H

/**
 * @file timer_ctrl.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Commands for the PIT (Programmable Interval Timer)
 */


/**
 * hardware ports of the timer
 */
#define PIT_CTRL_PORT		0x43
#define PIT_TMR0		0x40
#define PIT_TMR1		0x41
#define PIT_TMR2		0x42

/**
 * Modes for the PIT
 */
#define PIT_MODE_TERMINAL		0x00
#define PIT_MODE_HW_RETRIGGER	0x02
#define PIT_MODE_RATE			0x04
#define PIT_MODE_SQUARE			0x06
#define PIT_MODE_SW_STROBE		0x08
#define PIT_MODE_HW_STROBE		0x0a

/**
 * PIT command
 */
#define PIT_USE_BCD		0x01
#define PIT_RW_LSB		0x10
#define PIT_RW_MSB		0x20
#define PIT_RW_LSBMSB	0x30
#define PIT_COUNTER0	0x00
#define PIT_COUNTER1	0x40
#define PIT_COUNTER2	0xC0

#endif
