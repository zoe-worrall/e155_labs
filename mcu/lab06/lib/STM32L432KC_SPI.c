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

    // SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_SPE, 0b1);    // make the SPI bus enabled // turn off the spi bus before you receive (bit 6)
    
    ////////////////////// GPIO Enabled ///////////////////////////////////
    // connect GPIO pins to RCC
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    /////////////////////////////////////////////////////////////////////

    //////////////////////// SPI Enabled /////////////////////////////////
    // connect SPI pins to RCC --- pg 245/1600
    // https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    /////////////////////////////////////////////////////////////////////

    //////////////////////// SPI Enabled /////////////////////////////////
    // How to Setup According to Configuration (40.4.7, Pg. 1313)
    // 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
    //gpioEnable(GPIO_PORT_A);
    //gpioEnable(GPIO_PORT_B);

    pinMode(SPI_SCK_PIN, GPIO_ALT);
    pinMode(SPI_MISO_PIN, GPIO_ALT);
    pinMode(SPI_MOSI_PIN, GPIO_ALT);
    //pinMode(SPI_CS_PIN, GPIO_OUTPUT);

    // PA5 ->  AF5, SPI1_SCK
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

    // PA6 ->  AF5, SPI1_MISO
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5);

    // PA12 -> AF5, SPI1_MOSI
    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL12, 5);

    // Set ospeed high with SCK
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //////////////////////// 2. Configuring CR1 and CR2: ///////////////////////////
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BR, 0b111);
    SPI1->CR1 |=  _VAL2FLD(SPI_CR1_BR, br); // set baud rate (BR[2:0]) -- 9600 baud
    
    // configure MSTR (multimaster -- avoid conflict state (NSS master) )
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 0b1); // master configuration

    // set polarity
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_CPOL, cpol);
    SPI1->CR1 |=  _VAL2FLD(SPI_CR1_CPOL, cpol); // configure CPOL and CPHA. NSSP mode -> CPHA must be 0 (what is NSSP)
   
    // set phase
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_CPHA, cpha);
    SPI1->CR1 |=  _VAL2FLD(SPI_CR1_CPHA, cpha);

    SPI1->CR2 |=  _VAL2FLD(SPI_CR2_SSOE, 0b1);   // SS output is enabled in master mode and when the SPI interface is enabled
    
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0b1);  // 8 bits, RXNE event is generated if the FIFO level is greater than or equal to 1/2 (16-bit)

    // data size is whatever the user puts
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 7); // sending numBit number of bits
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////// 2. Turning Everything Else Off ///////////////////////////
    // configure transmit only mode(?)
    // SPI1->CR1 |=  _VAL2FLD(SPI_CR1_CRCL,  0b1);   // select the CRC length: 16-bit-> we aren't using CRC so doesn't matter
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BIDIMODE, 0b1); // set 2-line unidirectional data mode
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_BIDIOE,   0b1); // transmit-only mode
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_LSBFIRST, 0b1); // define the frame format (starts w MSB)
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_CRCEN,  0b1); // don't do CRC calculation'
    SPI1->CR1 &= ~_VAL2FLD(SPI_CR1_SSM,    0b1); // software slave management is disabled
    SPI1->CR2 &= ~_VAL2FLD(SPI_CR2_LDMATX, 0b1); // TXEIE no configuring LDMA_TX (not using DMA)
    SPI1->CR2 &= ~_VAL2FLD(SPI_CR2_LDMARX, 0b1); // RXNEIE no configuring LDMA_RX (not using DMA)
    SPI1->CR2 &= ~_VAL2FLD(SPI_CR2_FRF, 0b1);   // FRF we want to be using the SPI motorola mode (we're not doing a 3-line)
    SPI1->CR2 &= ~_VAL2FLD(SPI_CR2_NSSP, 0b1);  // NSS pulse generated, master mode only (we're in mastermode)
                                                  // allows the SPI to generate an NSS pulse 
                                                  // between two consecutive data 

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
    *((volatile char *) (&SPI1->DR)) = send;  // this is a pointer to a volatile char at the DR address in the SPI block
    while (!(SPI1->SR & SPI_SR_RXNE)); // make sure receive has happened before continuing
    return  SPI1->DR;

    /**
  In any master receive only mode (RXONLY=1 or BIDIMODE=1 & BIDIOE=0), master 
    starts to communicate and the clock starts running immediately after 
    SPI is enabled.
  For handling DMA, follow the dedicated section.
  */
  
}