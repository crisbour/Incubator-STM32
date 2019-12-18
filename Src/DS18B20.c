/*
 * DS18B20.c
 *
 *  Created on: 18 Dec 2019
 *      Author: bourc
 */

#include "DS18B20.h"

uint8_t data[9];

uint8_t DS18B20_Init(OneWire_t* OneWireStruct, DS18B20_CR precision){

	if(OneWire_Reset(OneWireStruct))
		return 1;

	OneWire_Select(OneWireStruct, OneWireStruct->ROM_NO);

	OneWire_WriteByte(OneWireStruct, DS18B20_CMD_WRITE_SCRATCHPAD);
	OneWire_WriteByte(OneWireStruct, 0);
	OneWire_WriteByte(OneWireStruct, 0);
	OneWire_WriteByte(OneWireStruct, precision<<5);

	OneWire_WriteByte(OneWireStruct, DS18B20_CMD_EEPROM_WRITE);

	return 0;
}

uint8_t DS18B20_StartConv(OneWire_t* OneWireStruct){

	if(OneWire_Reset(OneWireStruct))
		return 1;

	OneWire_Select(OneWireStruct, OneWireStruct->ROM_NO);
	OneWire_WriteByte(OneWireStruct, DS18B20_CMD_CONVERT);

	HAL_Delay(190);

	return 0;
}
float DS18B20_ReadTemp(OneWire_t* OneWireStruct){
	int16_t intTemp=0;
	while(1){
		if(OneWire_Reset(OneWireStruct))
			return 1;

		OneWire_Select(OneWireStruct, OneWireStruct->ROM_NO);
		OneWire_WriteByte(OneWireStruct, DS18B20_CMD_READ_SCRATCHPAD);

		for(uint8_t i=0;i<9;i++)
			data[i]=OneWire_ReadByte(OneWireStruct);

		if(OneWire_CRC8(data,8)==data[8])
			break;
	}

	if(data[1]&0xF0)	intTemp=1<<15;		//Set the sign bit

	intTemp|=((data[1]&0x07)<<8)|data[0];

	return ((float)intTemp)/16;
}
