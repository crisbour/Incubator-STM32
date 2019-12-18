/*
 * DS18B20.h
 *
 *  Created on: 18 Dec 2019
 *      Author: bourc
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "cb_stm32_onewire.h"

void DS18B20_InitReading(OneWire_t* OneWireStruct);

void DS18B20_ReadTemp(OneWire_t* OneWireStruct);



#endif /* DS18B20_H_ */
