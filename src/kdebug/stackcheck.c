/*
 * stackcheck.c
 *
 *  Created on: 07.03.2009
 *      Author: stefan
 */

#include "typedefs.h"
#include "exceptions.h"

void __stack_chk_fail() {
	for(;;);
	//KernelThrowException("Stack check failed!", EXC_ILLEGAL_STATE, 0, FALSE, NULL);
}
