/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

author: zoe worrall
contact: zworrall@g.hmc.edu
version: september 26, 2024

*/
#include <stdio.h>
#include "lib/main.h"
#include "stm32l432xx.h"


/***********************************
*                                  *
*       main()                     *
*
*  Sets all clocks to run as they are supposed to, and sets up where timers and pins point.
*
*  Runs three sets of music, Fur Elise, Megalovania, and the Super Mario Bros. Theme Song.
*
*  Should become permanently stuck in a while loop when running.
*    @return  -- 1 if done running, 0 if not
*/
int main(void) {
  ////////////////////////////////////////////////////////////////////

  //gpioEnable(GPIO_PORT_B);
  //pinMode(PB3, GPIO_OUTPUT);
  //digitalWrite(PB3, 1);

  //gpioEnable(GPIO_PORT_B);
  //pinMode(PB4, GPIO_OUTPUT);
  //digitalWrite(PB4, 1);

  //pinMode(PB5, GPIO_OUTPUT);
  //digitalWrite(PB5, 1);

  //pinMode(PB7, GPIO_OUTPUT);
  //digitalWrite(PB7, 1);

  gpioEnable(GPIO_PORT_B);
  pinMode(LEAD_PIN, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_1, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_2, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_3, GPIO_OUTPUT);

  gpioEnable(GPIO_PORT_A);
  pinMode(COMRADE_PIN_1, GPIO_OUTPUT);
  pinMode(COMRADE_PIN_2, GPIO_OUTPUT);
  pinMode(COMRADE_PIN_3, GPIO_OUTPUT);
  pinMode(COMRADE_PIN_4, GPIO_OUTPUT);

   ////////////////////////////////////////////////////////////////////
  // 1. Enable SYSCFG clock domain in RCC
  // RCC_APB2ENR, bit 0, is SYSCFGEN @ bit 0
  RCC->APB2ENR |= (1<<0);

  // Initialize timers
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
  initTIM(TIM6);

  volatile int keepTrack = 0;

  for (int i=0; i<20; i++) { delay_millis(TIM6, 1); }

  while(1) { 
    for (volatile int j=0; j<8; j++) {
      for (int i=0; i<500; i++) {
        for (int k=0; k<8; k++) {
          volatile uint32_t val = NETCHOICECASE_0[i];
          uint32_t bit_val = ((uint32_t)(val) >> k) & 1;

          digitalWrite(LEAD_PIN, bit_val);

          switch(k%4) {
            case 0: digitalWrite(COMRADE_PIN_1, bit_val); break;
            case 1: digitalWrite(COMRADE_PIN_2, bit_val); break;
            case 2: digitalWrite(COMRADE_PIN_3, bit_val); break;
            case 3: digitalWrite(COMRADE_PIN_4, bit_val); break;
            default: break;
          }

          delay_millis(TIM6, 200);
          digitalWrite(COMRADE_PIN_4, 0); digitalWrite(COMRADE_PIN_3, 0); digitalWrite(COMRADE_PIN_2, 0); digitalWrite(COMRADE_PIN_1, 0);

       }
       digitalWrite(LEAD_PIN, 0); 
       digitalWrite(COMRADE_PIN_4, 0); digitalWrite(COMRADE_PIN_3, 0); digitalWrite(COMRADE_PIN_2, 0); digitalWrite(COMRADE_PIN_1, 0);
       delay_millis(TIM6, 500);
       for (int k=0; k<8; k++) {
          char val = NETCHOICECASE_0[i];
          uint32_t bit_val = ((uint32_t)(val) >> k) & 1;

          digitalWrite(LEAD_PIN, bit_val);
          digitalWrite(FOLLOW_PIN_1, bit_val);
          digitalWrite(FOLLOW_PIN_2, bit_val);
          digitalWrite(FOLLOW_PIN_3, bit_val);

          digitalWrite(COMRADE_PIN_1, bit_val);
          digitalWrite(COMRADE_PIN_2, bit_val);
          digitalWrite(COMRADE_PIN_3, bit_val);
          digitalWrite(COMRADE_PIN_4, bit_val);

          delay_millis(TIM6, 200);
       }
      digitalWrite(LEAD_PIN, 0);
      digitalWrite(FOLLOW_PIN_1, 0);
      digitalWrite(FOLLOW_PIN_2, 0);
      digitalWrite(FOLLOW_PIN_3, 0);
      }
    }
  }
}


  // Enable LED as output
  //gpioEnable(GPIO_PORT_B);
  //GPIOB->PUPDR &= ~(0b11 << 6);  GPIOB->PUPDR |= (0b10 << 6);
  //GPIOB->PUPDR &= ~(0b11 << 8);  GPIOB->PUPDR |= (0b10 << 8);
  //GPIOB->PUPDR &= ~(0b11 << 10); GPIOB->PUPDR |= (0b10 << 10);
  //GPIOB->PUPDR &= ~(0b11 << 14); GPIOB->PUPDR |= (0b10 << 14);
  //GPIOB->MODER |= (0b01 << 6);
  //GPIOB->MODER |= (0b01 << 8);
  //GPIOB->MODER |= (0b01 << 10);
  //GPIOB->MODER |= (0b01 << 14);
  
  // RCC->AHB2ENR |= (1 << 0); // GPIOA
  //gpioEnable(GPIO_PORT_A);

  //GPIOA->PUPDR &= ~(0b11 << 16);  GPIOA->PUPDR |= (0b10 << 16);
  //GPIOA->PUPDR &= ~(0b11 << 20);  GPIOA->PUPDR |= (0b10 << 20);
  //GPIOA->PUPDR &= ~(0b11 << 22); GPIOA->PUPDR |= (0b10 << 22);
  //GPIOA->PUPDR &= ~(0b11 << 24); GPIOA->PUPDR |= (0b10 << 24);

  //pinMode(PA8, GPIO_OUTPUT);
  //pinMode(PA10, GPIO_OUTPUT);
  //pinMode(PA11, GPIO_OUTPUT);
  //pinMode(PA12, GPIO_OUTPUT);


/*************************** End of file ****************************/
