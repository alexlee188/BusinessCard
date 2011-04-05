#include <p24FJ64GA002.h>
#include "i2c.h"

#ifdef INCLUDE_I2C1_PIC24
void i2c1_init(unsigned char baud)
{
	unsigned int config_2;
	// Ugh, datasheet says we dont need to worry about tristating the pins,
	// but it doesnt work otherwise it would seem. I must be missing something.
	TBLPAG = (__CONFIG_2_ADDR & 0xFF0000) >> 16;
	config_2 = __builtin_tblrdl(__CONFIG_2_ADDR);
	if (config_2 & 0x0004)
	{
		TRISBbits.TRISB8 = 0;
		TRISBbits.TRISB9 = 0;
	} else
	{
		TRISBbits.TRISB5 = 0;
		TRISBbits.TRISB6 = 0;
	}
	
	
	I2C1BRG = baud;
	I2C1CONbits.I2CEN = 1;
}

void i2c1_start()
{
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN)
	{
		;
	}
}


void i2c1_stop()
{
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN)
	{
		;
	}
}

void i2c1_restart()
{
	I2C1CONbits.RSEN = 1;
	while(I2C1CONbits.RSEN)
	{
		;
	}
}

void i2c1_send_ack()
{
	I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN)
	{
		;
	}
}

void i2c1_send_nack()
{
	I2C1CONbits.ACKDT = 1;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN)
	{
		;
	}
}

char i2c1_send_address(unsigned char addr, char read)
{
	I2C1TRN = ((addr << 1) | (read != 0));
	while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF)
	{
		;
	}
	
	return I2C1STATbits.ACKSTAT;
}

unsigned char i2c1_send_byte(unsigned char b)
{
	I2C1TRN = b;
	while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF)
	{
		;
	}
	
	return I2C1STATbits.ACKSTAT;
}

unsigned char i2c1_read_byte()
{
	unsigned char b;
	
	I2C1CONbits.RCEN = 1;
	while (I2C1CONbits.RCEN)
	{
		;
	}
	b = I2C1RCV;
	
	return b;
}

unsigned char * i2c1_read_bytes(unsigned char * bytes, int length)
{
	unsigned int i;
	
	for(i = 0; i < length; ++i)
	{
		bytes[i] = i2c1_read_byte();
		if (i == length - 1)
		{
			i2c1_send_nack();
		} else
		{
			i2c1_send_ack();
		}
	}
	
	return bytes;
}
#endif

#ifdef INCLUDE_I2C2_PIC24
void i2c2_init(unsigned char baud)
{
	// Ugh, datasheet says we dont need to worry about tristating the pins,
	// but it doesnt work otherwise it would seem. I must be missing something.
	//TRISBbits.TRISB2 = 0;
	//TRISBbits.TRISB3 = 0;
	
	
	I2C2BRG = baud;
	I2C2CONbits.I2CEN = 1;
}

void i2c2_start()
{
	I2C2CONbits.SEN = 1;
	while(I2C2CONbits.SEN)
	{
		;
	}
}


void i2c2_stop()
{
	I2C2CONbits.PEN = 1;
	while(I2C2CONbits.PEN)
	{
		;
	}
}

void i2c2_restart()
{
	I2C2CONbits.RSEN = 1;
	while(I2C2CONbits.RSEN)
	{
		;
	}
}

void i2c2_send_ack()
{
	I2C2CONbits.ACKDT = 0;
	I2C2CONbits.ACKEN = 1;
	while (I2C2CONbits.ACKEN)
	{
		;
	}
}

void i2c2_send_nack()
{
	I2C2CONbits.ACKDT = 1;
	I2C2CONbits.ACKEN = 1;
	while (I2C2CONbits.ACKEN)
	{
		;
	}
}

char i2c2_send_address(unsigned char addr, char read)
{
	I2C2TRN = ((addr << 1) | (read != 0));
	while (I2C2STATbits.TRSTAT || I2C2STATbits.TBF)
	{
		;
	}
	
	return I2C2STATbits.ACKSTAT;
}

unsigned char i2c2_send_byte(unsigned char b)
{
	I2C2TRN = b;
	while (I2C2STATbits.TRSTAT || I2C2STATbits.TBF)
	{
		;
	}
	
	return I2C2STATbits.ACKSTAT;
}

unsigned char i2c2_read_byte()
{
	unsigned char b;
	
	I2C2CONbits.RCEN = 1;
	while (I2C2CONbits.RCEN)
	{
		;
	}
	b = I2C2RCV;
	
	return b;
}

unsigned char * i2c2_read_bytes(unsigned char * bytes, int length)
{
	unsigned int i;
	
	for(i = 0; i < length; ++i)
	{
		bytes[i] = i2c2_read_byte();
		if (i == length - 1)
		{
			i2c2_send_nack();
		} else
		{
			i2c2_send_ack();
		}
	}
	
	return bytes;
}
#endif
