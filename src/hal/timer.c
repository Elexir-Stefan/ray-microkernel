/**
 * @file timer.c
 * @author Stefan Nürnber [NURNware Technologies] stefan@nurnware.de
 * @brief Timer functions for the PIT
 */

#include <typedefs.h>
#include <hal/timer.h>
#include <hal/timer_ctrl.h>
#include <hal/io.h>
#include <exceptions.h>

void KernelTimerSetup(UINT8 timer, UINT32 intHz) {
	UINT16 timerDivisor;
	UINT16 timerPort;
	UINT8 timerCommand;
	timerDivisor = PIT_FREQU / intHz;

	switch (timer) {
	case 0:
		timerPort = PIT_TMR0;
		timerCommand = PIT_RW_LSBMSB | PIT_COUNTER0 | PIT_MODE_SQUARE;
		break;
	case 1:
		timerPort = PIT_TMR1;
		timerCommand = PIT_RW_LSBMSB | PIT_COUNTER1 | PIT_MODE_SQUARE;
		break;
	case 2:
		timerPort = PIT_TMR2;
		timerCommand = PIT_RW_LSBMSB | PIT_COUNTER2 | PIT_MODE_SQUARE;
		break;
	default:
		KernelThrowException("KernelTimerSetup: Wrong timer index!", EXC_ILLEGAL_STATE, 0, 0, 0);
		return; // keep compiler happy about uninitialized variables
	}

	/* send the write-lsb-then-msb-command to the PIC */
	OutPortB (PIT_CTRL_PORT, timerCommand);

	/* write LSB and then MSB */
	OutPortB (timerPort, timerDivisor & 0xff);
	OutPortB (timerPort, timerDivisor >> 8);

}

UINT16 TimerReadCounter(UINT8 channel) {
	UINT8 cLo, cHi;

	OutPortB(PIT_CTRL_PORT, (channel << 6) & 0xff);
	cLo = InPortB(PIT_TMR0 + channel);
	cHi = InPortB(PIT_TMR0 + channel);

	return (cHi << 8) + cLo;

}

DONTPROFILE UINT64 ReadTSC() {
	UINT32 eax, edx;
	UINT64 result;

	__asm__ __volatile__("rdtsc":"=a"(eax), "=d"(edx));
	result = edx;
	result <<= 32;
	return result + eax;
}

void ManualTimerInterrupt() {
	asm ("int $32");
}
