#include <p24FJ64GA002.h>
#include "main.h"
#include "ticks.h"
#include "i2c.h"
#include "eeprom.h"
#include "uart.h"
#include "console.h"

_CONFIG2(IESO_OFF & FNOSC_FRCDIV & FCKSM_CSECMD & OSCIOFNC_ON & IOL1WAY_ON & I2C1SEL_PRI & POSCMOD_NONE)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS1)

#define CONFIG_SIZE 4

unsigned char data_file = 0;
unsigned long eeprom_array_size = 0;
unsigned long eeprom_array_used = 0;
unsigned long eeprom_next_sample = 0;

void eeprom_erase(void)
{
	eeprom_write_byte(0x00, 0);
	eeprom_write_byte(0x01, 0);
	eeprom_write_byte(0x02, 0);
	eeprom_write_byte(0x03, 0);
}

unsigned char eeprom_write_record(unsigned long sample_id, int sample_a, int sample_b)
{
	unsigned long byte_addr = (sample_id * 3) + CONFIG_SIZE;
	
	if (data_file > 0x0F)
	{
		return 1;
	}
	
	eeprom_write_byte(byte_addr, (sample_a >> 2) & 0x00FF);
	eeprom_write_byte(byte_addr + 1, ((sample_a << 6) & 0x00FF) + ((sample_b >> 4) & 0x00FF));
	eeprom_write_byte(byte_addr + 2, ((sample_b << 4) & 0x00FF) + data_file);
	
	return 0;
}

unsigned char eeprom_read_record(unsigned long sample_id, int * sample_a, int * sample_b, unsigned char * data_file)
{
	unsigned long byte_addr = (sample_id * 3) + CONFIG_SIZE;
	unsigned char buf[3];
	
	eeprom_read_bytes(byte_addr, buf, 3);
	
	*sample_a = (buf[0] << 2) + (buf[1] >> 6);
	*sample_b = ((buf[1] & 0x3F) << 4) + (buf[2] >> 4);
	*data_file = buf[2] & 0x0F;
	
	return 0;
}

void init(void)
{
	unsigned char config[4];
	ticks_init();
	
	// initialize IO pin mappings
	RPOR7bits.RP15R = 18; // OC1 on RP15
	RPINR18bits.U1RXR = 11; // UART1 RX on RP11
	RPOR5bits.RP10R = 3; // UART1 TX on RP10
	OSCCONbits.IOLOCK = 1;
	
	EEPROM_WP = 1;
	EEPROM_WP_TRIS = 0;
	_LATB7 = 0;
	_TRISB7 = 0;
	
	PR2 = 0x03ff;
	OC1R = 0x03FF;
	OC1RS = 0x03FF;
	OC1CONbits.OCM = 0x06;
	T2CONbits.TON = 1;
	
	AD1PCFGbits.PCFG4 = 1;
	AD1PCFGbits.PCFG5 = 1;
	AD1CHS = 0x000B;
	AD1CON1bits.SSRC = 0x7;
	AD1CON3bits.SAMC = 14;
	_ADCS = 0x0; // change me if we switch clock freq
	_ADON = 1;
	
	uart1_init(0x0C, 0x06, 0x05);
	console_init(0x03);
	
	i2c2_init(0x15);
	
	if (eeprom_init(&eeprom_array_size))
	{
		// Bad things, we can't find even one EEPROM block. 
		_LATB7 = 1;
		while (1) {
			delay_ticks(100);
			_LATB7 = !_LATB7;
		}
	}
	
	eeprom_read_bytes(0, config, 4);
	data_file = config[3];
	eeprom_array_used = (((unsigned long) config[0]) << 16) + (config[1] << 8) + config[2];
	eeprom_next_sample = eeprom_array_used / 3;
	
	if (data_file > 0x0E || eeprom_array_used + 3 >= eeprom_array_size - CONFIG_SIZE)
	{
		/* we are out of room, so we must enter UART I/O loop
			and wait to be emptied */
		_LATB7 = 1;
		while (1) {
			delay_ticks(1000);
			_LATB7 = !_LATB7;
		}
	} else 
	{
		eeprom_write_byte(0x03, data_file + 1);
	}
}

int main(void)
{
	unsigned long long next_sample_at = 0;
	unsigned int light = 0, temp = 0;
	init();
	
	next_sample_at = ticks() + TICKS_PER_SAMPLE;
	
	while (1)
	{
		AD1CHS = 0x000B;
		_SAMP = 1;
		while (_SAMP || !AD1CON1bits.DONE) {}
		light = ADC1BUF0;
		OC1RS = 1023 - light;
		if (ticks() >= next_sample_at)
		{
			AD1CHS = 0x000C;
			_SAMP = 1;
			while (_SAMP || !AD1CON1bits.DONE) {}
			temp = ADC1BUF0;
			
			IEC1bits.T4IE = 0;
			next_sample_at += TICKS_PER_SAMPLE;
			eeprom_write_record(eeprom_next_sample, light, temp);
			eeprom_next_sample++;
			eeprom_array_used += 3;
			if (eeprom_next_sample % 20 == 0)
			{
				eeprom_write_byte(0x00, eeprom_array_used >> 16);
				eeprom_write_byte(0x01, eeprom_array_used >> 8);
				eeprom_write_byte(0x02, eeprom_array_used);
			}
			
			//if there is no more room, save our position and signal we need to be emptied
			if (eeprom_array_used + 3 >= eeprom_array_size - CONFIG_SIZE)
			{
				eeprom_write_byte(0x00, eeprom_array_used >> 16);
				eeprom_write_byte(0x01, eeprom_array_used >> 8);
				eeprom_write_byte(0x02, eeprom_array_used);
				IEC1bits.T4IE = 1;
				
				while (1) {
					delay_ticks(1000);
					_LATB7 = !_LATB7;
				}
			}
			IEC1bits.T4IE = 1;
		}
	}
	
	return 0;
}
