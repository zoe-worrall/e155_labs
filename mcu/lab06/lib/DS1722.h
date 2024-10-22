// DS1722.h
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// Builds header to send and receive SPI data

#include "STM32L432KC_SPI.h"

// Defining Thermometer Resolution Bits
#define EIGHT_BIT   0b000
#define NINE_BIT    0b001
#define TEN_BIT     0b010
#define ELEVEN_BIT  0b011
#define TWELVE_BIT  0b100

#define CS_PIN PA8

// makes the DS1722 sensor's configuration: first, says that we're writing
// then, configures the system.
// to write, it needs to:
//    1. write address: A7 A6 A5 A4 A3 A2 A2 A1 A0
//                      
// CPHA bit must be 1 (starts high)
// If CPOL is 1, read on rising edge
// note that this means data should be sent on falling edge 
    //             (i.e. when SPI is configured for PHA=0)
// configures the clock using a Polarity 1, Phase 1 SPI
void configSensor(int resolutionBits);


// Uses the spi system to receive SPI using send and receive
double readTemp(void);

char readConfig(void);

char readMSb(void);