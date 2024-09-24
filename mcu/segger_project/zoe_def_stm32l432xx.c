/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*                    Zoe Worrall, HMC, 09/20/2024                    *
*                                                                    *
*                                                                    *
**********************************************************************
*/
#include "zoe_def_stm32l432xx.h"
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
    RCC->PLL_CFGR &= ~(1<<1);
    RCC->PLL_CFGR |=  (1<<0);

    // Setting R to 2 -> 00 at bits 25, 26
    RCC->PLL_CFGR |= (1<<25); // 1
    RCC->PLL_CFGR |= (1<<26); // 1

    // Setting M to 1 -> 000 at bits 6:4
    RCC->PLL_CFGR &= ~(1<<4); // 0
    RCC->PLL_CFGR &= ~(1<<5); // 0
    RCC->PLL_CFGR &= ~(1<<6); // 0

    // Setting N to 10 -> 0010000 at bits 14:8
    RCC->PLL_CFGR &= ~(1<<14); // 0
    RCC->PLL_CFGR &= ~(1<<13); // 0
    RCC->PLL_CFGR |=  (1<<12); // 1
    RCC->PLL_CFGR &= ~(1<<11); // 0
    RCC->PLL_CFGR &= ~(1<<10); // 0
    RCC->PLL_CFGR &= ~(1<< 9); // 0
    RCC->PLL_CFGR &= ~(1<< 8); // 0
    // Final frequency = 4 MHz * (10/1) / 2 = 20 MHz
    
    RCC->CR |= (1<<24);         // Enables PLLON
    RCC->PLL_CFGR |= (1<<24);   // Sets PLLCLK to be the output

    // Now we're setting the clock configuration to use PLL
    RCC->CFGR &= ~(1<<30);  // MCOPRE = 000 (i.e. divide by 1)
    RCC->CFGR &= ~(1<<29);
    RCC->CFGR &= ~(1<<28);

    // Set MCOSEL[3:0] to be 0101 -> set PLL as main clock
    RCC->CFGR &= ~(1<<27);
    RCC->CFGR |=  (1<<26);
    RCC->CFGR &= ~(1<<25);
    RCC->CFGR |=  (1<<24);

    // Set APB high-prescaler to not divide HCLK
    RCC->CFGR &= ~(1<<13);

    // Set APB low-speed prescaler to not divide HCLK
    RCC->CFGR &= ~(1<<10);

    // Don't divide SYSCLK by anything (/1)
    RCC->CFGR &= ~(1<<7);

    // PLL used as the system clock
    RCC->CFGR |= (1<<3);
    RCC->CFGR |= (1<<2);

    return 1;
}
/*************************** End of file ****************************/
