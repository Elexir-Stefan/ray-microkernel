#ifndef _8259_CTRL_H
#define _8259_CTRL_H

/**
 * @file 8259_ctrl.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Constants to communicate with the 8259 PIC
 */



/**
 * IO ports for 1st 8259 Controller
 */
#define CTL_8259A_1	0x20
#define CTL_8259A_2 0xA0

#define CTL_8259A_1REG 0x21
#define CTL_8259A_2REG 0xA1

/**
 * commands for the 8259 controller
 */
#define CTL_INIT	0x11
#define CTL_EOI		0x20
#define CTL_MASTER	0x04
#define CTL_SLAVE	0x02
#define CTL_M8086	0x01

#endif
