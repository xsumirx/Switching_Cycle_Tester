
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <msp430.h>
#include "LCD_16x2.h"
#include "Timer.h"
#include "eeprom_m95010.h"
#include "Setting_Variables.h"

uint32_t msTick = 0;

void handle_event(int *event);

extern void TimerA0_Initilize();

char buffer1[10];

void Relay_Init();
void Switch_Init();
void STATE_main_loop();
void STATE_ENTER_CB(STATE s);
void STATE_LEAVE_CB(STATE s);
void STARTUP_CB();
void uintoa( char *out, uint16_t v);

int state_event,i;
int lcd_event;
int current_state;

bool isStartup = false;

extern uint8_t SETTING_BUFFER[8];
extern uint16_t second_counter;
extern uint32_t second_counter_delay;
extern uint32_t switch_mode_debounce,switch_action_debouce;


uint8_t NewProfile_i = 0;
uint8_t autostart = 0;
const char NewProfile[2][10] = {{'Y','E','S','\0'},{'N','O','\0'}};


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer


    BCSCTL1 |= (BCSCTL1 & 0x0B) | 0x0B;
    for(i = 20000; i > 1; i--);


      P1DIR = 0x01;                             // P1.0 output, else input
      P1OUT =  0x10;                            // P1.4 set, else reset
      P1REN |= 0x02;                            // P1.4 pullup
      P1IE |= 0x02;                             // P1.4 interrupt enabled
      P1IES |= 0x02;                            // P1.4 Hi/lo edge
      P1IFG &= ~0x02;                           // P1.4 IFG cleared



    TimerA0_Initilize();
    //Relay_Init();
    //Switch_Init();
    //lcd_init();
    //SPI_init();


    //lcd_send_command(0x01);
   // lcd_send_string("FIEM INDUSTRIES");

    //eeprom_setting_switch_delay_on = 10;
    //eeprom_setting_switch_delay_off = 10;
   // state_active = S_STARTUP;
   // eeprom_count_helper = 0;

   // if((P1OUT & 0x01) == 0x01)
  //  {
    	relay_status = true;
 //   }
 //   else
  //  {
  //  	relay_status = false;
 //   }

   // event_global += E_SWITCH_MODE;
    	RELAY_OFF;

    __bis_SR_register(GIE);       // Enter LPM3 w/ interrupt
    while(1){
    	if(event_global){
    		if(event_global & E_SWITCH_MODE){
    			event_global &= ~E_SWITCH_MODE;
    			/*--MODE SWITCH PRESSED--*/

    			RELAY_ON;
    			second_counter_delay = msTick + 200;
    		}
    	}

    	if(second_counter_delay < msTick)
    	{
    		RELAY_OFF;
    	}

    }
}


void STARTUP_CB(){

	isStartup = true;

	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS,&SETTING_BUFFER[0]);
	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS + 1,&SETTING_BUFFER[1]);
	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS + 2,&SETTING_BUFFER[2]);
	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS + 3,&SETTING_BUFFER[3]);
	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS + 4,&SETTING_BUFFER[4]);
	EEPROM_M95010_ReadByte(EEPROM_SETTING_START_ADDRESS + 5,&SETTING_BUFFER[5]);

	EEPROM_M95010_ReadByte(EEPROM_COUNT_ADDRESS,&SETTING_BUFFER[6]);
	EEPROM_M95010_ReadByte((EEPROM_COUNT_ADDRESS + 1),&SETTING_BUFFER[7]);

	eeprom_setting_auto_start = SETTING_BUFFER[0];
	eeprom_setting_step_size = SETTING_BUFFER[1];
	eeprom_setting_switch_delay_on = (SETTING_BUFFER[3] << 8) | SETTING_BUFFER[2];
	eeprom_setting_switch_delay_off = (SETTING_BUFFER[5] << 8) | SETTING_BUFFER[4];
	eeprom_count = (SETTING_BUFFER[7] << 8) | SETTING_BUFFER[6];

	if(eeprom_setting_auto_start == 0x00)
	{
		isStartup = true;
	}else
	{
		isStartup = false;
	}
}


void STATE_ENTER_CB(STATE s){

	switch(s){
		case S_LIVE:{

			if(isStartup == true){
				isStartup = false;
				TimerA0_Stop();
			}
			else
				TimerA0_Initilize();

			second_counter_delay = msTick + 1000;
			break;
		}
		default:
			break;
	}
}

void STATE_LEAVE_CB(STATE s){
	switch(s){
		case S_LIVE:{


			break;
		}
		case S_EEPROM_SAVE:{

			lcd_send_command(0x01);
			lcd_send_string("   Saving   ");
			if(EEPROM_SHOULD_SAVE){
				EEPROM_SHOULD_SAVE = false;
				SETTING_BUFFER[5] = eeprom_setting_switch_delay_off >> 8;
				SETTING_BUFFER[4] = eeprom_setting_switch_delay_off;
				SETTING_BUFFER[3] = eeprom_setting_switch_delay_on >> 8;
				SETTING_BUFFER[2] = eeprom_setting_switch_delay_on;
				SETTING_BUFFER[1] = eeprom_setting_step_size;
				SETTING_BUFFER[0] = eeprom_setting_auto_start;
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS,SETTING_BUFFER[0]);
				Timer_Delay_Block(20000);
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS + 1,SETTING_BUFFER[1]);
				Timer_Delay_Block(20000);
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS + 2,SETTING_BUFFER[2]);
				Timer_Delay_Block(20000);
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS + 3,SETTING_BUFFER[3]);
				Timer_Delay_Block(20000);
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS + 4,SETTING_BUFFER[4]);
				Timer_Delay_Block(20000);
				EEPROM_M95010_WriteByte(EEPROM_SETTING_START_ADDRESS + 5,SETTING_BUFFER[5]);
				Timer_Delay_Block(20000);
				if(NewProfile_i == 1){
					eeprom_count = 0;
					EEPROM_M95010_WriteByte(EEPROM_COUNT_ADDRESS,eeprom_count);
					Timer_Delay_Block(5000);
					EEPROM_M95010_WriteByte(EEPROM_COUNT_ADDRESS + 1,(eeprom_count >> 8));
				}
			}
			break;
		}
		default:
			break;
	}
}

void Relay_Init(){
	P1DIR |= 0x01;
	P1OUT |= 0x01;
}

void Switch_Init(){
	//Switch on P1.1 (mode) AND P1.2 (action)
	P1DIR 	&= 	~(BIT1 + BIT2);
	//P1OUT 	|= 	BIT1 + BIT2;
	//P1REN 	|= 	BIT1 + BIT2;
	P1IE 	|= 	BIT1 + BIT2;
	//P1IES 	|= 	BIT1 + BIT2;
	P1IFG 	&= 	~(BIT1 + BIT2);
}

void STATE_main_loop(){
	switch(state_active){
		case S_LIVE:{
			if(msTick > second_counter_delay){
				second_counter_delay += 1000;
				second_counter ++;

				if(relay_status == true)
				{
					if(second_counter > eeprom_setting_switch_delay_off){
						second_counter = 0;
						eeprom_count_helper++;
						RELAY_OFF;
						relay_status = false;
						second_counter_delay = msTick + 1000;
					}
				}
				else
				{
					if(second_counter > eeprom_setting_switch_delay_on){
						second_counter = 0;
						eeprom_count_helper++;
						RELAY_ON;
						relay_status = true;
						second_counter_delay = msTick + 1000;
					}
				}

				if(eeprom_count_helper >= 2)
				{
					eeprom_count ++;
					eeprom_count_helper = 0;
				}


				event_global += E_LCD_REFRESH_VARIABLE;
			}

			break;
		}
		default:
			break;
	}
}


void uintoa( char *out, uint16_t v )
{
    uint8_t work[5] = { 0 };
    int count = 0;

    do {
       work[count++] = v % 10;
      v /= 10;
    } while ( v );

    while ( count > 0 )
      *out++ = '0' + work[--count];
    *out = 0;
}

//It Just a Systick Timer
// Timer_A3 Interrupt Vector (TA0IV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  switch( TA0IV )
  {
    case  2:							// CCR1 not used
    	break;
    case  4:							// CCR2 not used
    	break;
    case 10:							//OverFlow Interrupt
    	//P1OUT ^= BIT0;
    	msTick++;
    	break;
  }
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT_P1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT_P1 (void)
#else
#error Compiler not supported!
#endif
{

	switch(P1IFG & (BIT1 + BIT2)){
		case 0x02:{
			//Mode is Pressed
			//if(switch_mode_debounce < msTick){
				event_global |= E_SWITCH_MODE;
				switch_mode_debounce += 1000;
			//}

			break;
		}
		case 0x04:{
			//Action is Pressed
			//if(switch_action_debouce < msTick){
				event_global |= E_SWITCH_ACTION;
				switch_action_debouce += 1000;
			//}
			break;
		}
		case 0x06:{
			//Both Pressed Simultanously
			//TOGGLE_RELAY;
			break;
		}
		default:
			break;
	}

	P1IFG = 0;
}


