/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*                    Zoe Worrall, HMC, 09/20/2024                    *
*                                                                    *
*
**********************************************************************
*/
#include "zoe_def_stm32l432xx.h"
#include "playNote.h"
#include <stdio.h>

/***********************************
*                                  *
*           main()                 *
*
*  Function description
*   Application entry point.
*/
int main(void) {

  configurePLL();

  // Testing if I can play 220 Hz Noise for 10 seconds
  playNote(20, 10000);
}

/*************************** End of file ****************************/
