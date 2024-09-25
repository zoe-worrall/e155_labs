/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/
#include "lib/clk.h"
#include "lib/gpio.h"
#include "lib/timer.h"


#define RCC_BASE_ADR (0x40021000UL)
#define RCC_AHB2ENR ((uint32_t *) (RCC_BASE_ADR + 0x4C))


#define DELAY_DURATION  200

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


// from E85 (Megalovania) //
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
const int MEGALOVANIA[] = { D4,  D4, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                       C4,  C4, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                       B3,  B3, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4,
                      Bb3, Bb3, D5, A4, 0, Gs4, 0, G4, 0, F4, D4, F4, G4};


int megaspacing[] = {  500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500,
                       500,  500,  1000,  1000, 500,   500, 500,  500, 500,  1000,  500,  500,  500};


// Simple delay function to use a bunch of NOPs to stall the processor
void main_delay(int cycles) {
   while (cycles-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

#define PIN_NUMBER 5
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
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

  // set AF14 (aka use TIM15_BKIN on pin PA within register AF9 [7:4], setting AF14 (1110: AF14) )
    // see page 272 https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
    // see page 57 https://hmc-e155.github.io/assets/doc/ds11451-stm32l432kc.pdf
  GPIOA->AFRL &= ~(0b1111 << 20);
  GPIOA->AFRL |=  (0b0001 << 20);  // Set PA5 to Alternate Function A1 (TIM2_CH1)

  // GPIOA->ODR |= (1<<5);
  
  while(1) { 
    //for (volatile int i=0; i<sizeof(MEGALOVANIA); i++) {
      //configure_TIM23_PWM(TIM2, FUR_ELISE[i][0], 0.25); 
      //delay(TIM6, FUR_ELISE[i][1] * 10);
      
      

    //configure_TIM23_PWM(TIM2, MEGALOVANIA[i], 0.5);
    //delay(TIM6, megaspacing[i]);
    //}
    for (int i=0; i<10000; i++) {
      configure_TIM23_PWM(TIM2, C4, 0.5);
      delay(TIM6, 10000000);
      }
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
