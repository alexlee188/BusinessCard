#include <p24FJ64GA002.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "console.h"
#include "main.h"
#include "eeprom.h"

const char * console_str_boot __attribute__ ((space(auto_psv))) = "Light And Temp v" FIRMWARE_VERSION NEWLINE;
const char * console_str_about __attribute__ ((space(auto_psv))) = "Created by Edward Holets" NEWLINE "Copyright 2008" NEWLINE;
const char * console_str_help __attribute__ ((space(auto_psv))) = "A - About" NEWLINE "? - Help" NEWLINE "E - Erase" NEWLINE "D - Dump" NEWLINE "S - Stats" NEWLINE "R - Reset" NEWLINE "X - eXpand" NEWLINE;
const char * console_str_expand __attribute__ ((space(auto_psv))) = "Enter new EEPROM array size ";
const char * console_cmd_gohome __attribute__ ((space(auto_psv))) = "\x1B[0;0H";
const char * console_cmd_clear_screen __attribute__ ((space(auto_psv))) = "\x1B[2J";
const char * console_cmd_prompt __attribute__ ((space(auto_psv))) = ">\x1B[K\x1B[J";

#define CMD_NONE 0
#define CMD_EXPAND 1

void console_loop(void)
{
	static unsigned char byte = 0, cmd = CMD_NONE, byte_count = 0;
	static char buf[32];
	unsigned long i;
	int light, temp;
	unsigned char file;
	
	while (uart1_rx_byte(&byte) == 0)
	{
		if (cmd == CMD_NONE)
		{
			uart1_tx_str(NEWLINE);
			if (byte == 'A')
			{
				uart1_tx_str(console_str_about);
			} else if (byte == '?')
			{
				uart1_tx_str(console_str_help);
			} else if (byte == 'S')
			{
				sprintf(buf, "Size: %lu" NEWLINE, eeprom_array_size);
				uart1_tx_str(buf);
				sprintf(buf, "Used: %lu" NEWLINE, eeprom_array_used);
				uart1_tx_str(buf);
				sprintf(buf, "Runs: %u" NEWLINE, data_file);
				uart1_tx_str(buf);
			} else if (byte == 'R')
			{
				__asm__ volatile ("reset");
			} else if (byte == 'E')
			{
				eeprom_erase();
				__asm__ volatile ("reset");
			} else if (byte == 'X')
			{
				cmd = CMD_EXPAND;
				uart1_tx_str(console_str_expand);
			} else if (byte == 'D')
			{
				for(i = 0; i < eeprom_next_sample; ++i)
				{
					eeprom_read_record(i, &light, &temp, &file);
					sprintf(buf, "0x%X 0x%X 0x%X" NEWLINE, light, temp, (unsigned int) file);
					uart1_tx_str(buf);
				}
				uart1_tx_str("Done." NEWLINE);
			}
			uart1_tx_str(console_cmd_prompt);
		} else if (cmd == CMD_EXPAND)
		{
			if (byte != '\r' && byte_count < 20)
			{
				uart1_tx_byte(byte);
				buf[byte_count] = byte;
				byte_count++;
			} else
			{
				buf[byte_count] = 0;
				byte_count = 0;
				eeprom_array_size = atol(buf);
				eeprom_alter_array_size(eeprom_array_size);
				eeprom_erase();
				__asm__ volatile ("reset");
			}
		}
	}
}

void _ISR __attribute ((auto_psv)) _T4Interrupt(void)
{
	IFS1bits.T4IF = 0;
	console_loop();
}

void console_init(unsigned char priority)
{
	uart1_tx_str(console_cmd_clear_screen);
	uart1_tx_str(console_cmd_gohome);
	uart1_tx_str(console_str_boot);
	uart1_tx_str(console_cmd_prompt);
	
	// init console loop timer
	IFS1bits.T4IF = 0;
	IEC1bits.T4IE = 1;
	IPC6bits.T4IP = priority;
	T4CONbits.TCKPS = 0x00;
	PR4 = 1000;
	T4CONbits.TON = 1;
}

