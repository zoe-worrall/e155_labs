/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*                    Zoe Worrall, HMC, 09/20/2024                    *
*                    Contact: zworrall@g.hmc.edu                     *
*                                                                    *
**********************************************************************

  This C program contains five functions:

  setModeofPinA and setModeofPinB set the modes of the A and B GPIO pins on
      the STM32L432xx Board.

  readPinInput reads the input of a GPIO B pin

  setPinAOutput and setPinBOutput set the outputs of the GPIO A and B 
        pins respectively

*/
#include "clk.h"

/***********************************
*                                  *
*       configurePLL()             *
*
*  Properly sets all pins in order to allow for the system to run a PLL clock
*   on a STM32L432 Board.
*  Uses MSI clock as an input. Since MSI is not configured for this setup, it remains at
*   it's initial frequency of 4 MHz -> VCO = 4 MHz
*
*  Output is VCO * N / (M * R)
*
*    @param M -- Can only be between values 1 to 8
*    @param N -- Can only be between values 8 to 86
*    @param R -- Can only be values 2, 4, 6, 8  (characteristic of PLLCLK)
*
*    @return  -- 1 if done running, 0 if not
*/
int configurePLL(void) {
  // As defined in the user manual, in order to set up the PLL, you need to do the following:
      // 1. Set the PLLON to 0 (Disable PLL)
      // 2. Wait for PLLRDY to be cleared -> PLLRDY, either 0 when unlocked (not ready) or 1 when locked
      // 3. Change the parameters you want (in this case, M, N, R)
      // 4. Set the PLLON to 1 (Enable PLL)
      // 5. Enable PLL_outputs (I am planning on using PLLCLK, so PLLREN --> PLLCFGR)

    // according to requisites: the output of VCO * N/M needs to be between 64 and 344 MHz
        // 4MHz * 16/1 = 64MHz -> N = 0010000, M = 000
    // according to PLL requisites, the output of PLL needs to be below 80 MHz
        // 64 MHz / 8 = 8 MHz -> R = 11

    RCC->CR &= ~(1<<24);       // Disables PLLON
    while ((RCC->CR >> 25 & 1) != 0);  // Waits until PLLRDY is clear, i.e. when its locked
    
    // Setting clock for PLL to be MSI -> PLL_SRC (@ bits 1:0) = 2'b01
    RCC->PLL_CFGR &= ~(0b1<<1);
    RCC->PLL_CFGR |=  (0b1<<0);

    // Setting R to 2 -> 00 at bits 25, 26
    RCC->PLL_CFGR |= (0b1<<25); // 1
    RCC->PLL_CFGR |= (0b1<<26); // 1

    // Setting M to 1 -> 000 at bits 6:4
    RCC->PLL_CFGR &= ~(0b1<<4); // 0
    RCC->PLL_CFGR &= ~(0b1<<5); // 0
    RCC->PLL_CFGR &= ~(0b1<<6); // 0

    // Setting N to 10 -> 0010000 at bits 14:8
    RCC->PLL_CFGR &= ~(0b11111111 << 8);
    RCC->PLL_CFGR |=  (16 << 8);  // set to 15
    // Final frequency = 4 MHz * (10/1) / 2 = 20 MHz
    
    RCC->CR |= (0b1<<24);         // Enables PLLON
    RCC->PLL_CFGR |= (0b1<<24);   // Sets PLLCLK to be the output

    return 1;
}

/***********************************
*                                  *
*       configureClock()           *
*
*  Sets up a clock using the configurePLL function. 
*  The clock will have a frequency of 20 MHz.
*  
*    @return  -- 1 if done running, 0 if not
*/
int configureClock(void) {
    int configured = configurePLL();
    RCC->CFGR |= (0b11 << 0);

    while(!((RCC->CFGR >> 2) & 0b11));  // wait for index 2:3 to be set to b11
    return configured;
}

/*************************** End of file ****************************/
