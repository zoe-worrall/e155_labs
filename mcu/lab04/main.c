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
#include "lib/clk.h"
#include "lib/gpio.h"
#include "lib/timer.h"


#define RCC_BASE_ADR (0x40021000UL)
#define RCC_AHB2ENR ((uint32_t *) (RCC_BASE_ADR + 0x4C))


#define DELAY_DURATION  200

///////////////// Given With Lab 4 //////////////////////
const int FUR_ELISE[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	0}};

/////////////////////////////////////////////////////////

////////////////////////////from E85 (Megalovania) //////////////////////////////////////////
#define Bb3 233
#define B3  256
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define Gs4 415
#define A4  440
#define Bb4 466
#define B5  494
#define C5  523
#define D5  587
#define C3  131
#define G3  196
#define E3  165
#define A5  880
const int MEGALOVANIA[] = { D4,  D4, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                            C4,  C4, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                            B3,  B3, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                           Bb3, Bb3, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4, 0};


const int megaspacing[] = {  500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500, 0};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////// SUPER MARIO BROS. INTRO TO THEME //////////////////////////////////////////
const int MARIO[] = { E4, E4, E4, C4, E4, G4, G3, C4, E3, G3, 0, A4, B5, Bb4, C3, G3,
                      E4, G4, A5, F4, G4, 0, E4, C4, D4, B3, 0 };

const int mariospacing[] = { 125, 63, 125, 63, 125, 250, 250, 188, 188, 125, 63, 125, 125, 63, 125,
                        124, 63, 63, 125, 63, 63, 63, 125, 63, 125, 188, 0};
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIN_NUMBER 5
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
  configureClock();

  // from other blink_led class
  
  RCC->APB1_ENR1 |= (1<<0); // Turn on Timer2 (to GPIO)
  RCC->APB1_ENR1 |= (1<<4); // Turn on Timer6 (delay timer);
  // setup_TIM2_CH1_PWM();
  configure_TIM23_PWM(TIM2, 10, 0.5);
  configure_TIMx(TIM6);
  // configure_TIM2_CH1_PWM();

  // RCC->APB2_ENR |= (1<<16); // TIM15EN

  RCC->AHB2_ENR |= (1<<0);
  setModeOfPinA(PIN_NUMBER*2, 3);

  // sets AF1 so that Timer 2, Channel 1 goes to Pin GPIO A 5 on the breakout adapter board.
    // see page 272 https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
    // see page 57 https://hmc-e155.github.io/assets/doc/ds11451-stm32l432kc.pdf
  GPIOA->AFRL &= ~(0b1111 << 20);
  GPIOA->AFRL |=  (0b0001 << 20);  // Set PA5 to Alternate Function A1 (TIM2_CH1)

  // GPIOA->ODR |= (1<<5);
  
  while(1) { 

    // Play Fur Elise
    int i = 0;
    while(FUR_ELISE[i][0] != 0 || FUR_ELISE[i][1] != 0) {
      configure_TIM23_PWM(TIM2, FUR_ELISE[i][0], 0.5); 
      delay(TIM6, FUR_ELISE[i][1]);
      i++;
    }

    // Count to the next song
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 250);

    configure_TIM23_PWM(TIM2, C4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 125);

    configure_TIM23_PWM(TIM2, D4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 125);

    configure_TIM23_PWM(TIM2, E4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0.5);
    delay(TIM6, 250);

    // Play Megalovania (Undertale)
    volatile int j = 0;
    while(MEGALOVANIA[j] != 0 || megaspacing[j] != 0) {
      configure_TIM23_PWM(TIM2, MEGALOVANIA[j], 0.5);
      delay(TIM6, megaspacing[j]/10);
      j++;
    }  

    // Count to the next song
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 250);

    configure_TIM23_PWM(TIM2, C4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 125);

    configure_TIM23_PWM(TIM2, D4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 125);

    configure_TIM23_PWM(TIM2, E4, 0.5);
    delay(TIM6, 250);
    configure_TIM23_PWM(TIM2, 0, 0.5);
    delay(TIM6, 250);

    configure_TIM23_PWM(TIM2, 0, 0.5);
    delay(TIM6, 500);
    
    // Play Super Mario Bros. Theme
    volatile int k = 0;
    while (MARIO[k] != 0 || mariospacing[k] != 0) {
      configure_TIM23_PWM(TIM2, MARIO[k], 0.5);
      delay(TIM6, mariospacing[k]);
      k++;
    }

    // Delay before reloop occurs
    configure_TIM23_PWM(TIM2, 0, 0);
    delay(TIM6, 500);
    
  }
}



/**

 //configureTimer();
  //configurePWM(2, 0.5);

   Page 261:
      Peripheral alternate function:
      – Connect the I/O to the desired AFx in one of the GPIOx_AFRL or GPIOx_AFRH register.
      – Select the type, pull-up/pull-down and output speed via the GPIOx_OTYPER, GPIOx_PUPDR and GPIOx_OSPEEDER registers, respectively.
      – Configure the desired I/O as an alternate function in the GPIOx_MODER register.
  
  // mux can only set PA9 -- MODE9 of GPIOA is pins 18 and 19. On board this is mapped to PB3?? But in schematic it's PA2...
  //setModeOfPinB(6, 3); // set pin to be an alternate function (reading peripheral of the timer)
  
  *RCC_AHB2ENR |= (1 << 1);
*/


/**
*
*  Alternate functions: find something that uses TIM15's OC1REF inside the alternate function
https://hmc-e155.github.io/assets/doc/ds11451-stm32l432kc.pdf
*/

// To Future Zoe:
  // AHB2 is responsible for GPIO pins if they're reading off of H-clock
     //RCC->AHB2_ENR |= (1<<1);

/*************************** End of file ****************************/
