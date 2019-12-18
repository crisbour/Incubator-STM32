/*
 * DS18B20.h
 *
 *  Created on: 18 Dec 2019
 *      Author: bourc
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "cb_stm32_onewire.h"

#define DS18B20_CMD_CONVERT				0x44
#define DS18B20_CMD_READ_SCRATCHPAD		0xBE
#define DS18B20_CMD_WRITE_SCRATCHPAD	0x4E
#define DS18B20_CMD_EEPROM_WRITE		0x48

typedef enum{
	nine_bits_PRECISION=0x00,
	ten_bits_PRECISION=0x01,
	eleven_bits_PRECISION=0x10,
	twelve_bits_PRECISION=0x11
}DS18B20_CR;	//Configuration register

uint8_t DS18B20_Init(OneWire_t* OneWireStruct, DS18B20_CR);

uint8_t DS18B20_StartConv(OneWire_t* OneWireStruct);

float DS18B20_ReadTemp(OneWire_t* OneWireStruct);



#endif /* DS18B20_H_ */
