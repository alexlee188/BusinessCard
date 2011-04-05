# MPLAB IDE generated this makefile for use with GNU make.
# Project: Data Logger (EEPROM).mcp
# Date: Mon May 12 00:39:19 2008

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

Data\ Logger\ (EEPROM).hex : Data\ Logger\ (EEPROM).cof
	$(HX) "Data Logger (EEPROM).cof"

Data\ Logger\ (EEPROM).cof : main.o ticks.o
	$(CC) -mcpu=24FJ64GA002 "main.o" "ticks.o" -o"Data Logger (EEPROM).cof" -Wl,-L"C:\Program Files\Microchip\MPLAB C30\lib",--script="C:\Program Files\Microchip\MPLAB C30\support\PIC24F\gld\p24FJ64GA002.gld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,-Map="Data Logger (EEPROM).map",--report-mem

main.o : ticks.h c:/program\ files/microchip/mplab\ c30/bin/bin/../../support/PIC24F/h/p24FJ64GA002.h main.c
	$(CC) -mcpu=24FJ64GA002 -x c -c "main.c" -o"main.o" -D__DEBUG -g -Wall

ticks.o : util.h ticks.h c:/program\ files/microchip/mplab\ c30/bin/bin/../../support/PIC24F/h/p24FJ64GA002.h ticks.c
	$(CC) -mcpu=24FJ64GA002 -x c -c "ticks.c" -o"ticks.o" -D__DEBUG -g -Wall

clean : 
	$(RM) "main.o" "ticks.o" "Data Logger (EEPROM).cof" "Data Logger (EEPROM).hex"

