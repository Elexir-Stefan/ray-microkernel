/**
 * @file uart.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mi Mär 12 2008
 * @see uart.c
 * @brief Prototypes for uart.c
 */



#ifndef _uart_H
#define _uart_H

#define UART_TR_BUFFER_REG	0
#define UART_INT_ENABLE_REG	1
#define UART_INT_IDENTIFY_REG	2
#define UART_FIFO_CTRL_REG	2
#define UART_LINE_CTRL_REG	3
#define UART_MODEM_CTRL_REG	4
#define UART_LINE_STATUS_REG	5
#define UART_MODEM_STATUS_REG	6
#define UART_SCRATCH_PAD_REG	7

// for control reg
#define UART_ENABLE_DLAB	0x80

// for modem control reg
#define UART_DTR		0x01
#define UART_RTS		0x02
#define UART_OUT1		0x04
#define UART_OUT2		0x08

#define UART_PARITY_ENABLE	0x08
#define UART_PARITY_EVEN	0x10
#define UART_PARITY_ODD		0x00



typedef struct {
	UINT16 ioBase;
	UINT8 irq;
	BOOL isReading;
} COM_PORT;

typedef enum {
	UART5Bits,
		UART6Bits,
		UART7Bits,
		UART8Bits
} UARTDataBits;

typedef enum {
	UARTOneStop,
	UARTOneAndHalfStop,
	UARTTwoStop
} UARTStopBits;

typedef enum {
	UARTNoParity,
	UARTEvenParity,
	UARTOddParity
} UARTParity;

void InitUARTPort(COM_PORT *port, UINT32 baud, UARTDataBits dataBits, UARTStopBits stopBits, UARTParity parity);
void UARTWriteString(char *string);

int getDebugChar();
void putDebugChar(int b);

void UARTWriteByte(COM_PORT* comport, char byte);

void SetDebugComPort(COM_PORT* port);

#endif	/* _uart_H */

