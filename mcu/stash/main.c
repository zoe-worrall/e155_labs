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
  // Enable LED as output
  gpioEnable(GPIO_PORT_B);
  pinMode(LEAD_PIN, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_1, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_2, GPIO_OUTPUT);
  pinMode(FOLLOW_PIN_3, GPIO_OUTPUT);

  gpioEnable(GPIO_PORT_A);
  pinMode(FELLOW_PIN_1, GPIO_OUTPUT);
  pinMode(FELLOW_PIN_2, GPIO_OUTPUT);
  pinMode(FELLOW_PIN_3, GPIO_OUTPUT);
  pinMode(FELLOW_PIN_4, GPIO_OUTPUT);

  // Initialize timers
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
  initTIM(TIM6);

  while(1) { 
    for (int i=0; i<1027; i++) {
      //read_Model(i);
      //repeat_Model(i);
      //delay_millis(TIM6, 1000);
    }
  }
}

void read_Model(int idx) {
/**
  int bitVal = 0;
  for (int j=0; j<8; j++) {
    digitalWrite(FELLOW_PIN_1, 0); 
    digitalWrite(FELLOW_PIN_2, 0); 
    digitalWrite(FELLOW_PIN_3, 0); 
    digitalWrite(FELLOW_PIN_4, 0);
    digitalWrite(LEAD_PIN, 0);
    bitVal = (NETCHOICE_CASE_1[idx] << j) & 1;
    
    // comrades
    switch(j%4) {
      case 0: digitalWrite(FELLOW_PIN_1, bitVal); break;
      case 1: digitalWrite(FELLOW_PIN_2, bitVal); break;
      case 2: digitalWrite(FELLOW_PIN_3, bitVal); break;
      case 3: digitalWrite(FELLOW_PIN_4, bitVal); break;
      case 4: digitalWrite(FELLOW_PIN_1, bitVal); break;
      case 5: digitalWrite(FELLOW_PIN_2, bitVal); break;
      case 6: digitalWrite(FELLOW_PIN_3, bitVal); break;
      case 7: digitalWrite(FELLOW_PIN_4, bitVal); break;
      default: break;
    }

    // the dictator
    digitalWrite(LEAD_PIN, bitVal);

    delay_millis(TIM6, 1000);
  }

  digitalWrite(FELLOW_PIN_1, 0); 
  digitalWrite(FELLOW_PIN_2, 0); 
  digitalWrite(FELLOW_PIN_3, 0); 
  digitalWrite(FELLOW_PIN_4, 0); 
  digitalWrite(LEAD_PIN, 0);
  */
}

void repeat_Model(int idx) {
/**
  int bitVal = 0;
  for (int j=0; j<8; j++) {
    digitalWrite(FELLOW_PIN_1, 0); 
    digitalWrite(FELLOW_PIN_2, 0); 
    digitalWrite(FELLOW_PIN_3, 0); 
    digitalWrite(FELLOW_PIN_4, 0);
    digitalWrite(LEAD_PIN, 0); 
    digitalWrite(FOLLOW_PIN_1, 0); 
    digitalWrite(FOLLOW_PIN_2, 0); 
    digitalWrite(FOLLOW_PIN_3, 0);
    bitVal = (NETCHOICE_CASE_1[idx] << j) & 1;
    
    // comrades
    digitalWrite(FELLOW_PIN_1, bitVal);
    digitalWrite(FELLOW_PIN_2, bitVal);
    digitalWrite(FELLOW_PIN_3, bitVal);
    digitalWrite(FELLOW_PIN_4, bitVal);
  

    // the dictator
    digitalWrite(LEAD_PIN, bitVal);
    digitalWrite(FOLLOW_PIN_1, bitVal);
    digitalWrite(FOLLOW_PIN_2, bitVal);
    digitalWrite(FOLLOW_PIN_3, bitVal);

    delay_millis(TIM6, 1000);
  }

  digitalWrite(FELLOW_PIN_1, 0); 
  digitalWrite(FELLOW_PIN_2, 0); 
  digitalWrite(FELLOW_PIN_3, 0); 
  digitalWrite(FELLOW_PIN_4, 0); 
  digitalWrite(FOLLOW_PIN_1, 0); 
  digitalWrite(FOLLOW_PIN_2, 0); 
  digitalWrite(FOLLOW_PIN_3, 0); 
  digitalWrite(LEAD_PIN, 0);
  */

}

/*************************** End of file ****************************/
