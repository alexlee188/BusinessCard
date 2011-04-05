# MPLAB IDE generated this makefile for use with GNU make.
# Project: Data Logger (EEPROM).mcp
# Date: Tue Apr 05 19:23:11 2011

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

Data\ Logger\ (EEPROM).hex : Data\ Logger\ (EEPROM).cof
	$(HX) "Data Logger (EEPROM).cof"

Data\ Logger\ (EEPROM).cof : main.o
	$(CC) -mcpu=24FJ64GB002 "main.o" -o"Data Logger (EEPROM).cof" -Wl,-L"C:\Program Files\Microchip\MPLAB C30\lib",--script="C:\Program Files\Microchip\mplabc30\v3.25\support\PIC24F\gld\p24FJ64GB002.gld",--defsym=__MPLAB_BUILD=1,--heap=256,-Map="Data Logger (EEPROM).map",--report-mem

main.o : console.h uart.h eeprom.h ticks.h main.h c:/program\ files/microchip/mplabc30/v3.25/support/PIC24F/h/p24FJ64GB002.h main.c
	$(CC) -mcpu=24FJ64GB002 -x c -c "main.c" -o"main.o" -g -Wall

clean : 
	$(RM) "main.o" "Data Logger (EEPROM).cof" "Data Logger (EEPROM).hex"

