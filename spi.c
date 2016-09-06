
#include "spi.h"

void SPI_init(){
	P1DIR |= 0x10; //P1.4 Make this Output Pin for CS Line
	P1OUT &= ~0x10;
	P1SEL |= 0xE0;
	USICTL0 |= USIPE7 + USIPE6 + USIPE5 + USIMST + USIOE;
	//USICTL1 |= USICKPL;
	USICTL1 |= USICKPH;
	USICKCTL |= USIDIV_4 + USISSEL_2 ;
	USICTL0 &= ~USISWRST;
}


uint8_t SPI_transfer_8(uint8_t _data){
	USICTL1 &= ~BIT0;
	USISRL = _data;
	USICNT = 8;
	while(!(USICTL1 & BIT0));
	return USISRL;

}
