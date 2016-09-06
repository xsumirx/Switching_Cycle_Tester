
#include "Timer.h"


void TimerA0_Initilize(){
    TA0CCR0 = 16000;
    TA0CTL |= TASSEL_2 + MC_1 + TAIE;  //UP MODE + ACLK + Interrupt Enable
}

void TimerA0_Stop(){
	TA0CCR0 = 0;
}

bool TimerA0_isRunning(){
	if(TA0CCR0 > 0){
		return true;
	}else
		return false;
}


void Timer_Delay_Block(uint16_t _del){
	while(_del--);
}
