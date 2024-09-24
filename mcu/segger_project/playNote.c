#include "playNote.h"
#include "zoe_def_stm32l432xx.h"


/**
* Delay for some number of ms
*/
void delay(int ms) {
  volatile int i = ONE_MS;
  while (i--);
}

/**
*  Run one on-off of the output clock at given frequency
*/
int do_freq(int freq) {
  int wait = PLL_FREQ/freq;
  int freq_control[wait];

  while(1) {
   for(volatile int i = 0; i < 20000; i++);
   GPIOB->ODR ^= (1 << 3);
   }
  for (int i=0; i<wait; i++); //

  return 1;
}

/***********************************
*                                  *
*           playNote()                 *
*
*  Assumes 8 MHz due to header zoe_def_stm32l432xx.h
*
*   @param freq     -- frequency in Hz
*   @param duration -- duration of note in ms
*/
int playNote(int freq, int duration) {
    int on_off = PLL_FREQ/freq;
    volatile int dur_cycles = duration * ONE_MS;

    // Enable the IO Port B Clock
    RCC->AHB2_ENR |= (1 << 1);

     // Set Up General Purpose Output Pin @ PB5)
    GPIOB->MODER |=  (1 << 10);
    GPIOB->MODER &= ~(1 << 11);

    // Need to decide whether pull-up, pull-down, etc.

    while(dur_cycles-- > 0) {    // should last for the total number of ticks
      for(volatile int i=0; i<on_off; i++);
      GPIOB->ODR ^= (1 << 5);
     }
     return 1;
}


