// STM32L432KC_SPI.h
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// Header for the SPI C program. Initializes all necessary functions that must be performed by SPI.

#ifndef STM32L4_SPI_H
#define STM32L4_SPI_H

#include <stdint.h>
#include <stm32l432xx.h>
#include "STM32L432KC_GPIO.h"

#define SPI_SCK_PIN  PA5    // when triggered, should go up and down
#define SPI_MISO_PIN PA6    // when triggered, shouldn't have any output
#define SPI_MOSI_PIN PA12   // when triggered, has an output
#define SPI_CS_PIN   PA11   // chip select - only high at very beginning of the signal

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
 *          1: data changed on leading edge of clk and captured on next edge)
 *    -- numBits: The number of bits that will be sent over SPI to the device - can only be 8, 9, 10, 11, or 12
 * Refer to the datasheet for more low-level details. */ 
void initSPI(int br, int cpol, int cpha);


/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send);

#endif