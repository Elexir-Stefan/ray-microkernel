/**
 * @file atom.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Di Mär 11 2008
 * @brief Provides functions to execute code in an atomic way
 */

#include <typedefs.h>
#include <hal/atom.h>
#include <hal/int.h>

UINT32 atomicCounter = 0;

DONTPROFILE inline void AtomicSectionStart() {
	/*if (atomicCounter == 0) {
		KernelInterruptsDisable();
	}
	atomicCounter++;
	*/
}

DONTPROFILE inline void AtomicSectionEnd() {
	/*atomicCounter--;
	if (atomicCounter == 0) {
		KernelInterruptsEnable();
	}
	*/
}
