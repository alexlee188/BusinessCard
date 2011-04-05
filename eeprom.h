#ifndef _EEPROM_H_
#define _EEPROM_H_

#define EEPROM_WP _LATB5
#define EEPROM_WP_TRIS _TRISB5

unsigned char eeprom_init(unsigned long * array_size);
unsigned char eeprom_write_byte(unsigned long address, unsigned char byte);
unsigned char eeprom_read_byte(unsigned long address, unsigned char * byte);
unsigned char eeprom_read_bytes(unsigned long address, unsigned char * bytes, unsigned long length);
unsigned char eeprom_alter_array_size(unsigned long new_size);


#endif
