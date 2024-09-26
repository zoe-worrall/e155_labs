/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*                    Zoe Worrall, HMC, 09/20/2024                    *
*                    Contact: zworrall@g.hmc.edu                     *
*                                                                    *
**********************************************************************

  This C program contains four functions:

  setModeofPinA and setModeofPinB set the modes of the A and B GPIO pins on
      the STM32L432xx Board.

*/

//////////////////////////// DEFINITIONS AND INCLUSIONS ////////////////////////////
#include "timer.h"
#include "clk.h"
#include <math.h>


#define CURR_TIM TIM2
#define ETF_DIV_CLKINT8 0b0011

// fCK_PSC / (PSC[15:0] + 1).
#define PSC_DIV_152 32
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////// Function Assignment ////////////////////////////

/***********************************
*                                  *
*    configure_TIM23_PWM()         *
*
*  Set up Timer 2 or 3 as a PWM.
*
*  Uses MSI clock as an input. Since MSI is not configured for this setup, it remains at
*   it's initial frequency of 20 MHz.
*
*
*    @param TIMx -- A pointer to the timer being configured
*    @param freq -- The frequency to which it is being set
*    @param duty -- The desired duty cycle of the timer
*
*    @return     -- 1 if done configuring
*/
int configure_TIM23_PWM(TIM_23_STM32L432xx_TypeDef *  TIMx, int freq, double duty) {
  TIMx->CCMR1 &= ~(0b11 << 0); // CC1S
  TIMx->CCMR1 &= ~(1<<7);      // OC1CE
  TIMx->CCMR1 &= ~(1<<16);     // OC1M[3]
  TIMx->CCMR1 |= (0b111 << 4); // OC1M[2:0]
  TIMx->CCER |=  (1<<0);       // CC1E
  TIMx->CCER &= ~(1<<1);       // CC1P
  TIMx->SMCR &= ~(1<<14);      // ECE 

  // SMS[3:0]:Slave mode selection, Bits 16,2,1,0. Disable, so that internal clock is used CEN enabled)
  TIMx->SMCR &= ~(1<<16); TIM2->SMCR &= ~(1<<2); TIM2->SMCR &= ~(1<<1); TIM2->SMCR &= ~(1<<0);

  TIMx->SMCR &= ~(0b1111 << 8);
  TIMx->SMCR |=  (ETF_DIV_CLKINT8 << 8);  // ETF [3:0]

  TIMx->PSC  &= ~(0b1111111111111111<<0);  // controls how much you divide the incoming clock; let's keep it at 152 
                                            //   (dividing by 2^15 makes it about 0.5 Hz if you keep the duty cycle 50% :) )
  TIMx->PSC  |=  (PSC_DIV_152<<0);

  /////// CALCULATE FREQUENCY ////////
  int GPIO_FREQ = PLL_FREQ / (8 * PSC_DIV_152);     // divide by ETF and (PSC + 1)

  double log_scale = log2( (GPIO_FREQ + 0.0) / freq); // prevent from losing accuracy in long division
  int best_scale = ceil(log_scale) - 16;            // if freq = 220, this is 0
  if (best_scale < 0) best_scale = 0;

  TIMx->ARR &=  ~(0b1111111111111111<<16); // (pg 767) counter just counts continuously between 0 and the auto-reload value in the TIMx_ARR register
  TIMx->ARR &=  ~(0b1111111111111111<<0);

  uint32_t f_scale = GPIO_FREQ / pow(2, best_scale); 
  uint32_t forARR = 0;
  if (freq > 0) {
    forARR = ceil(f_scale / freq); // used to set frequency (?)
    TIMx->ARR |= (forARR<<0);
  } else { // don't set ARR if the frequency is 0
   TIMx->ARR |=   (1<<15);   // maximum possible is set to 2^15, although we could go higher
   }
    ///////////////////////////////////// (Counter goes from 0 -> ARR)

  // for the duty cycle (0 when CNT > CCR1, 1 when CNT < CCR1 -- normally I want duty of 50%)
  /////// CALCULATE DUTY ////////
  TIMx->CCR1 &= ~(0b111111111111111<<0);

  uint32_t this_cycle = forARR * duty;
  if (duty != 0) {
    TIMx->CCR1 |=  (this_cycle << 0);                    // TIMx_CCRx -> CCR1  (count up to this number PWM mode 2 - TIMx_CNT<TIMx_CCR1 else active )
  } else {
    TIMx->CCR1 |= (1 << 0);
  }
  ///////////////////////////////

  TIMx->CCMR1 |= (1<<3);
  TIMx->CR1 |= (1<<7); // ARPE
  TIMx->EGR |= (1<<1); // CC1G
  TIMx->EGR |= (1<<0); // UG
  TIMx->CR1 &= ~(0b11 << 5); // CMS; 00, Edge Aligned Mode (direction dependent on DIR)
  TIMx->CR1 &= ~(1<<4);      // DIR; 0, counter is upcounter
  TIMx->CR1 |=  (1<<0);      // CEN: 1, enable the counter

  return 1;
}



/***********************************
*                                  *
*    setup_TIM2_CH1_PWM()          *
*
*  Sets Timer 2's first Channel (CH1) in PWM mode 1, and outputs it.
*   Find additional information on page 906  of Reference Manual
*
*  Uses MSI clock as an input. Since MSI is not configured for this setup, it remains at
*   it's initial frequency of 20 MHz.
*
*   Configured to output 20 MHz / (256 * 8) or 9765.62 Hz
*
*    @return     -- 1 if done setting up PWM channel
*/
int setup_TIM2_CH1_PWM(void) {  // aiming to set up CH1 on TIM2 as PWM
// to setup output of timer to CHy:
  // (OCy) Output Control on for y
      // Output Enable -->   TIMx_CCER -> CCyE
      //                     TIMx_BDTR -> MOE, OSSI
      //                     TIMx_CR2  -> OIS4

      
  // setup Output Selector for OCyREF: TIMx_CCMR2 -> OCyCE, OC1M[3:0]
  TIM2->CCMR1 &= ~(0b11 << 0); // CC1S: CC1 channel is configured as output. 
                                      // CC1S bits are writable only when the channel 
                                      //  is OFF (CC1E = 0 in TIMx_CCER).
  TIM2->CCMR1 &= ~(1<<7);  // OC1CE,  0: OC1Ref is not affected by the ETRF input
  TIM2->CCMR1 &= ~(1<<16); // OC1M[3]:Output Compare 1 mode-bit 3
  TIM2->CCMR1 |= (0b111 << 4); // OC1M[2:0] PWM mode 2 - TIMx_CNT<TIMx_CCR1 else active.
                            // Make sure that CCRy is on, and directing things to OCyREF


  // use Mux for output: TIMx_CCER -> CCyP = 0
  // use Mux for output: TIMx_CCER -> CCyE = 1
  TIM2->CCER |= (1<<0); // CC1E (ENABLED) enabled  (OC1 is output)
  TIM2->CCER &= ~(1<<1); // CC1P (POLARITY) set 0 (OC1 is active high)   // Program the OCy polarity to active high

  // Slave controller mode is set to counting: 000 is INT_CLK
      // CK_INT goes to Slave controller CK_PSC
      // internal clock mode: select using TIMx_SMCR
            // ECE
            // SMS[2:0]
  TIM2->SMCR &= ~(1<<14); // make sure ECE is turned off (use internal clock)


  /// MORE PWM SETUP ///
  // SMS[3:0]:Slave mode selection, Bits 16,2,1,0. Disable, so that internal clock is used CEN enabled)
  TIM2->SMCR &= ~(1<<16); TIM2->SMCR &= ~(1<<2); TIM2->SMCR &= ~(1<<1); TIM2->SMCR &= ~(1<<0);
  
  TIM2->SMCR &= ~(0b1111 << 8);
  TIM2->SMCR |=  (0b0011 << 8);  // ETF [3:0], frequency = 1/8 of f_CK_INT

  // set prescaling ( CK_PSC / (PSC[15:0] + 1))
  TIM2->PSC |= (0b0000000100000000);  // downscale by 152

  // to specifically set up PWM mode on a Timer
      // TIMx_ARR  -> frequency
  TIM2->ARR &=  ~(0b1111111111111111<<16); //  (pg 767) counter just counts continuously between 0 and the auto-reload value in the TIMx_ARR register
  TIM2->ARR &=  ~(0b1111111111111111<<0);
  TIM2->ARR |= (1<<15);

      // TIMx_CCRx -> duty cycle
  TIM2->CCR1 &= ~(0b111111111111111<<0);
  TIM2->CCR1 |=  (1<<14);

      // Enable the preload register:             TIMx_CCMRx -> OCxPE
  TIM2->CCMR1 |= (1<<3); // 1: Preload register on TIMx_CCR1 enabled. 
                         //     Read/Write operations access the preload 
                         //     register. TIMx_CCR1 preload value is 
                         //     loaded in the active register at each update event.

      // Set the auto-reload preload register:    TIMx_CR1   -> ARPE
  TIM2->CR1 |= (1<<7); // ARPE = 1, register ARR is buffered

      // Set the UG bit in TIMx_EGR
  TIM2->EGR |= (1<<1); // CC1G = 1, 1: A capture/compare event is generated on channel 1: If channel CC1 is configured as output,
                       //     CC1IF flag is set, Corresponding interrupt or DMA request is sent if enabled.
  TIM2->EGR |= (1<<0); // UG = 1, Re-initialize the counter and generates 
                       //     an update of the registers.

      // Assign edge-aligned or center-aligned mode: TIMx_CR1 -> CMS
  TIM2->CR1 &= ~(0b11 << 5); // CMS; 00, Edge Aligned Mode (direction dependent on DIR)
  TIM2->CR1 &= ~(1<<4);      // DIR; 0, counter is upcounter
  TIM2->CR1 |=  (1<<0);      // CEN: 1, enable the counter


  return 1;
}



/***********************************
*                                  *
*    delay()                       *
*
*  Uses a generic clock to wait for some determined amount of time (milliseconds).
*
*  Uses MSI clock as an input. Since MSI is not configured for this setup, it remains at
*   it's initial frequency of 20 MHz. Assumes that the clock is configured at a PSC of 512.
*
*   Configured to output 20 MHz / 512 or 39062.5Hz
*
*    @param DELAY_TIMx -- A pointer to the timer that you are using to delay
*    @param ms         -- The amount of milliseconds that should be delayed
*/
void delay(TIM_67_STM32L432xx_TypeDef * DELAY_TIMx, uint32_t ms) {
  int wait_till = (ms * 2000) + DELAY_TIMx->CNT;
  for (int i=0; i<wait_till; i++);
}

/**
*  Configures any Timer (BESIDES TIM2!!) to run at 8 kHz
*/
/***********************************
*                                  *
*    configure_TIMx()              *
*
*  Configures any Timer (BESIDES TIM2!!) to run at 39 kHz.
*
*  Uses MSI clock as an input. Since MSI is not configured for this setup, it remains at
*   it's initial frequency of 20 MHz. Assumes that the clock is configured at a PSC of 512.
*
*   Configured to output 20 MHz / 512 or 39062.5Hz
*
*    @param DELAY_TIMx -- A pointer to the timer that you are creating
*
*    @return           -- 1 once the function completes configuration
*/
int configure_TIMx(TIM_67_STM32L432xx_TypeDef *  TIMx) {
  TIMx->CCMR1 &= ~(0b11 << 0); // CC1S
  TIMx->CCMR1 &= ~(1<<7);      // OC1CE
  TIMx->CCMR1 &= ~(1<<16);     // OC1M[3]
  TIMx->CCMR1 |= (0b111 << 4); // OC1M[2:0]
  TIMx->PSC  &= ~(0b1111111111111111<<0);  // controls how much you divide the incoming clock; let's keep it at 152 
                                            //   (dividing by 2^15 makes it about 0.5 Hz if you keep the duty cycle 50% :) )
  TIMx->PSC  |=  (512<<0);
  TIMx->ARR &=  ~(0b1111111111111111<<0);  // assumption is made here that you aren't using TIM2
  TIMx->ARR |=   (0b1000000000000000<<0);  // ARR capable of changing the timer from 16 kHz to 0.5 Hz; here, we set it so that counter
                                              // is repeating every 0.5 Hz.

  TIMx->CCMR1 |= (1<<3);
  TIMx->CR1 |= (1<<7); // ARPE
  TIMx->EGR |= (1<<1); // CC1G
  TIMx->EGR |= (1<<0); // UG
  TIMx->CR1 &= ~(0b11 << 5); // CMS; 00, Edge Aligned Mode (direction dependent on DIR)
  TIMx->CR1 &= ~(1<<4);      // DIR; 0, counter is upcounter
  TIMx->CR1 |=  (1<<0);      // CEN: 1, enable the counter
}


/*************************** End of file ****************************/
