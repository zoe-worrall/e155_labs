/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*                    Zoe Worrall, HMC, 09/20/2024                    *
*                                                                    *
*                                                                    *
**********************************************************************
*/
#include "gpio.h"

/***********************************
*                                  *
*       setModeOfPin()             *
*
*  Set the mode of some pin on the board
*
*  @param:   pin -- the pin number
*            mode -- the mode that you're using (1=Input, 2=Output, 3=Alternate Function, 4=Analog), 
*            gpio -- which GPIO you're using (1=GPIOA, 2=GPIOB, 3=GPIOC)
*  @return   True if the pin is set, false if it's not
*/
int setModeOfPinB(int pin, int mode) {
  switch (mode) {
    case 1: { GPIOB->MODER &= ~(1 << pin); break; }
    case 2: { GPIOB->MODER &= ~(0b11 << pin); GPIOB->MODER |= (0b01 << pin); break; }
    case 3: { GPIOB->MODER &= ~(0b11 << pin); GPIOB->MODER |= (0b10 << pin); break; }
    case 4: { GPIOB->MODER |= (0b11 << pin); break; }
    default: return 0;
  }
  return 1;
}


/***********************************
*                                  *
*       setModeOfPin()             *
*
*  Set the mode of some pin on the board
*
*  @param:   pin -- the pin number
*            mode -- the mode that you're using (1=Input, 2=Output, 3=Alternate Function, 4=Analog), 
*            gpio -- which GPIO you're using (1=GPIOA, 2=GPIOB, 3=GPIOC)
*  @return   True if the pin is set, false if it's not
*/
int setModeOfPinA(int pin, int mode) {
  switch (mode) {
    case 1: { GPIOA->MODER &= ~(1 << pin); break; }
    case 2: { GPIOA->MODER &= ~(0b11 << pin); GPIOA->MODER |= (0b01 << pin); break; }
    case 3: { GPIOA->MODER &= ~(0b11 << pin); GPIOA->MODER |= (0b10 << pin); break; }
    case 4: { GPIOA->MODER |= (0b11 << pin); break; }
    default: return 0;
  }
  return 1;
}

/**
*  Read the input from a pin
*     @param -- pin
*     @return -- whether the pin is on or off
*/
int readPinInput(int pin) {
  return ((GPIOB->IDR >> pin) & 1);
}

/**
*  Set the output of a pin
*/
void setPinBOutput(int pin, int value) {
  GPIOB->ODR &= ~(1 << pin);
  if (value > 1) return; // make sure that we don't break the other bits by entering a value >1.
  GPIOB->ODR |=  (value << pin);
}

/**
*  Set the output of a pin
*/
void setPinAOutput(int pin, int value) {
  GPIOA->ODR &= ~(1 << pin);
  if (value > 1) return; // make sure that we don't break the other bits by entering a value >1.
  GPIOA->ODR |=  (value << pin);
}