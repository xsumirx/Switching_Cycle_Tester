#include "LCD_16x2.h"






void lcd_delay(unsigned int k)
{
	int i = 3000;
	while(i--);
}


void data_write(void)
{
	ENABLE_HIGH;
	lcd_delay(1);
	ENABLE_LOW;
}



void lcd_send_command(unsigned char cmd)
{
	CWR;
	P2OUT = (P2OUT & 0xF0)|((cmd>>4) & 0x0F); // send higher nibble
	data_write(); // give enable trigger
	P2OUT = (P2OUT & 0xF0)|(cmd & 0x0F); // send lower nibble
	data_write(); // give enable trigger
}


void lcd_send_data(unsigned char data)
{
	DR;
	P2OUT = (P2OUT & 0xF0)|((data>>4) & 0x0F); // send higher nibble
	data_write(); // give enable trigger
	P2OUT = (P2OUT & 0xF0)|(data & 0x0F); // send lower nibble
	data_write(); // give enable trigger
}


void lcd_send_string(char *s)
{
	while(*s)
	{
		lcd_send_data(*s);
		s++;
	}
}

void lcd_send_string_const(const char *s)
{
	while(*s)
	{
		lcd_send_data(*s);
		s++;
	}
}

void lcd_cursor(bool isOff){
	if(!isOff){
		lcd_send_command(LCD_CURSOR_OFF);
	}else{
		lcd_send_command(LCD_CURSOR_ON);
	}
}

void lcd_blink(){
	lcd_send_command(LCD_BLINK_ON);
}

void lcd_init(void)
{
	P2DIR |= 0xFF;
	P2OUT &= 0x00;
	lcd_send_command(0x33);
	lcd_send_command(0x32);
	lcd_send_command(0x28); // 4 bit mode
	lcd_send_command(0x0E); // clear the screen
	lcd_send_command(0x01); // display on cursor on
	lcd_send_command(0x06); // increment cursor
	lcd_send_command(0x80); // row 1 column 1
}
