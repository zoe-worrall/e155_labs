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
#define BUTTON_PIN PA5
#define DELAY_TIM TIM2

void EXTI9_5_IRQHandler(void);

#endif // MAIN_H