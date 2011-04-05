#ifndef __I2C_PIC24_H_
#define __I2C_PIC24_H_

#if defined (__PIC24FJ64GA002__)
	#define __CONFIG_1_ADDR 0x00ABFE
	#define __CONFIG_2_ADDR 0x00ABFC
#elif defined (__PIC24FJ48GA002__)
	#define __CONFIG_1_ADDR 0x0083FE
	#define __CONFIG_2_ADDR 0x0083FC
#elif defined (__PIC24FJ32GA002__)
	#define __CONFIG_1_ADDR 0x0057FE
	#define __CONFIG_2_ADDR 0x0057FC
#elif defined (__PIC24FJ26GA002__)
	#define __CONFIG_1_ADDR 0x002BFE
	#define __CONFIG_2_ADDR 0x002BFC
#else
	#error "I2C Module Requires a PIC24FJxxGA002"
#endif

// define some baud rate constants for various common MIPS and Hz
#define I2C_BAUD_100K_16 0x4f
#define I2C_BAUD_100K_8 0x27
#define I2C_BAUD_100K_4 0x13
#define I2C_BAUD_400K_16 0x13
#define I2C_BAUD_400K_8 0x09
#define I2C_BAUD_400K_4 0x04
#define I2C_BAUD_1M_16 0x07
#define I2C_BAUD_1M_8 0x03

#define INCLUDE_I2C1_PIC24	
#define INCLUDE_I2C2_PIC24

#ifdef INCLUDE_I2C1_PIC24
	void i2c1_init(unsigned char baud);
	void i2c1_start();
	void i2c1_stop();
	void i2c1_restart();
	void i2c1_send_ack();
	void i2c1_send_nack();
	char i2c1_send_address(unsigned char addr, char read);
	unsigned char i2c1_send_byte(unsigned char b);
	unsigned char i2c1_read_byte();
	unsigned char * i2c1_read_bytes(unsigned char * bytes, int length);
#endif

#ifdef INCLUDE_I2C2_PIC24
	void i2c2_init(unsigned char baud);
	void i2c2_start();
	void i2c2_stop();
	void i2c2_restart();
	void i2c2_send_ack();
	void i2c2_send_nack();
	char i2c2_send_address(unsigned char addr, char read);
	unsigned char i2c2_send_byte(unsigned char b);
	unsigned char i2c2_read_byte();
	unsigned char * i2c2_read_bytes(unsigned char * bytes, int length);
#endif

#endif
