#include <p24FJ64GB002.h>
#include "main.h"

_CONFIG2(IESO_OFF & PLL96MHZ_ON & FNOSC_FRCDIV & FCKSM_CSECMD & OSCIOFNC_ON & IOL1WAY_ON & I2C1SEL_PRI & POSCMOD_NONE)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS1)


void init(void)
{
    AD1PCFGL = 0xFFFF;
	{
    unsigned int pll_startup_counter = 600;
    CLKDIVbits.PLLEN = 1;
    while(pll_startup_counter--);
	}

	TRISA = 0;
	_LATA2 = 0;
    _LATA3 = 1;
	
}

int main(void)
{
long i;

	init();
			
	while (1){
			for (i=0; i < 50000; i++){
			_LATA4 = 0;
			}
		_LATA2 = ! _LATA2;
		_LATA3 = ! _LATA3;
	}

	
	return 0;
}
