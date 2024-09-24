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
#include "timer.h"
#include "clk.h"
#include <math.h>


#define CURR_TIM TIM2

int configure_TIM23_PWM(TIM_23_STM32L432xx_TypeDef *  TIMx, int freq, double duty) {
  TIMx->CCMR1 &= ~(0b11 << 0); // CC1S
  TIMx->CCMR1 &= ~(1<<7);      // OC1CE
  TIMx->CCMR1 &= ~(1<<16);     // OC1M[3]
  TIMx->CCMR1 |= (0b111 << 4); // OC1M[2:0]
  TIMx->CCER |=  (1<<0);        // CC1E
  TIMx->CCER &= ~(1<<1);       // CC1P
  TIMx->SMCR &= ~(1<<14);      // ECE 

  // SMS[3:0]:Slave mode selection, Bits 16,2,1,0. Disable, so that internal clock is used CEN enabled)
  TIM2->SMCR &= ~(1<<16); TIM2->SMCR &= ~(1<<2); TIM2->SMCR &= ~(1<<1); TIM2->SMCR &= ~(1<<0);

  TIM2->SMCR &= ~(0b1111 << 8);
  TIM2->SMCR |=  (0b0011 << 8);  // ETF [3:0]

  double log_scale = log2( (PLL_FREQ + 0.0) / freq); // prevent from losing accuracy in long division
  int best_scale = ceil(log_scale) - 16;            // if freq = 220, this is 0
  if (best_scale < 0) best_scale = 0;

  TIM2-> &= ~(0b1111111111111111<<0);
  if (freq) {
      uint32_t f_scale = PLL_FREQ / pow(2, best_scale); 
      uint32_t forARR = f_scale / freq; // used to set frequency (?)
      TIM2->ARR |= (forARR<<0);

  TIM2->PSC  &= ~(0b000000000000000<<0);
  TIM2->PSC  |=  (0b000000100000000<<0);
  TIM2->ARR &=  ~(0b1111111111111111<<16); // (pg 767) counter just counts continuously between 0 and the auto-reload value in the TIMx_ARR register
  TIM2->ARR &=  ~(0b1111111111111111<<0);
  TIM2->ARR |=   (1<<15);
  TIM2->CCR1 &= ~(0b111111111111111<<0);
  TIM2->CCR1 |=  (1<<14);       // TIMx_CCRx -> duty cycle
  TIM2->CCMR1 |= (1<<3);
  TIM2->CR1 |= (1<<7); // ARPE
  TIM2->EGR |= (1<<1); // CC1G
  TIM2->EGR |= (1<<0); // UG
  TIM2->CR1 &= ~(0b11 << 5); // CMS; 00, Edge Aligned Mode (direction dependent on DIR)
  TIM2->CR1 &= ~(1<<4);      // DIR; 0, counter is upcounter
  TIM2->CR1 |=  (1<<0);      // CEN: 1, enable the counter

  return 1;
}

/***********************************
* Sets the timer to be PWM mode on TIM15: Find on page 906  *
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
  TIM2->PSC |= (0b000000100000000);  // downscale by 152

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



/**
*  Sets the timer up to generate a PWM signal of a given frequency and duty cycle
*/
int configure_TIM2_CH1_PWM(int freq, double duty) { // spec. for CH2 of Device
 // incoming frequency is 16.447 kHz
 // 
}


void delay(uint32_t time){
  for (volatile int i=0; i<(time); i++);
}






// void writeTimer(int counter)

// int readTimer(void);


// @zoe: To Remember

  // Set auto-reload preload register to upcounting via TIM15,  CR1,  ARPE
      // upcounting: count up to TIM15_ARR register -- we need the shadow registers enabled
      // Disabling UEV avoids updating shadow registers when writing to preload reg -> no update until UDIS = 0
      // Repetition register (RCR) not important, but is loaded when update is sent

  // Set counter clock (i.e. internal, external, prescaler)

  // write desired data to TIMx_ARR and TIMx_CCRx

  // set CCxIE bit (if doing interrupts, which I'm not at the moment)

  // Select Output Mode -- OCxM  = 011 to toggle OCx output when CNT = CCRx
                        // OCxPE = 0   to disable preload reg -> must be 0 is you want to change output waveform at any time
                        // CCxP  = 0   to active high
                        // CCxE  = 1   to enable output***

  // Enable counter (CEN bit in TIMx_CR1)

  /**
   from setup of Power to Timer 2, Ch1
      // Compliance with OCREF_CLR, OCREF can only be asserted if
          // the result of the comparison..

  old from configureTimer
  //////////// Setting up Clock Control Register  ////////////
  TIM2->CR1 |=  (1<<7);   // ARPE - make auto-reload preload enable buffered
  TIM2->CR1 &= ~(1<<3);   // OPM  - One Pulse Mode OFF, make sure that counter isn't stopped at update event
  TIM2->CR1 &= ~(1<<1);   // UDIS - Update Disable (I want updates to happen every UG or UEV event)
  TIM2->CR1 |=  (1<<0);   // CEN  - Enable the counter
  ////////////////////////////////////////////////////////////
  
  // in channel 2:
  
  //////////// Setting Output of Timer  ////////////
  TIM2->CR2 |=  (1<<0);      // CCPC = 1, CCxE, CCxNE and OCxM bits are preloaded, after 
                                      //   having been written, they are updated only when a 
                                      //   commutation event (COM) occurs (COMG bit set or 
                                      //   rising edge detected on TRGI, depending on the CCUS bit).
  TIM2->CR2 &= ~(0b111<<4);
  TIM2->CR2 |=  (0b101<<4); // MMS: Compare - OC2REF signal is used as trigger output (TRGO)
  TIM2->CR2 &= ~(1<<7);     // TIS: (0) The TIMx_CH1 pin is connected to TI1 input
  TIM2->CR2 |=  (1<<8);     // OIS1: Output Idle state 1 (OC1 output) -- turned on(?)

  ////////////////////////////////////////////////////////////

  //////////// Setting UG (Update Generator)  ////////////
  TIM2->EGR |= (1<<0); // Reinitialize counter + update registers -- UG (update generate)
  ////////////////////////////////////////////////////////////
*/

/*
  //////////// Set PWM Mode 1  ////////////
  TIM2->CCMR1 &= ~(1 << 16);  // OC1M[3] -- OutputCompare2mode-bit3 output compare mode is for CC2
  TIM2->CCMR1 &= ~(0b111<<4);
  TIM2->CCMR1 |=  (0b110<<4); // OC1M[2:0]: PWM Mode 1 turned on for Channel 2
              // basically sets this pin to PWM mode 1, - Channel 1 is active as long as 
                   // TIMx_CNT<TIMx_CCR1 else inactive.  0111: PWM mode 2 - Channel 1 
                   // is inactive as long as TIMx_CNT<TIMx_CCR1 else active.

  TIM2->CCMR1 |= (1<<3);  // sets OC1PE to 1 (Preload register on TIMx_CCR1 enabled. 
                                  //      Read/Write operations access the preload register. 
                                  //      TIMx_CCR1 preload value is loaded in the active 
                                  //      register at each update event.
  // CURR_TIM->CCMR1 |= (1<<15); // OC2CE (output compare 2 clear enable)
                            
  TIM2->CCMR1 &= ~(0b11<<0);  // CCIS -- Set CC1 acts as Output
  ////////////////////////////////////////////////

  // Set Capture and Enable Registers //
  TIM2->CCER |= (1<<0); // CC1E (enabled) - output of Output Selected used. 1: On - OC1 signal is output on the corresponding output pin
  TIM2->CCER &= ~(1<<1); // CC1P 0: OC1 active high

  //////////// COUNTER CLOCK SCALED ////////////
  TIM2->PSC &= ~(0b1111111111111111 << 0);
  TIM2->PSC |=  (20 << 0);  // scale down the 20 MHz clock by 10 to 1 MHz (since final counter can only go up to 65k)
  //////////////////////////////////////////////

  //////////// DUTY CYCLE ////////////
  //CURR_TIM->CNT &= ~(0b1111111111111111 << 0);
  //int full_cycle = pow(2,16);
  //CURR_TIM->CNT |=  (full_cycle << 0);

  TIM2->CCR2 &= ~(0b1111111111111111 << 0);  // channel 2 is active as long as TIMx_CNT<TIMx_CCR1
  int duty_cycle = duty * pow(2,16);
  TIM2->CCR2 |=  (duty_cycle << 0);
  ////////////////////////////////////

  //////////// FREQUENCY //////////// (based on code from E85)
  double log_scale = log2( (PLL_FREQ + 0.0) / freq); // prevent from losing accuracy in long division
  int best_scale = ceil(log_scale) - 16;            // if freq = 220, this is 0
  if (best_scale < 0) best_scale = 0;
  
  TIM2->ARR &= ~(0b1111111111111111<<0);
  if (freq) {
      uint32_t f_scale = PLL_FREQ / pow(2, best_scale); 
      uint32_t forARR = f_scale / freq; // used to set frequency (?)
      TIM2->ARR |= (forARR<<0);
  }
  ////////////////////////////////////
  return 1;
  */

  /**
  Specific Explanations:

  
  // ???? Make sure that CNT counter goes to CCRy
  // ???? CK_CNT goes into CNT counter
  // ???? PSC Prescaler links CK_PSC to CK_CNT
   // Enable the OCy output:                   TIMx_CCERy
    // Compare TIMx_CCRy <= TIMx_CNT

OC1M:
    // The PWM mode can be selected independently on each channel 
        // (one PWM per OCx output) by writing ‘110’ (PWM mode 1) 
        // or ‘111’ (PWM mode 2) in the OCxM bits in the TIMx_CCMRx 
        // register.
    On page 954: These bits define the behavior of the output reference 
          signal OC1REF from which OC1 and OC1N are derived. OC1REF is 
          active high whereas OC1 and OC1N active level depends on CC1P 
          and CC1NP bits.
          -> 0110: PWM mode 1 - Channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.

OC1PE:
  // You must enable the corresponding preload register by setting the OCxPE bit in the TIMx_CCMRx register
      // x = 1:: TIM15_CCMR1 -> OC1PE [3] must be set: 1 (enabled)
      // PWM can be used without validating preload only in "One Pulse Mode", which I'm not using

EGR:
  // As the preload registers are transferred to the shadow registers only when an update event occurs, 
      // before starting the counter, you have to initialize all the registers by setting the UG bit 
      // in the TIMx_EGR register
      
  (not set atm)
  OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. 
        It can be programmed as active high or active low. 
            * OCx output is enabled by a combination of the 
                    CCxE, CCxNE, MOE, OSSI and OSSR bits (TIMx_CCER and TIMx_BDTR registers). 
            Refer to the TIMx_CCER register description for more details.
            */

/*************************** End of file ****************************/
