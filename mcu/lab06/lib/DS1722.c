// DS1722.c
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>


#include "STM32L432KC_SPI.h"

// returns the temperature of the SPI
char readTemp(void) {
  char temperature = spiSendReceive('a');
  return temperature;
}