#include <p24FJ64GA002.h>
#include "uart.h"

volatile struct uart_buffer buf_rx;
volatile struct uart_buffer buf_tx;

void _ISR __attribute__((auto_psv)) _U1RXInterrupt(void)
{
	unsigned char byte;
	IFS0bits.U1RXIF = 0;
	
	while (U1STAbits.URXDA)
	{
		byte = U1RXREG;
		if (buf_rx.length < UART_BUFFER_SIZE)
		{
			if (buf_rx.length + buf_rx.start >= UART_BUFFER_SIZE)
			{
				buf_rx.bytes[(buf_rx.length + buf_rx.start) - UART_BUFFER_SIZE] = byte;
			} else
			{
				buf_rx.bytes[buf_rx.length + buf_rx.start] = byte;
			}
			buf_rx.length++;
		}
	}
}

void _ISR __attribute__((auto_psv)) _U1TXInterrupt(void)
{
	IFS0bits.U1TXIF = 0;
	
	while (!U1STAbits.UTXBF && buf_tx.length > 0)
	{
		U1TXREG = buf_tx.bytes[buf_tx.start];
		buf_tx.start++;
		buf_tx.length--;
		if (buf_tx.start >= UART_BUFFER_SIZE)
		{
			buf_tx.start = 0;
		}
	}
}

unsigned char uart1_tx_bytes(unsigned char * bytes, unsigned char length)
{
	unsigned char i;
	if (buf_tx.length + length >= UART_BUFFER_SIZE)
	{
		return 0xFF;
	}
	
	for(i = 0; i < length; ++i)
	{
		if (uart1_tx_byte(bytes[i]))
		{
			return i + 1;
		}
	}
	
	return 0;
}

unsigned char uart1_tx_str(const char * str)
{
	unsigned char i = 0;
	
	while (str[i] != 0)
	{
		if (uart1_tx_byte(str[i]))
		{
			return i + 1;
		}
		i++;
	}
	
	return 0;
}

unsigned char uart1_tx_byte(unsigned char byte)
{
	if (buf_tx.length >= UART_BUFFER_SIZE)
	{
		return 1;
	}
	
	if (buf_tx.length + buf_tx.start >= UART_BUFFER_SIZE)
	{
		buf_tx.bytes[(buf_tx.length + buf_tx.start) - UART_BUFFER_SIZE] = byte;
	} else
	{
		buf_tx.bytes[buf_tx.length + buf_tx.start] = byte;
	}
	buf_tx.length++;
	
	if (U1STAbits.TRMT)
	{
		U1TXREG = buf_tx.bytes[buf_tx.start];
		buf_tx.length--;
		buf_tx.start++;
		if (buf_tx.start >= UART_BUFFER_SIZE)
		{
			buf_tx.start = 0;
		}
	}
	
	return 0;
}

unsigned char uart1_rx_byte(unsigned char * byte_ptr)
{
	if (buf_rx.length == 0)
	{
		return 1;
	}
	
	(*byte_ptr) = buf_rx.bytes[buf_rx.start];
	buf_rx.start++;
	buf_rx.length--;
	if (buf_rx.start >= UART_BUFFER_SIZE)
	{
		buf_rx.start = 0;
	}
	
	return 0;
}

unsigned char uart1_rx_bytes(unsigned char * bytes, unsigned char length)
{
	unsigned char i, byte;
	
	if (buf_rx.length < length)
	{
		return 0xFF;
	}
	
	for(i = 0; i < length; ++i)
	{
		if (uart1_rx_byte(&byte))
		{
			return i + 1;
		}
		bytes[i] = byte;
	}
	
	return 0;
}

unsigned char uart1_tx_length()
{
	return buf_tx.length;
}

unsigned char uart1_rx_length()
{
	return buf_rx.length;
}

void uart1_init(unsigned int baud, char rx_priority, char tx_priority)
{
	buf_rx.start = 0;
	buf_rx.length = 0;
	buf_tx.start = 0;
	buf_tx.length = 0;
	
	IPC2bits.U1RXIP = rx_priority;
	IPC3bits.U1TXIP = tx_priority;
	IEC0bits.U1RXIE = 1;
	IEC0bits.U1TXIE = 1;
	U1STAbits.UTXISEL0 = 1;
	U1BRG = baud;
	U1MODEbits.BRGH = 1;
	U1MODEbits.UARTEN = 1;
	U1STAbits.UTXEN = 1;
}


