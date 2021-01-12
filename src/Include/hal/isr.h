#ifndef _ISR_H
#define _ISR_H

/**
 * @file isr.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief Interrupt Service Routines
 * Prototypes of funcions that will be called from the processor if
 * an exception occured
 */


/**
 * @brief Called by the processeor when a 'Division by zero' exception occured
 */
void ISR_exc_DivZero(void);

/**
 * @brief Called by the processeor when a 'Debug' exception occured
 */
void ISR_exc_Debug(void);

/**
 * @brief Called by the processeor when a NonMaskableInterrupt exception occured
 */
void ISR_exc_NonMaskableInterrupt(void);

/**
 * @brief Called by the processeor when a 'Breakpoint' exception occured
 */
void ISR_exc_Breakpoint(void);

/**
 * @brief Called by the processeor when an 'Overflow' exception occured
 */
void ISR_exc_Overflow(void);

/**
 * @brief Called by the processeor when a 'Out of bounds' exception occured
 */
void ISR_exc_OutOfBounds(void);

/**
 * @brief Called by the processeor when a 'Invalid opcode' exception occured
 */
void ISR_exc_InvalidOpCode(void);

/**
 * @brief Called by the processeor when a 'No co-processor' exception occured
 */
void ISR_exc_No387(void);

/**
 * @brief Called by the processeor when a 'Double fault' exception occured
 */
void ISR_exc_DoubleFault(UINT32 err_num);

/**
 * @brief Called by the processeor when a '387SegOverrun' exception occured
 */
void ISR_exc_387SegOverrun(void);

/**
 * @brief Called by the processeor when a 'BadTSS' exception occured
 */
void ISR_exc_BadTSS(UINT32 err_num);

/**
 * @brief Called by the processeor when a 'Segment not present' exception occured
 */
void ISR_exc_SegmentNotPresent(UINT32 err_num);

/**
 * @brief Called by the processeor when a 'Stack fault' exception occured
 */
void ISR_exc_StackFault(UINT32 err_num);

/**
 * @brief Called by the processeor when a 'General protection fault' exception occured
 */
void ISR_exc_GeneralProtectionFault(UINT32 err_num);

/**
 * @brief Called by the processeor when a 'Page fault' exception occured
 */
void ISR_exc_PageFault(UINT32 err_num);

/**
 * @brief Called by the processeor when a 'Unknown interrupt' exception occured
 */
void ISR_exc_UnknownInt(void);

/**
 * @brief Called by the processeor when a co-processor exception occured
 */
void ISR_exc_387Fault(void);

/**
 * @brief Called by the processeor when a 'Alignment' exception occured
 */
void ISR_exc_Alignment(void);

/**
 * @brief Called by the processeor when a machine-exception occured
 */
void ISR_exc_Machine(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED19(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED20(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED21(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED22(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED23(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED24(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED25(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED26(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED27(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED28(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED29(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED30(void);

/**
 * @brief Reserved exception for the processor
 */
void ISR_exc_RESERVED31(void);


#endif
