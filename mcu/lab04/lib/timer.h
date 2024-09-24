#ifndef TIMER_H
#define TIMER_H

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


#define TIM2_BASE  (0x40000000UL)
#define TIM3_BASE  (0x40000400UL)

#define TIM16_BASE (0x40014400UL)
#define TIM15_BASE (0x40014000UL)

#define TIM7_BASE  (0x40001400UL)
#define TIM6_BASE  (0x40001000UL)

/**
*   Defining the memory structure for Timers 2 and 3 (the main ones)
*
*  Found pages 192-242/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t CR1;      /*< 0x00 TIMx Control Register 1 >*/
  __IO uint32_t CR2;      /*< 0x04 TIMx Control Register 2 >*/
  __IO uint32_t SMCR;     /*< 0x08 TIMx Slave Mode Control Register >*/
  __IO uint32_t DIER;     /*< 0x0C TIMx DMA/Interrupt Enable Register >*/
  __IO uint32_t SR;       /*< 0x10 TIMx Status Register >*/
  __IO uint32_t EGR;      /*< 0x14 TIMx Event Generation Register >*/
  __IO uint32_t CCMR1;    /*< 0x18 TIMx Capture/Compare Mode Register 1 >*/
  __IO uint32_t CCMR2;    /*< 0x1C TIMx Capture/Compare Mode Register 2 >*/
  __IO uint32_t CCER;     /*< 0x20 TIMx Capture/Compare Enable Register >*/
  __IO uint32_t CNT;      /*< 0x24 TIMx Counter >*/
  __IO uint32_t PSC;      /*< 0x28 TIMx Prescaler >*/
  __IO uint32_t ARR;      /*< 0x2C TIMx Auto-Reload Register >*/
  uint32_t UNASSIGNED_1;  //    30
  __IO uint32_t CCR1;     /*< 0x34 TIMx Capture/Compare Register 1 >*/
  __IO uint32_t CCR2;     /*< 0x38 TIMx Capture/Compare Register 2 >*/
  __IO uint32_t CCR3;     /*< 0x3C TIMx Capture/Compare Register 3 >*/
  __IO uint32_t CCR4;     /*< 0x40 TIMx Capture/Compare Register 4 >*/
  uint32_t UNASSIGNED_2;  //    44
  __IO uint32_t DCR;      /*< 0x48 TIMx DMA Control Register >*/
  __IO uint32_t DMAR;     /*< 0x4C TIMx DMA Address (full transfer) Register >*/
  __IO uint32_t OR1;      /*< 0x50 TIMx Option Register 1 >*/
  uint32_t UNASSIGNED_3;  //    54
  uint32_t UNASSIGNED_4;  //    58
  uint32_t UNASSIGNED_5;  //    5C
  __IO uint32_t OR2;      /*< 0x60 TIMx Option Register 2 >*/
} TIM_23_STM32L432xx_TypeDef;

#define TIM2 ((TIM_23_STM32L432xx_TypeDef *) TIM2_BASE)
#define TIM3 ((TIM_23_STM32L432xx_TypeDef *) TIM3_BASE)

/**
*   Defining the memory structure for Timer 15
*
*  Found pages 192-242/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t CR1;      /*< 0x00 TIM15 Control Register 1 >*/
  __IO uint32_t CR2;      /*< 0x04 TIM15 Control Register 2 >*/
  __IO uint32_t SMCR;     /*< 0x08 TIM15 Slave Mode Control Register >*/
  __IO uint32_t DIER;     /*< 0x0C TIM15 DMA/Interrupt Enable Register >*/
  __IO uint32_t SR;       /*< 0x10 TIM15 Status Register >*/
  __IO uint32_t EGR;      /*< 0x14 TIM15 Event Generation Register >*/
  __IO uint32_t CCMR1;    /*< 0x18 TIM15 Capture/Compare Mode Register 1 >*/
  uint32_t UNASSIGNED_1;  //    1C
  __IO uint32_t CCER;     /*< 0x20 TIM15 Capture/Compare Enable Register >*/
  __IO uint32_t CNT;      /*< 0x24 TIM15 Counter >*/
  __IO uint32_t PSC;      /*< 0x28 TIM15 Prescaler >*/
  __IO uint32_t ARR;      /*< 0x2C TIM15 Auto-Reload Register >*/
  __IO uint32_t RCR;      /*< 0x30 TIM15 Repetition Counter Register >*/
  __IO uint32_t CCR1;     /*< 0x34 TIM15 Capture/Compare Register 1 >*/
  __IO uint32_t CCR2;     /*< 0x38 TIM15 Capture/Compare Register 2 >*/
  uint32_t UNASSIGNED_2;  //    3C
  uint32_t UNASSIGNED_3;  //    40
  __IO uint32_t BDTR;     /*< 0x44 TIM15 Break and Dead-Time Register >*/
  __IO uint32_t DCR;      /*< 0x48 TIM15 DMA Control Register >*/
  __IO uint32_t DMAR;     /*< 0x4C TIM15 DMA Address (full transfer) Register >*/
  __IO uint32_t OR1;      /*< 0x50 TIM15 Option Register 1 >*/
  uint32_t UNASSIGNED_4;  //    54
  uint32_t UNASSIGNED_5;  //    58
  uint32_t UNASSIGNED_6;  //    5C
  __IO uint32_t OR2;      /*< 0x60 TIM15 Option Register 2 >*/
} TIM15_STM32L432xx_TypeDef;

#define TIM15 ((TIM15_STM32L432xx_TypeDef *) TIM15_BASE)

/**
*   Defining the memory structure for Timers 15 and 16
*
*  Found pages 192-242/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t CR1;      /*< 0x00 TIMx Control Register 1 >*/
  __IO uint32_t CR2;      /*< 0x04 TIMx Control Register 2 >*/
  uint32_t UNASSIGNED_0;  //    08
  __IO uint32_t DIER;     /*< 0x0C TIMx DMA/Interrupt Enable Register >*/
  __IO uint32_t SR;       /*< 0x10 TIMx Status Register >*/
  __IO uint32_t EGR;      /*< 0x14 TIMx Event Generation Register >*/
  __IO uint32_t CCMR1;    /*< 0x18 TIMx Capture/Compare Mode Register 1 >*/
  uint32_t UNASSIGNED_1;  //    1C
  __IO uint32_t CCER;     /*< 0x20 TIMx Capture/Compare Enable Register >*/
  __IO uint32_t CNT;      /*< 0x24 TIMx Counter >*/
  __IO uint32_t PSC;      /*< 0x28 TIMx Prescaler >*/
  __IO uint32_t ARR;      /*< 0x2C TIMx Auto-Reload Register >*/
  __IO uint32_t RCR;      /*< 0x30 TIMx Repetition Counter Register >*/
  __IO uint32_t CCR1;     /*< 0x34 TIMx Capture/Compare Register 1 >*/
  uint32_t UNASSIGNED_1_5;//    38
  uint32_t UNASSIGNED_2;  //    3C
  uint32_t UNASSIGNED_3;  //    40
  __IO uint32_t BDTR;     /*< 0x44 TIMx Break and Dead-Time Register >*/
  __IO uint32_t DCR;      /*< 0x48 TIMx DMA Control Register >*/
  __IO uint32_t DMAR;     /*< 0x4C TIMx DMA Address (full transfer) Register >*/
  __IO uint32_t OR1;      /*< 0x50 TIMx Option Register 1 >*/
  uint32_t UNASSIGNED_4;  //    54
  uint32_t UNASSIGNED_5;  //    58
  uint32_t UNASSIGNED_6;  //    5C
  __IO uint32_t OR2;      /*< 0x60 TIMx Option Register 2 >*/
} TIM_ALL_STM32L432xx_TypeDef;

/**
*   Defining the memory structure for Reset and Clock Control (RCC)
*
*  Found pages 192-242/1600 in the RM0394 Rev 4 Reference Manual for STM32L432XX Boards
*/ 
typedef struct {
  __IO uint32_t CR1;      /*< 0x00 TIM16 Control Register 1 >*/
  __IO uint32_t CR2;      /*< 0x04 TIM16 Control Register 2 >*/
  uint32_t UNASSIGNED_0;  //    08
  __IO uint32_t DIER;     /*< 0x0C TIM16 DMA/Interrupt Enable Register >*/
  __IO uint32_t SR;       /*< 0x10 TIM16 Status Register >*/
  __IO uint32_t EGR;      /*< 0x14 TIM16 Event Generation Register >*/
  __IO uint32_t CCMR1;    /*< 0x18 TIM16 Capture/Compare Mode Register 1 >*/
  uint32_t RESERVED_1;    //    1C
  uint32_t RESERVED_2;    //    20
  __IO uint32_t CNT;      /*< 0x24 TIM16 Counter >*/
  __IO uint32_t PSC;      /*< 0x28 TIM16 Prescaler >*/
  __IO uint32_t ARR;      /*< 0x2C TIM16 Auto-Reload Register >*/
} TIM_67_STM32L432xx_TypeDef; // Tim6 & 7

#define TIM6 ((TIM_67_STM32L432xx_TypeDef *) TIM6_BASE)
#define TIM7 ((TIM_67_STM32L432xx_TypeDef *) TIM7_BASE)

////////////////////////////////////////////////////////////////////////
// Defining the required functions for this class -- see pages 969
////////////////////////////////////////////////////////////////////////

// Sets mode of timer: 
    // 1=Upcounting Mode (1,2,3,1,...), 
    // 2=Repetition Mode,
    // 3=Clock selection
    // 4=Capture/Compare Channels
    // 5=Capture Input Mode
    // 6=PWM Input Mode
    // 7=Forced Output Mode
    // 8=Output Compare Mode
    // 9=PWM Mode**
    // 10=Combined PWM Mode
    // 11=Complementary Outputs and Dead-Time Insertion
    // 12=Using Break Function
    // 13=One-Pulse Mode
    // 14=Retriggerable One Pulse Mode
    // 15=UIF Bit Remapping
    // 16=Timer Input XOR Function
    // 17=External Trigger Synchronization (Reset mode, Gated mode, Trigger mode, Reset+Trigger mode)
    // 18=Combined Reset + Trigger Mode
    // 19=DMA Burst Mode
    // 20=Timer Synchronization
    // 21=Debug Mode
// void setTimerMode(int mode);

// void writeTimer(int counter);

// int readTimer(void);

// Delays for 1 millisecond
void delay(uint32_t time);

int setup_TIM2_CH1_PWM(void);  // returns 1 when clock is set

int configure_TIM2_CH1_PWM(int freq, double duty);  // returns 1 when clock is set

int configure_TIM_PWM(TIM_ALL_STM32L432xx_TypeDef *  TIMx, int freq, double duty);

#endif