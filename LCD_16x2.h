
#ifndef LCD_16X2_H_
#define LCD_16X2_H_

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>

#define DR P2OUT = P2OUT | BIT5 // define RS high
#define CWR P2OUT = P2OUT & (~BIT5) // define RS low
#define ENABLE_HIGH P2OUT = P2OUT | BIT4 // define Enable high signal
#define ENABLE_LOW P2OUT = P2OUT & (~BIT4) // define Enable Low signal

#define _lcd_delay 5000



#define LCD_CURSOR_ON 0x0E
#define LCD_CURSOR_OFF 0x0C
#define LCD_BLINK_ON 0x0F

void lcd_delay(unsigned int k);
void lcd_send_command(unsigned char cmd);
void lcd_send_data(unsigned char data);
void lcd_send_string(char *s);
void lcd_cursor(bool isOff);
void lcd_blink();
void lcd_init(void);
void lcd_send_string_const(const char *s);



#endif
