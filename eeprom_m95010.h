/*
 * eeprom_m95010.h
 *
 *  Created on: Oct 5, 2015
 *      Author: sumir
 */

#ifndef EEPROM_M95010_H_
#define EEPROM_M95010_H_

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>
#include "spi.h"
#include "Setting_Variables.h"

#define EEPROM_ASSERT_DELAY 5000

#define WRITE_ENABLE 		0x06
#define WRITE_DISABLE 		0x04
#define READ_STATUS_R 		0x05
#define WRITE_STATUS_R 		0x01
#define RD_MEM 				0x03
#define WR_MEM 				0x02

bool EEPROM_M95010_ReadByte(uint8_t address,uint8_t *_read_byte);
bool EEPROM_M95010_WriteByte(uint8_t address,uint8_t data);
bool EEPROM_M95010_WriteSetting();


#endif /* EEPROM_M95010_H_ */
