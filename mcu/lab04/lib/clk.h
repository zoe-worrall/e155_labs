#ifndef CLOCK_H
#define CLOCK_H

/**
*   Nucleo-L432KC Singer Header
*
*   @file: clk.h
*   @author: Zoe Worrall
*   @contact: zworrall@g.hmc.edu
*   @version: 09/21/2024
*/

#include <stdint.h>
#define __IO volatile

#define PLL_FREQ 20000000

////////////////////// define RCC /////////////////////////
#define RCC_BASE (0x40021000UL)
/**
*   Defining the memory structure for Reset and Clock Control (RCC)
*
*  Found pages 192-242/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t CR;                   /*< 0x00 Clock Control Register >*/
  __IO uint32_t ICSCR;                /*< 0x04 Internal Clock Soruces Calibration Register >*/
  __IO uint32_t CFGR;                 /*< 0x08 Clock Configuration Register >*/
  __IO uint32_t PLL_CFGR;             /*< 0x0C PLL Configuration Register >*/
  __IO uint32_t PLL_SAI1_CFGR_A;      /*< 0x10 PLLSAI1 Configuration Register >*/
  __IO uint32_t UNASSIGNED_1;
  __IO uint32_t CIER;                 /*< 0x18 Clock Interrupt Enable Register >*/
  __IO uint32_t CIFR;                 /*< 0x1C Clock Interrupt Flag   Register >*/
  __IO uint32_t CICR;                 /*< 0x20 Clock Interrupt Clear  Register >*/
  __IO uint32_t UNASSIGNED_2;       
  __IO uint32_t AHB1_RSTR;            /*< 0x28 AHB1 Peripheral Reset Register >*/
  __IO uint32_t AHB2_RSTR;            /*< 0x2C AHB2 Peripheral Reset Register >*/
  __IO uint32_t AHB3_RSTR;            /*< 0x30 AHB3 Peripheral Reset Register >*/ 
  __IO uint32_t UNASSIGNED_3;
  __IO uint32_t APB1_RSTR_1;          /*< 0x38 APB1 Peripheral Reset Register 1 >*/
  __IO uint32_t APB1_RSTR_2;          /*< 0x3C APB1 Peripheral Reset Register 2 >*/
  __IO uint32_t APB2_RSTR;            /*< 0x40 APB2 Peripheral Reset Register >*/     /* (not available on STM32L432) */
  __IO uint32_t UNASSIGNED_4;     
  __IO uint32_t AHB1_ENR;             /*< 0x48 AHB1 Peripheral Clock Enable Register >*/
  __IO uint32_t AHB2_ENR;             /*< 0x4C AHB2 Peripheral Clock Enable Register >*/
  __IO uint32_t AHB3_ENR;             /*< 0x50 APB3 Peripheral Clock Enable Register >*/
  __IO uint32_t UNASSIGNED_5;
  __IO uint32_t APB1_ENR1;            /*< 0x58 APB1 Peripheral Clock Enable Register 1 >*/
  __IO uint32_t APB1_ENR2;            /*< 0x5C APB1 Peripheral Clock Enable Register 2 >*/
  __IO uint32_t APB2_ENR;             /*< 0x60 APB2 Peripheral Clock Enable Register >*/
  __IO uint32_t UNASSIGNED_6;        
  __IO uint32_t AHB1_SM_ENR;          /*< 0x68 AHB1 Peripheral Clocks Enable in Sleep and Stop Modes Register >*/
  __IO uint32_t AHB2_SM_ENR;          /*< 0x6C AHB2 Peripheral Clocks Enable in Sleep and Stop Modes Register >*/
  __IO uint32_t AHB3_SM_ENR;          /*< 0x70 AHB1 Peripheral Clocks Enable in Sleep and Stop Modes Register >*/
  __IO uint32_t UNASSIGNED_7; 
  __IO uint32_t APB1_SM_ENR1;         /*< 0x78 APB1 Peripheral Clocks Enable in Sleep and Stop Modes Register 1 >*/
  __IO uint32_t APB1_SM_ENR2;         /*< 0x7C APB1 Peripheral Clocks Enable in Sleep and Stop Modes Register 2 >*/
  __IO uint32_t APB2_SM_ENR;          /*< 0x80 APB2 Peripheral Clocks Enable in Sleep and Stop Modes Register >*/
  __IO uint32_t UNASSIGNED_8;
  __IO uint32_t CCIPR;                /*< 0x88 Peripherals Independent Clock Configuration Register >*/
  __IO uint32_t UNASSIGNED_9;
  __IO uint32_t BDCR;                 /*< 0x90 Backup Domain Control Register >*/
  __IO uint32_t CSR;                  /*< 0x94 Control/Status Register >*/
  __IO uint32_t CRR_CR;               /*< 0x98 Clock Recovery RC Register >*/
  __IO uint32_t CCIPR_2;              /*< 0x9C Peripherals Independent Clock Configuration Register >*/
} RCC_STM32L432xx_TypeDef;

#define RCC ((RCC_STM32L432xx_TypeDef *) RCC_BASE)
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Defining the required functions for this class
////////////////////////////////////////////////////////////////////////

 /** 
 *      Configures the phase-locked loop clocks

 *      @Parameter M - The division factor applied to the PLL clock input
 *                 N - The multiplication factor applied to the PLL clock input
 *                 R - The division factor of the output of VCO
 *
 *      The input clock (MSI, HSI16, OR HSE) is selected by a multiplexer.
 *      This signal is then divided by M, multiplied by N within a Voltage
 *         Controlled Oscillator, and then divided by R.
 *
 *      Has several if statements to make sure that all the values are assigned
 *        correctly, because users are stupid.
 *
 *      Sets PLL frequency to 8 MHz
 */
int configurePLL(void);

/**
* Configures a clock (PLL)
*/
int configureClock(void);

#endif