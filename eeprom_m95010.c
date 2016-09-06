/*
 * eeprom_m95010.c
 *
 *  Created on: Oct 5, 2015
 *      Author: sumir
 */


#include "eeprom_m95010.h"



bool EEPROM_M95010_ReadByte(uint8_t address,uint8_t *_read_byte){
	//Only 7 Bits of Address
	/*
	 * 1. Check Status Register
	 * 2. Send RD_MEM
	 * 3. Send Address
	 * 4. Send FF
	 *
	 */

	DEASSART_CS;
	bool return_status = false;
	uint16_t timeout = 0;
	uint8_t temp_status = 0;
	int i = 0;

	SPI_transfer_8(0xff);

	ASSERT_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);

	do{
		timeout++;
		SPI_transfer_8(READ_STATUS_R);
		temp_status = SPI_transfer_8(0xFF);
		if(!(temp_status & 0x01)){
			DEASSART_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			ASSERT_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			SPI_transfer_8(RD_MEM);
			SPI_transfer_8(address & 0x7f);
			*_read_byte = SPI_transfer_8(0xff);
			return_status = true;
			break;
		}

	}while(timeout < 50);

	DEASSART_CS;

	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	return return_status;

}



bool EEPROM_M95010_WriteByte(uint8_t address,uint8_t data){
	//Only 7 Bits of Address
	/*
	 * 1. Write ENABLE
	 * 1. Check Status Register
	 * 2. Send WR_MEM
	 * 3. Send Address
	 * 4. Send Data
	 * 5. Check Status WIP
	 *
	 */

	DEASSART_CS;
	bool return_status = false;
	uint16_t timeout = 0;
	uint8_t temp_status = 0;

	int i;


	SPI_transfer_8(0xff);

	ASSERT_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	SPI_transfer_8(WRITE_ENABLE);
	DEASSART_CS;

	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);

	ASSERT_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	do{
		timeout++;
		SPI_transfer_8(READ_STATUS_R);
		temp_status = SPI_transfer_8(0xFF);

		if(!(temp_status & 0x01)){
			DEASSART_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			ASSERT_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			SPI_transfer_8(WR_MEM);
			SPI_transfer_8(address & 0x7f);
			SPI_transfer_8(data);
			return_status = true;
			break;
		}

	}while(timeout < 50);
	DEASSART_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	return return_status;

}


bool EEPROM_M95010_WriteSetting(){
	//Only 7 Bits of Address
	/*
	 * 1. Write ENABLE
	 * 1. Check Status Register
	 * 2. Send WR_MEM
	 * 3. Send Address
	 * 4. Send Data
	 * 5. Check Status WIP
	 *
	 */

	DEASSART_CS;
	bool return_status = false;
	uint16_t timeout = 0;
	uint8_t temp_status = 0;

	int i;


	SPI_transfer_8(0xff);

	ASSERT_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	SPI_transfer_8(WRITE_ENABLE);
	DEASSART_CS;

	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);

	ASSERT_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	do{
		timeout++;
		SPI_transfer_8(READ_STATUS_R);
		temp_status = SPI_transfer_8(0xFF);

		if(!(temp_status & 0x01)){
			DEASSART_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			ASSERT_CS;
			for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
			SPI_transfer_8(WR_MEM);
			SPI_transfer_8(EEPROM_SETTING_START_ADDRESS & 0x7f);
			for(i=0; i<=2; i++)
				SPI_transfer_8(SETTING_BUFFER[i]);
			return_status = true;
			break;
		}

	}while(timeout < 50);
	DEASSART_CS;
	for(i= EEPROM_ASSERT_DELAY; i > 1; i--);
	return return_status;

}


