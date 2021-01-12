#ifndef _IRQ_HAN_H
#define _IRQ_HAN_H

/**
 * @file irq_han.h
 * @author Stefan Nuernberger
 * @date 02-25-2006
 * @brief IRQ handler
 * Functions to assiciate interrupts handlers with a certain IRQ
 */

#include "mtask/task_types.h"
#include "app/callbacks.h"
#include "app/tdm.h"

typedef struct {
	UINT32 numPending;
	PROCESS_PTR thread;
} REGISTERED_HANDLER;

/**
 * @brief Called from the standard IRQ-handler in assembler
 */
void IRQHandler();

/**
 * @brief Assigns a function to a certain IRQ as handler
 * @param irqNum IRQ number where the function will be registered
 * @param thread The thread which will be used as a dispatcher
 */
RAY_TDM IRQRegisterHandler(UINT8 irqNum, PROCESS_PTR thread);



/**
 * This function is called in a loop in user mode and waits/blocks
 * until an IRQ has arrived.  This function has two purposes:
 * a) The first call enables the IRQ to be delivered (unmasked)
 * b) subsequent calls unmask the IRQ again and cen therefore
 * receive further IRQs
 * @param irqNum The IRQ number to wait for. Must be the one that has been
 * registered for this @a thread.
 * @param thread
 */
RAY_TDM IRQAcknowledgeAndWait(UINT8 irqNum, PROCESS_PTR thread);

/**
 * @brief Looses the binding between a function and an IRQ
 * @param irqNum IRQ handler number to delete
 * @param thread The thread that registered the IRQ
 */
RAY_TDM IRQUnregisterHandler(UINT8 irqNum, PROCESS_PTR thread);

/**
 * initializes the irq handler
 */
void IRQHandlerInit(void);


/**
 * Removes all callbacks for the thread specified
 * @param thread which's registered callbacks should be removed
 * @return FALSE if no callbacks were registered, TRUE otherwise
 */
BOOL IRQUnregisterThread(PROCESS_PTR thread);


#endif
