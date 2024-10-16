// STM32L432KC_SPI.c
// Zoe Worrall
// zworrall@g.hmc.edu
// October 14, 2024
// Sends and receives data using the STM432's SPI bus

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

// connected clock is HSI
void initSPI(int br, int cpol, int cpha) {
    ////////////////////// GPIO Enabled ///////////////////////////////////
    // connect GPIO pins to RCC
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // How to Setup According to Configuration (40.4.7, Pg. 1313)
    // 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
    gpioEnable(GPIO_PORT_A);
    // PA5 ->  AF5, SPI1_SCK
    pinMode(SPI_SCK_PIN, GPIO_ALT);
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

    // PA6 ->  AF5, SPI1_MISO
    pinMode(SPI_MISO_PIN, GPIO_ALT);
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5);

    // PA12 -> AF5, SPI1_MOSI
    pinMode(SPI_MOSI_PIN, GPIO_ALT);
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRH_AFSEL12, 5);

    // PB0 -> AF5, SPI1_NSS
    gpioEnable(GPIO_PORT_B);
    pinMode(SPI_NSS_PIN, GPIO_ALT);
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL0, 5);

    // Set ospeed high with SCK
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3);

    /////////////////////////////////////////////////////////

    /////////////////// SPI Enabled /////////////////////////////////
    // connect SPI pins to RCC
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    ///////// 2. SPI_CR1: ///////////////////////////
        // Select simplex/half-duplex mode - data frame format -- 16 bits
          // leaving this at 0 for now: unidirectional data mode for 2-lines
        // configure LSBFIRST (frame format) -> not required in TI mode
          // leaving this at 0 for msb first
        // configure SSM and SSI -> not required in TI mode, not required in NSSP mode
          // Recommended in class that we tell NSS to deliver to GPIO
    ///////// 3. SPI_CR2: ////////////////////////////////
        // configure DS[3:0] bits to select data length
        // configure SSOE -> not required in slave, TI, or NSSP mode
            // set FRF bit if need TI protocol (NSS = 0 in TI mode)
        // not setting. FRF at 0 means Motorola mode.
            // set NSSP if NSS pulse mode      (CPHA=0 and TI=0 if NSSP mode)
            // configure FRxTH.                (RxFIFO -> read access size of SPIx_DR reg)
        // need to configure this for 16 bits if we want to send 12-bits in the future
        // letting this stay at 0
            // initialize LDMA_TX and LDMA_RX  >IF< DMA = packed mode
        // not in datapacking mode (?) -- not changing
     ///////// 4. SPI_CRCPR: ////////////////////////////////////
            // configure CRC polynomial if necessary
        // not sure what to do with the polynomial so i'm leaving it
      ////////// 5. Write DMA registers.///////////////////////////
            // configure DMA streams meant for SPI Tx and RX >IF< DMA streams used
        // unnecessary to configure DMA

    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BR, br);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br); // set baud rate (BR[2:0]) -- 9600 baud
    
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_CPOL, cpol);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol); // configure CPOL and CPHA. NSSP mode -> CPHA must be 0 (what is NSSP)
   
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_CPHA, cpha);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);

        // configure CRCL and CRCEN if CRC needed (keep SCK clock idle)
              //while(SPI1->CR1 & SPI_CR1_SPE); // wait until SPI isn't enabled (used in debugging)
              //SPI1->CR1 |= _VAL2FLD(SPI_CR1_CRCL, 0b1); // 16 bit CRC length
              //SPI1->CR1 |= _VAL2FLD(SPI_CR1_CRCEN, 0b1); // enable CRC calculation
    
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_SSM, 0b1); // no need to use SSI
        // configure MSTR (multimaster -- avoid conflict state (NSS master) )
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 0b1); // master configuration

   
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 8);       // sending 8 bits
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 0b1);   // SS output is enabled in master mode and when the SPI interface is enabled
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0b1);  // RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
    
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 0b1);    // make the SPI bus enabled
}

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send) {

  /**
  It is recommended to enable the SPI slave before the master sends the clock. 
    If not, undesired data transmission might occur. The data register of the 
    slave must already contain data to be sent before starting communication 
    with the master (either on the first edge of the communication clock, or 
    before the end of the ongoing communication if the clock signal is 
    continuous). The SCK signal must be settled at an idle state level corresponding
    to the selected polarity before the SPI slave is enabled.
  The master at full-duplex (or in any transmit-only mode) starts to 
    communicate when the SPI is enabled and TXFIFO is not empty, or 
    with the next write to TXFIFO.
    */

    while(!(SPI1->SR & SPI_SR_TXE));  // make sure transmit is empty before setting things
    SPI1->DR = send;                  // make volatile?
    while(!(SPI1->SR & SPI_SR_RXNE)); // make sure receive has happened before continuing
    return SPI1->DR;                  // make volatile?

    /**
  In any master receive only mode (RXONLY=1 or BIDIMODE=1 & BIDIOE=0), master 
    starts to communicate and the clock starts running immediately after 
    SPI is enabled.
  For handling DMA, follow the dedicated section.
  */
  
}