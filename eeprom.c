#include <p24FJ64GA002.h>
#include "i2c.h"
#include "eeprom.h"

#define EEPROM_CONFIG_SPACE 3 /* bytes */
#define eeprom_bus_address(global_byte_address) (0x50 + ((global_byte_address & 0x00010000) >> 14) + ((global_byte_address & 0x00060000) >> 17))
#define eeprom_block_last_byte(global_byte_address) ((global_byte_address & 0xFFFF0000) + 0xFFFF)

unsigned char __eeprom_write_raw_byte(unsigned long address, unsigned char byte);

unsigned char __eeprom_write_raw_byte(unsigned long address, unsigned char byte)
{
	unsigned char return_code = 0, bus_addy;
	bus_addy = eeprom_bus_address(address);
	i2c2_start();
	return_code |= i2c2_send_address(bus_addy, 0);
	if (return_code)
	{
		goto done;
	}
	return_code |= i2c2_send_byte(address >> 8);
	if (return_code)
	{
		goto done;
	}
	return_code |= i2c2_send_byte(address & 0x00FF);
	if (return_code)
	{
		goto done;
	}
	return_code |= i2c2_send_byte(byte);
	if (return_code)
	{
		goto done;
	}
	EEPROM_WP = 0;
	i2c2_stop();
	EEPROM_WP = 1;
	i2c2_start();
	while(i2c2_send_address(bus_addy, 0))
	{
		i2c2_restart();
	}
	
done:	
	i2c2_stop();	
	return return_code;
}

unsigned char eeprom_init(unsigned long * array_size)
{
	unsigned char eeprom_config[EEPROM_CONFIG_SPACE];
	
	i2c2_start();
	i2c2_send_address(eeprom_bus_address(0), 0);
	i2c2_send_byte(0x00);
	i2c2_send_byte(0x00);
	i2c2_restart();
	i2c2_send_address(eeprom_bus_address(0), 1);
	i2c2_read_bytes(eeprom_config, EEPROM_CONFIG_SPACE);
	i2c2_stop();
	
	*array_size = ((((unsigned long) eeprom_config[0]) << 16) +  (eeprom_config[1] << 8) + eeprom_config[2]) - EEPROM_CONFIG_SPACE;
	
	return 0;
}

unsigned char eeprom_write_byte(unsigned long address, unsigned char byte)
{
	return __eeprom_write_raw_byte(address + EEPROM_CONFIG_SPACE, byte);
}

unsigned char eeprom_read_byte(unsigned long address, unsigned char * byte)
{
	unsigned char return_code = 0, bus_addy;
	address += EEPROM_CONFIG_SPACE;
	bus_addy = eeprom_bus_address(address);
	i2c2_start();
	return_code |= i2c2_send_address(bus_addy, 0);
	if (return_code)
	{
		goto done;
	}
	return_code |= i2c2_send_byte(address >> 8);
	if (return_code)
	{
		goto done;
	}
	return_code |= i2c2_send_byte(address & 0x00FF);
	if (return_code)
	{
		goto done;
	}
	i2c2_restart();
	return_code |= i2c2_send_address(bus_addy, 1);
	if (return_code)
	{
		goto done;
	}
	*byte = i2c2_read_byte();
	i2c2_send_nack();
	
done:	
	i2c2_stop();	
	return return_code;
}

unsigned char eeprom_read_bytes(unsigned long address, unsigned char * bytes, unsigned long length)
{
	unsigned char return_code = 0, bus_addy;
	unsigned long read_length = 0, block_end, bytes_offset = 0;
	address += EEPROM_CONFIG_SPACE;
	i2c2_start();
	while (length > 0)
	{
		bus_addy = eeprom_bus_address(address);
		block_end = eeprom_block_last_byte(address);

		if (address + length > block_end)
		{
			read_length = block_end - address;
		} else
		{
			read_length = length;
		}
		
		return_code |= i2c2_send_address(bus_addy, 0);
		if (return_code)
		{
			goto done;
		}
		return_code |= i2c2_send_byte(address >> 8);
		if (return_code)
		{
			goto done;
		}
		return_code |= i2c2_send_byte(address & 0x00FF);
		if (return_code)
		{
			goto done;
		}
		i2c2_restart();
		return_code |= i2c2_send_address(bus_addy, 1);
		if (return_code)
		{
			goto done;
		}
		i2c2_read_bytes(bytes + bytes_offset, read_length);
		i2c2_restart();
		address += read_length;
		length -= read_length;
		bytes_offset += read_length;
	}
	
done:	
	i2c2_stop();	
	return return_code;
}

unsigned char eeprom_alter_array_size(unsigned long new_size)
{
	unsigned char return_code = 0;
	return_code |= __eeprom_write_raw_byte(0x00, new_size >> 16);
	if (return_code)
	{
		goto done;
	}
	return_code |= __eeprom_write_raw_byte(0x01, new_size >> 8);
	if (return_code)
	{
		goto done;
	}
	return_code |= __eeprom_write_raw_byte(0x02, new_size);
	
done:	
	return return_code;
}
