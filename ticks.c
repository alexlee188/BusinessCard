#include <p24FJ64GB002.h>
#include "ticks.h"
#include "util.h"

volatile unsigned long long system_ms_count = 0;

void _ISRFAST __attribute__((no_auto_psv)) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;
	system_ms_count++;
}

unsigned long long ticks()
{
	unsigned long long cur_ticks;
	INTERRUPT_PROTECT(cur_ticks = system_ms_count);
	return cur_ticks;
}

void ticks_init()
{
	// start system timer - 1 ms resolution
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	IPC0bits.T1IP = 0x07;
	T1CONbits.TCKPS = 0x00;
	PR1 = 2000;
	T1CONbits.TON = 1;
}

void delay_ticks(unsigned int delay)
{
	unsigned int timer;
	unsigned long long cur_ticks, delay_till;
	timer = TMR1;
	cur_ticks = ticks();
	delay_till = cur_ticks + delay;
	
	while (ticks() < delay_till || TMR1 < timer) {}
}
