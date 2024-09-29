// STM32L432KC_FLASH.c
// Source code for FLASH functions

#include "lib/STM32L432KC_FLASH.h"

void configureFlash(void) {
  FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
  FLASH->ACR |= FLASH_ACR_PRFTEN;
}