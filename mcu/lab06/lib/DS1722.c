// DS1722.c
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#include "STM32L432KC_SPI.h"
#include "DS1722.h"

char globalConfig;

// returns the temperature of the SPI
char readTemp(int resolutionBits) {
  ///////////// configure the data that needs to be sent into the SPI (see Figure 2 on DS1722 datasheet) /////////////
  char config = 0;
 
  // shift in MSb  -> set to be divided by 256 atm
  config |= (0b111 << 5);
  globalConfig = config;

  // shift in the 1SHOT bit
  config |= (0b1 << 4);
  globalConfig = config;

  // shift in the resolution bits
  switch(resolutionBits) {
    case 8:  config |= (EIGHT_BIT << 1); break;
    case 9:  config |= (NINE_BIT  << 1); break;
    case 10: config |= (TEN_BIT   << 1); break;
    case 11: config |= (ELEVEN_BIT << 1); break;
    case 12: config |= (TWELVE_BIT << 1); break;
    default: break;
  }
  globalConfig = config;

  // shift in the shutdown bit
  config |= (0b0 << 0);
  globalConfig = config;

  char temperature = spiSendReceive(config);
  globalConfig = config;
  return temperature;
}