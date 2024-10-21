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

void configSensor(int resolutionBits);


// Uses the spi system to receive SPI using send and receive
double readTemp(void);

char readConfig(void);

char readMSb(void);