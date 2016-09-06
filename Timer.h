#ifndef TIMER_H_
#define TIMER_H_


#include <stdbool.h>
#include <stdint.h>
#include<msp430.h>

void TimerA0_Initilize();
void Timer_Delay_Block(uint16_t _del);
bool TimerA0_isRunning();
void TimerA0_Stop();

#endif
