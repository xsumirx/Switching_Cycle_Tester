/*
 * Setting_Variables.h
 *
 *  Created on: Oct 8, 2015
 *      Author: led
 */

#ifndef SETTING_VARIABLES_H_
#define SETTING_VARIABLES_H_


#define E_SWITCH_MODE 				0x01
#define E_SWITCH_ACTION 			0x02
#define E_LCD_REFRESH_VARIABLE		0x04
#define E_LCD_REFRESH_FRAME			0x08
#define E_EEPROM_WRITE_SETTING		0x10
#define E_EEPROM_WRITE_COUNT		0x20

#define RELAY_TOGGLE (P1OUT ^= 0x01)
#define RELAY_ON (P1OUT |= 0x01)
#define RELAY_OFF (P1OUT &= ~0x01)

#define EEPROM_SETTING_START_ADDRESS 0x02
#define EEPROM_SETTING_END_ADDRESS 0x07

#define EEPROM_COUNT_ADDRESS 50


typedef uint8_t EVENT;
typedef uint8_t STATE;


uint8_t eeprom_setting_auto_start;
uint8_t eeprom_setting_step_size;
uint16_t eeprom_count;
uint8_t  eeprom_count_helper;
uint16_t eeprom_setting_switch_delay_on;
uint16_t eeprom_setting_switch_delay_off;

uint32_t second_counter_delay;
uint16_t second_counter;

uint32_t switch_mode_debounce,switch_action_debouce;

uint8_t SETTING_BUFFER[8];

bool EEPROM_SHOULD_SAVE;
bool relay_status;

enum states{
	S_STARTUP,
	S_LIVE,
	S_MODE_STEP_PLUS,
	S_MODE_STEP_MINUS,
	S_MODE_DELAY_ON_PLUS,
	S_MODE_DELAY_ON_MINUS,
	S_MODE_DELAY_OFF_PLUS,
	S_MODE_DELAY_OFF_MINUS,
	S_MODE_AUTOSTART,
	S_NEW,
	S_EEPROM_SAVE
};

EVENT event_global;
STATE state_active;


#endif /* SETTING_VARIABLES_H_ */
