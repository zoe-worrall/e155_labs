/**
*   Nucleo-L432KC Singer Header
*
*   @file: zoe_def_stm32l432xx.h
*   @author: Zoe Worrall
*   @contact: zworrall@g.hmc.edu
*   @version: 09/20/2024
*/

#include <stdint.h>
#include "zoe_def_stm32l432xx.h"

#define PLL_FREQ 8000000UL // 8_000_000 (8 MHz)
#define ONE_MS PLL_FREQ * 8000

#define PB3  // button 3

/**
*   Plays a single note assuming PLL is set to 8MHz
*/
int playNote(int freq, int duration);

/**
* Delays for some number of ms
*/
void delay(int ms);

/**
*  Runs the frequency slower for the given speaker
*/
int do_freq(int freq);