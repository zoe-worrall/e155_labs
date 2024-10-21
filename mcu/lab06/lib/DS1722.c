// DS1722.c
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#include "STM32L432KC_SPI.h"
#include <math.h>
#include "DS1722.h"

#define READ_SPI_POL 1
#define WRITE_SPI_POL 0

char globalConfig;
uint16_t twoBytes;

// pin 5: SDO = MOSI, SDO goes to Master In (MISO, PA6)
// pin 6: SDI = MISO, SDI goes to Master Out (MOSI, PA12)
/**
#define SPI_SCK_PIN  PA5    // when triggered, should go up and down
#define SPI_MISO_PIN PA6    // when triggered, shouldn't have any output
#define SPI_MOSI_PIN PA12   // when triggered, has an output
#define SPI_CS_PIN   PA11   // chip select - only high at very beginning of the signal
*/

#define ADDRESS_READ_CONFIG 0x00
#define ADDRESS_READ_LSB    0X01
#define ADDRESS_READ_MSB    0x02
#define ADDRESS_WRIT_CONFIG 0x80


// multi-byte burst
// Address and data bytes are shifted MSB first into the serial data input (SDI) and out of the serial data
//output (SDO). Any transfer requires the address of the byte to specify a write or a read, followed by one
//or more bytes of data. Data is transferred out of the SDO for a read operation and into the SDI for a write
//operation.

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
void configSensor(int resolutionBits) {

   ///////////// configure the data that needs to be sent into the SPI (see Figure 2 on DS1722 datasheet) /////////////
  char config = 0;
 
  // shift in MSb  -> set to be divided by 256 atm
  config |= (0b111 << 5);

  // shift in the 1SHOT bit
  config |= (0b1 << 4);

  // shift in the resolution bits
  switch(resolutionBits) {
    case 8:  config |= (EIGHT_BIT << 1); break;
    case 9:  config |= (NINE_BIT  << 1); break;
    case 10: config |= (TEN_BIT   << 1); break;
    case 11: config |= (ELEVEN_BIT << 1); break;
    case 12: config |= (TWELVE_BIT << 1); break;
    default: break;
  }
  // shift in the shutdown bit
  config |= (0b0 << 0);
  globalConfig = config;
  ///////////// ///////////// ///////////// ///////////// ///////////// 

    digitalWrite(SPI_CS_PIN, 1);
    spiSendReceive(ADDRESS_WRIT_CONFIG);
    spiSendReceive(config);
    digitalWrite(SPI_CS_PIN, 0);

}

char readConfig(void) {
  spiSendReceive(ADDRESS_READ_CONFIG);
  spiSendReceive(0x00);

  return 0xAB;
}

char readMSb(void) {
  digitalWrite(CS_PIN, PIO_HIGH);
  spiSendReceive(ADDRESS_READ_MSB);
  spiSendReceive(0x00);
  digitalWrite(CS_PIN, PIO_LOW);

  return 0xAB;
}

// returns the temperature of the SPI
double readTemp(void) {
  // 1111xxx0
  digitalWrite(CS_PIN, 1);
  spiSendReceive(ADDRESS_READ_MSB);
  char msb_temp = spiSendReceive(0x00);
  spiSendReceive(ADDRESS_READ_LSB);
  char lsb_temp = spiSendReceive(0x00);
  digitalWrite(CS_PIN, 0);

  twoBytes = (msb_temp << 8) | lsb_temp;

  double sum = 0;

  // MSb to LSb, adding all sums
  if (twoBytes >> 15 & 0b1) { sum += -1 * pow(2, 7); }
  if (twoBytes >> 14 & 0b1) { sum += 1 * pow(2, 6); }
  if (twoBytes >> 13 & 0b1) { sum += 1 * pow(2, 5); }
  if (twoBytes >> 12 & 0b1) { sum += 1 * pow(2, 4); }
  if (twoBytes >> 11 & 0b1) { sum += 1 * pow(2, 3); }
  if (twoBytes >> 10 & 0b1) { sum += 1 * pow(2, 2); }
  if (twoBytes >>  9 & 0b1) { sum += 1 * pow(2, 1); }
  if (twoBytes >>  8 & 0b1) { sum += 1 * pow(2, 0); }
  if (twoBytes >>  7 & 0b1) { sum += 1 * pow(2, -1); }
  if (twoBytes >>  6 & 0b1) { sum += 1 * pow(2, -2); }
  if (twoBytes >>  5 & 0b1) { sum += 1 * pow(2, -3); }
  if (twoBytes >>  4 & 0b1) { sum += 1 * pow(2, -4); }

  return sum;
}