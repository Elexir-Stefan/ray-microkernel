/**
 * @file system.c
 * @author Stefan Nuernberger
 * @date 2012-04-01
 * @brief System Management Functions (Reset, ... etc.)
 */

#include <typedefs.h>
#include <mtask/task_types.h>
#include <app/tdm.h>
#include <hal/system.h>
#include <hal/barekbd.h>
#include <hal/idt.h>


#include <debug.h>


RAY_TDM SystemHardResetUser(PROCESS_PTR process, UINT32 method)
{
	PROCESS_PTR parent = process->threadParent;

	// privilege check
	// FIXME Change that to something meaningful
	if (parent->privLevel <= 200)
	{
		return SystemHardReset(method);
		return TDM_SUCCESS;
	}
	else
	{
		// Not enough privileges

		ASSERT(FALSE);
		return TDM_INSUFFICIENT_RIGHTS;
	}
}

/**
 * Resets the system by using the 8042 keyboard controller to pulse the CPU's RESET pin
 * @return
 */
BOOL SystemResetByKeyboard(void)
{
	UINT8 temp;

	asm volatile ("cli"); /* disable all interrupts */

	/* Clear all keyboard buffers (output and command buffers) */
	do
	{
		temp = InPortB(KBRD_INTRFC); /* empty user data */
		if (check_flag(temp, KBRD_BIT_KDATA) != 0)
			InPortB(KBRD_IO); /* empty keyboard data */
	} while (check_flag(temp, KBRD_BIT_UDATA) != 0);

	OutPortB(KBRD_INTRFC, KBRD_RESET); /* pulse CPU reset line */

	return FALSE; // did not succeed, obviously
}

/**
 * Resets the system by loading a size 0 IDT and issue an interrupt (-> triple fault and reset)
 * @return
 */
BOOL SystemResetByIDT(void)
{
	IDT_PTR zeroSize;

	zeroSize.base = 0;
	zeroSize.limit = 0;

	IDTLoad(&zeroSize);

	// Issue a non-maskable interrupt
	// Division by zero
	int zero = 0;
	int y = 1;
	y = y / zero;

	return FALSE; // did not succeed, obviously
}

/**
 * Halts the system forever by disabling interrupts and then halting the CPU
 * @return
 */
BOOL SystemResetByHLT(void)
{
	asm volatile ("cli; hlt");

	return FALSE; // did not succeed, obviously
}

RAY_TDM SystemHardReset(UINT32 method)
{
	BOOL result = TRUE;

	if (method & RESET_METHOD_KBD)
	{
		result &= SystemResetByKeyboard();

	}

	if (method & RESET_METHOD_IDT)
	{
		result &= SystemResetByIDT();
	}

	if (method & RESET_METHOD_HLT)
	{
		result &= SystemResetByHLT();
	}

	if (result)
	{
		return TDM_SUCCESS;
	}
	else
	{
		return TDM_ILLEGAL_ARGUMENT;
	}
}
