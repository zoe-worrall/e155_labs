// Nucleo-L432KC Blink demo
// Josh Brake
// jbrake@hmc.edu
// 9/21/22

#include <stdint.h>
#define GPIOB_BASE_ADR (0X48000400UL)                           
#define RCC_BASE_ADR   (0x40021000UL)                           /// register and clock control(?)
#define RCC_AHB2ENR    ((uint32_t *) (RCC_BASE_ADR +   0x4C))   /// Where to go to enable the RCC register (ENR)
#define GPIOB_MODER    ((uint32_t *) (GPIOB_BASE_ADR + 0x00))   /// GPIO B Register Mode (pointer to a uint32)
#define GPIOB_ODR      ((uint32_t *) (GPIOB_BASE_ADR + 0x14))   /// output data register (pointer to a uint32) (what we're changing)
#define DUMMY_DELAY    100000

#define LED_PIN   3  // this is PB3



void delay(int cycles) {
  while (cycles-- > 0) {
    volatile int x=200;
    while (x-- > 0);
      __asm("nop");
    }
}

/*****************************************************************\
*                           main.c
*
* Loops through the function and turns the LED on and off as we do
*/
int main(void) {
  *RCC_AHB2ENR |= (1<<1); // enables

  // need to set [7][6] to 01 -> configuring a GPIO pin as an output
  // these act together : we are powering PB3, so we use MODE3, which corresponds with MODE3 in the GPIOB bus 
      // tables 8.12 in https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
  *GPIOB_MODER &= ~(1 << 7);  // GPIO (xxx7xxxx) && (11101111)
  *GPIOB_MODER |=  (1 << 6);  // GPIO (xxxx6xx) || (0000100)

  while (1) {
    delay(DUMMY_DELAY);
    *GPIOB_ODR |= (1 << LED_PIN);
    delay(DUMMY_DELAY);
    *GPIOB_ODR &= ~(1 << LED_PIN);
  }

}

