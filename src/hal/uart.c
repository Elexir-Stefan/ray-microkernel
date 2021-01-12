/**
 * @file uart.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 12 2008
 * @see uart.h
 * @brief UART (serial device) communication primitives (for debugging purposes only!)
 */

#if defined(RAY_KERNEL_DEBUGGER) || defined(RAYPROFILING) || 1

#include <typedefs.h>
#include <hal/uart.h>
#include <hal/io.h>
#include <exceptions.h>
#include <video/video_char.h>
#include <hal/8259.h>

static COM_PORT *currentComPort;

void SetDebugComPort(COM_PORT* port) {
	currentComPort = port;
}

void InitUARTPort(COM_PORT* port, UINT32 baud, UARTDataBits dataBits, UARTStopBits stopBits, UARTParity parity) {

	UINT8 divisor;
	UINT8 bitSettings;
	switch (baud) {
		case 115200: divisor = 1; break;
		case 57600:  divisor = 2; break;
		case 38400:  divisor = 3; break;
		case 28800:  divisor = 4; break;
		case 19200:  divisor = 6; break;
		case 14400:  divisor = 8; break;
		case 9600:   divisor = 12; break;
		case 4800:   divisor = 24; break;
		default:
			KernelThrowException("Wrong baud rate specified.", EXC_ILLEGAL_STATE, baud, 0, 0);
			return;
	}

	switch (dataBits) {
		case UART5Bits: bitSettings = 0x00; break;
		case UART6Bits: bitSettings = 0x01; break;
		case UART7Bits: bitSettings = 0x02; break;
		case UART8Bits: bitSettings = 0x03; break;
		default:
			KernelThrowException("Wrong amount of data bits specified.", EXC_ILLEGAL_STATE, dataBits, 0, 0);
			return;
	}

	switch (stopBits) {
		case UARTOneStop:        bitSettings |= 0x00; break;
		case UARTOneAndHalfStop: bitSettings |= 0x04; break;
		case UARTTwoStop:        bitSettings |= 0x04; break;
		default:
			KernelThrowException("Wrong amount of stop bits specified.", EXC_ILLEGAL_STATE, stopBits, 0, 0);
			return;
	}

	switch (parity) {
		case UARTNoParity:   bitSettings |= 0x00; break;
		case UARTEvenParity: bitSettings |= UART_PARITY_ENABLE | UART_PARITY_EVEN; break;
		case UARTOddParity:  bitSettings |= UART_PARITY_ENABLE | UART_PARITY_ODD; break;
		default:
			KernelThrowException("Wrong parity specified.", EXC_ILLEGAL_STATE, parity, 0, 0);
	}

	OutPortB(port->ioBase + UART_INT_ENABLE_REG, 0x00);	// disable interrupts
	OutPortB(port->ioBase + UART_LINE_CTRL_REG, UART_ENABLE_DLAB);	// enable DLAB (set baud rate divisor)

	// set baud rate divisor
	OutPortB(port->ioBase , divisor);	// set baud rate (low byte divisor)
	OutPortB(port->ioBase + 1, 0x00);	// set baud rate (high byte divisor)

	// set data bits, stop bits and parity
	OutPortB(port->ioBase + 3, bitSettings);

	// set fifo to 14 bytes
	OutPortB(port->ioBase + UART_FIFO_CTRL_REG, 0xC7);

	// turn off DTR, RTS and OUT2
	OutPortB(port->ioBase + UART_MODEM_CTRL_REG, UART_DTR | UART_RTS | UART_OUT2);

	// enable interrupts
	//OutPortB(port->ioBase + UART_INT_ENABLE_REG, 0x01);
	//C8259UnmaskIRQ(port->irq);

	port->isReading = FALSE;

}

static BOOL UARTReceived() {
	return InPortB(currentComPort->ioBase + UART_LINE_STATUS_REG) & 1;
}

static BOOL UARTTransmitEmpty(COM_PORT* port) {
	return InPortB(port->ioBase + UART_LINE_STATUS_REG) & 0x20;
}

int getDebugChar() {
	currentComPort->isReading = TRUE;
	while (!UARTReceived()) {
	}
	int c = InPortB(currentComPort->ioBase);
	currentComPort->isReading = FALSE;
	return c;
}

void putDebugChar(int b) {
	while(!UARTTransmitEmpty(currentComPort));
	OutPortB(currentComPort->ioBase, b);
}

void UARTWriteByte(COM_PORT* comport, char byte) {
	while(!UARTTransmitEmpty(comport));
		OutPortB(comport->ioBase, byte);
}

void UARTWriteString(char *string) {
	while(*string) {
		putDebugChar(*string++);
	}
}

#endif
