#include <p24FJ64GB002.h>
#include "main.h"
#include "ticks.h"
#include "eeprom.h"
#include "uart.h"
#include "console.h"

_CONFIG2(IESO_OFF & FNOSC_FRCDIV & FCKSM_CSECMD & OSCIOFNC_ON & IOL1WAY_ON & I2C1SEL_PRI & POSCMOD_NONE)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS1)

#define CONFIG_SIZE 4

unsigned char data_file = 0;
unsigned long eeprom_array_size = 0;
unsigned long eeprom_array_used = 0;
unsigned long eeprom_next_sample = 0;


void init(void)
{
//	ticks_init();
	
	// initialize IO pin mappings
	RPOR7bits.RP15R = 18; // OC1 on RP15
	RPINR18bits.U1RXR = 11; // UART1 RX on RP11
	RPOR5bits.RP10R = 3; // UART1 TX on RP10
	
	EEPROM_WP = 1;
	EEPROM_WP_TRIS = 0;
	
	PR2 = 0x03ff;
	OC1R = 0x03FF;
	OC1RS = 0x03FF;
	
	AD1PCFGbits.PCFG4 = 1;
	AD1PCFGbits.PCFG5 = 1;
	AD1CHS = 0x000B;
	AD1CON1bits.SSRC = 0x7;
	AD1CON3bits.SAMC = 14;
	_ADCS = 0x0; // change me if we switch clock freq
	_ADON = 1;

	_LATA2 = 0;
    _LATA3 = 0;
	
}

int main(void)
{
int i;

	init();
			
				for (i=0; i < 10000; i++){
					
					_LATA2 = !_LATA2;
					_LATA3 = !_LATA3;
				}

	
	return 0;
}
