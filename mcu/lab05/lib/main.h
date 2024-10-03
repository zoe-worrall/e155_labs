// main.h
// Josh Brake
// jbrake@hmc.edu
// 10/31/22 

#ifndef MAIN_H
#define MAIN_H


#include "STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines 
///////////////////////////////////////////////////////////////////////////////

#define LED_PIN PB3
#define BUTTON_PIN_1 PA6
#define BUTTON_PIN_2 PA9
#define DELAY_TIM TIM6
#define FREQ_TIM TIM2

#define CASE_A 1
#define CASE_B 2

void EXTI9_5_IRQHandler(void);

void A_Event(int code);
void B_Event(int code);

#endif // MAIN_H