/*
 * spi.h
 *
 *  Created on: Oct 5, 2015
 *      Author: sumir
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>


#define ASSERT_CS (P1OUT &= ~0x10)
#define DEASSART_CS (P1OUT |= 0x10)

void SPI_init();
uint8_t SPI_transfer_8(uint8_t _data);



#endif /* SPI_H_ */
