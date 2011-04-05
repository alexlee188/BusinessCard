#ifndef _MAIN_H_
#define _MAIN_H_

#define TICKS_PER_SAMPLE 10000 /* 30 seconds at 4MHz Clock Freq */
#define FIRMWARE_VERSION "1.0"

extern unsigned char data_file;
extern unsigned long eeprom_array_size;
extern unsigned long eeprom_array_used;
extern unsigned long eeprom_next_sample;
void eeprom_erase(void);
unsigned char eeprom_read_record(unsigned long sample_id, int * sample_a, int * sample_b, unsigned char * data_file);

#endif
