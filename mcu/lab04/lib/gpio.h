#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#define __IO volatile

////////////////////////////// GPIO Definition //////////////////////////////
#define GPIOA_BASE (0x48000000UL)
#define GPIOB_BASE (0x48000400UL)
#define GPIOC_BASE (0x48000800UL)
/**
*   Defining the GPIO Pin Structure
*
*  Found pages 267-274/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t MODER;             /*< 0x00 Port Mode Register >*/
  __IO uint32_t OTYPER;            /*< 0x04 GPIO Port Output Type Register >*/
  __IO uint32_t OSPEEDR;           /*< 0x08 GPIO Port Output Speed Register >*/
  __IO uint32_t PUPDR;             /*< 0x0C GPIO Port Pull-Up/Pull-Down Register >*/
  __IO uint32_t IDR;               /*< 0x10 GPIO Port Input Data Register >*/
  __IO uint32_t ODR;               /*< 0x14 GPIO Port Output Data Register >*/
  __IO uint32_t BSRR;              /*< 0x18 GPIO Port Bit Set/Reset Register >*/
  __IO uint32_t LCKR;              /*< 0x1C GPIO Port Configuration Lock Register >*/
  __IO uint32_t AFRL;              /*< 0x20 GPIO Alternate Function Low Register >*/
  __IO uint32_t AFRH;              /*< 0x24 GPIO Alternate Function High Register >*/
  __IO uint32_t BRR;               /*< 0x28 GPIO Port Bit Reset Register >*/
} GPIO_STM32L432xx_TypeDef;

// defining the address for each of the GPIO Pin bases
#define GPIOA ((GPIO_STM32L432xx_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_STM32L432xx_TypeDef *) GPIOB_BASE)
#define GPIOC ((GPIO_STM32L432xx_TypeDef *) GPIOC_BASE)

////////////////////////////////////////////////////////////////////////

/**
* Defines what mode you're going to be using the GPIOB pin for: 1. input, 2. output, 3. "alternate function", or 4. "analog config"
*
* Returns 1 when properly set
*/
int setModeOfPinB(int pin_num, int mode);

/**
* Defines what mode you're going to be using the GPIOA pin for: 1. input, 2. output, 3. "alternate function", or 4. "analog config"
*
* Returns 1 when properly set
*/
int setModeOfPinA(int pin_num, int mode);

/**
*  Read the input from a pin
*
*     @return -- the voltage being put into the pin
*/
int readPinInput(int pin);

/**
*  Set the output of a pin from GPIOB
*/
void setPinBOutput(int pin, int value);

/**
*  Set the output of a pin from GPIOA
*/
void setPinAOutput(int pin, int value);

#endif