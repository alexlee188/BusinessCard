#ifndef __UART_H_
#define __UART_H_

#define UART_BUFFER_SIZE 128

struct uart_buffer {
	unsigned char bytes[UART_BUFFER_SIZE];
	unsigned char length;
	unsigned char start;
};

void uart1_init(unsigned int baud, char rx_priority, char tx_priority);
unsigned char uart1_tx_bytes(unsigned char * bytes, unsigned char length);
unsigned char uart1_tx_str(const char * str);
unsigned char uart1_tx_byte(unsigned char byte);
unsigned char uart1_rx_byte(unsigned char * byte_ptr);
unsigned char uart1_rx_bytes(unsigned char * bytes, unsigned char length);
unsigned char uart1_tx_length();
unsigned char uart1_rx_length();
	
void _ISR __attribute__((auto_psv)) _U1RXInterrupt(void);
void _ISR __attribute__((auto_psv)) _U1TXInterrupt(void);

#endif
