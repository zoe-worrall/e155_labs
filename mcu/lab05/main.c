/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

@author: zoe worrall
@contact: zworrall@g.hmc.edu
@version: October 8, 2024

-------------------------- END-OF-HEADER -----------------------------

File    : main.c for Lab 5
Purpose : A function that will measure the frequency and duty cycle of
          an incoming motor.

*/

#include <stdio.h>
#include "lib/main.h"
#include "stm32l432xx.h"

volatile int idxA;
volatile int idxB;
int hav_ent;

volatile int arrA[5];
volatile int arrB[5];
volatile int curr_count_avg;
volatile int forwards;

volatile int first_time_loop_A;
volatile int first_time_loop_B;

double frequency = -1;


// Function used by printf to send characters to the laptop. Supplied by Kavi for purposed of Lab5
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    ITM_SendChar((*ptr++));
  }
  return len;
}

/*********************************************************************
*
*       main()
*
*  Enables all required pins. Runs a program that is interrupted and computes the direction and
*     frequency of rotation of a TS-25GA370 Motor with encoder pins A and B
*  
*/
int main(void) {
    ///////////////////////////Enabling Basic Functions for Main /////////////////////////////////////////
    // Enable LEDs as outputs

    // Enable button as input PA5
    gpioEnable(GPIO_PORT_A);
    pinMode(BUTTON_PIN_1, GPIO_INPUT);
    GPIOA->PUPDR &= ~_VAL2FLD(GPIO_PUPDR_PUPD6, 0b11); // reset PA6 to 0
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD6, 0b10); // Set PA6 as pull down
 
    pinMode(BUTTON_PIN_2, GPIO_INPUT); // enable pin PA9
    GPIOA->PUPDR &= ~_VAL2FLD(GPIO_PUPDR_PUPD9, 0b11); // reset PA9 to 0
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD9, 0b10); // Set PA9 as pull down

    // Initialize timers
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
    initTIM(DELAY_TIM);

      /// I'm going to add an additional timer
      // required frequency range to observe: 10 rpm to 600 rpm
          // pulses per loop/rotation = 12 - so one set of pulses
          //      (i.e. 4 interrupts) is one of these pulses

          // 120 pulses per minute to 600 pulses per minute -> in Hz?
          // 1 pulse = 4 signals (i.e. A rises, B rises, A falls, B falls)

          // 480 s/minute = 8 signals/second
          // 8 sig/sec  TO   480 sig/sec (should be able to detect between 8 and 480 sig/sec)
              // in order to count the maximum time delay, min Hz?
              // in order to catch the minimum time delay, max Hz?
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initFreqTIM(FREQ_TIM);

     // for debugging: initialize timer 7 to track frequency value
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
    initFreqTIM(TIM7);

    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    // 1. Enable SYSCFG clock domain in RCC
    // RCC_APB2ENR, bit 0, is SYSCFGEN @ bit 0
    RCC->APB2ENR |= (1<<0);

    //not zero indexed
    SYSCFG->EXTICR[1] &= ~_VAL2FLD(SYSCFG_EXTICR2_EXTI6, 0b111); // Figure 29

    //not zero indexed
    SYSCFG->EXTICR[2] &= ~_VAL2FLD(SYSCFG_EXTICR3_EXTI9, 0b111); // Figure 29

    // Enable interrupts globally
    __enable_irq();
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    // 1. Configure mask bit for pin PA5 -- need to set it for EXTI2 ** changed to pin PA5 for now
        // EXTI: GPIO lines are bits 0-15, and are configurable
    EXTI->IMR1 |= _VAL2FLD(EXTI_IMR1_IM6, 0b1); // 1: Interrupt request from Line 4 is not masked
    EXTI->IMR1 |= _VAL2FLD(EXTI_IMR1_IM9, 0b1); // 1: Interrupt request from Line 4 is not masked
    
    // 2. Enable rising edge trigger
    EXTI->RTSR1 |= _VAL2FLD(EXTI_RTSR1_RT6, 0b1); // 1: Rising trigger enabled (for Event and Interrupt) for input line 4
    EXTI->RTSR1 |= _VAL2FLD(EXTI_RTSR1_RT9, 0b1); // 1: Rising trigger enabled (for Event and Interrupt) for input line 4

    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= _VAL2FLD(EXTI_FTSR1_FT6, 0b1); // 1: Falling trigger enabled (for Event and Interrupt) for input line 4
    EXTI->FTSR1 |= _VAL2FLD(EXTI_FTSR1_FT9, 0b1); // 1: Falling trigger enabled (for Event and Interrupt) for input line 4


    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn); // based of of table 46 in the user guide (https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), whould be 6 for systick
    // EXTI9_5_IRQn -- enables interrupt pins 9-5
    ////////////////////////////////////////////////////////////////////

    // used in the case of an extremely slow motor, where more than 1 second is taken to update
    int prev_idxA = 0; int prev_idxB = 0;
    int long_term_count = 0;

    // sets the current index and spin direction. Some of these variables are used for debugging.
    idxA = 0; idxB = 0; curr_count_avg = 0;
    volatile int for_spin = -1;

    // Determines whether or not this is the user's first time inside a loop, to send a special message :)
    first_time_loop_A = 1; first_time_loop_B = 1;
    hav_ent = 0;
    int stall = 0; int stall_exited = 0;

    // while loop that will be interrupted if the motor triggers a pin.
    while(1){
        
        hav_ent = 0;

        delay_millis(TIM6, 1000);

        // IF WE HAVEN'T FILLED UP THE ARRAY
        if (curr_count_avg == 0) {

            if  ( first_time_loop_A & first_time_loop_B) {

              printf("Waiting to start\n");

            } else if (!(first_time_loop_A) & !(first_time_loop_B)) { // making sure that the array is completely full

              curr_count_avg = ((arrA[0] + arrA[1] + arrA[2] + arrA[3] + arrA[4])/5 + (arrB[0] + arrB[1] + arrB[2] + arrB[3] + arrB[4])/5);
              frequency = (3e5 + 0.0) / (4 * curr_count_avg * 120);
              if (forwards) {
                printf("Newly caught signal, frequency is %f rev/sec \n", frequency);
              } else {
                printf("Newly caught signal, frequency is -%f rev/sec \n", frequency);
              }

            }

        } else if (stall) { // STALLED; LET'S WAIT TO SEE IF WE GET SOMETHING

            if (hav_ent) { // we made it out of the loop after waiting for under 10 seconds
                stall = 0;
                long_term_count = 0;

                int total_count = long_term_count * TIM2->ARR + ( ((arrA[0] + arrA[1] + arrA[2] + arrA[3] + arrA[4])/5 + (arrB[0] + arrB[1] + arrB[2] + arrB[3] + arrB[4])/5));
                frequency = (300000.0) / (4 * total_count * 120);
                printf("Slow frequency, but we caught it! Frequency is %f rev/sec \n", frequency);

            } else if (long_term_count > 10) { // we've stalled for too long. It's not running

                  printf("The motor isn't spinning D: \n");

            }
        } else { // ARRAY IS FILLED, AND WE HAVEN'T STALLED; WE CAN FIND THE AVERAGE
            curr_count_avg = ((arrA[0] + arrA[1] + arrA[2] + arrA[3] + arrA[4])/5 + (arrB[0] + arrB[1] + arrB[2] + arrB[3] + arrB[4])/5);
            frequency = (300000.0) / (4 * curr_count_avg * 120); // frequency of 3e5 / our location in the period
            if (!hav_ent) {
               printf("Haven't yet, adding to long term count %d\n", long_term_count);
            } else {
              if (forwards) {
                printf("Frequency is %f rev/sec \n", frequency);
              } else {
                printf("Frequency is -%f rev/sec \n", frequency);
              }
            }
        }
        
        // Enters stall state if we haven't entered a loop in the past second.
        if (!hav_ent) {
          long_term_count++;
          stall = 1;
          }
        }

    }

/**
*  What to do when handling an interrupt from the input pin (i.e. pin PA5, since that's one of the only ones I can control on my MCU)
*/
void EXTI9_5_IRQHandler (void){

    hav_ent = 1;

    // Check that the button was what triggered our interrupt
    int code = digitalRead(BUTTON_PIN_1) + 2 * digitalRead(BUTTON_PIN_2);

    if (EXTI->PR1 & (1 << 6)) { // pin A5 was triggered

       arrA[idxA] = TIM2->CNT;
       A_Event(code);
       if ((idxA == 4) & first_time_loop_A) { first_time_loop_A = 0; }

       idxA++; idxA %= 5;
       EXTI->PR1 |= (1 << _FLD2VAL(EXTI_PR1_PIF6, 1)); // clear the event

    } else if (EXTI->PR1 & (1 << 9)) { // pin A9 was triggered

       arrA[idxA] = TIM2->CNT;
       B_Event(code);
       if ((idxB == 4) & first_time_loop_B) { first_time_loop_B = 0; } // if we've reached the end of the arr, we can start calculating frequency 
       idxB++; idxB %= 5;
       EXTI->PR1 |= (1 << _FLD2VAL(EXTI_PR1_PIF9, 1)); // clear the event

    }
    TIM2->CNT = 0; // reset the counter so that we can compute the frequency later
}

void A_Event(int code) {
  switch(code) {
    case 0: // backwards, A just fell, B is at 0
      forwards = 0;
      break;
    case 1: // forwards,  A just rose, B is at 0
      forwards = 1;
      break;
    case 2: // forwards,  A just fell, B is constant
      forwards = 1;
      break;
    case 3: // backwards, A just rose, B is constant
      forwards = 0;
      break;
    default: // something's gone wrong ;-;
      break;
    }
}

void B_Event(int code) {
  switch(code) {
    case 0: // forwards, B just fell, A is at 0
      forwards = 1;
      break;
    case 1: // backwards,  B just fell, A is const
      forwards = 0;
      break;
    case 2: // backwards,  B just rose, A is 0
      forwards = 0;
      break;
    case 3: // forwards, B just rose, A is constant
      forwards = 1;
      break;
    default: // something's gone wrong ;-;
      break;
    }
}

/*************************** End of file ****************************/
