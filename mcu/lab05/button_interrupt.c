// button_interrupt.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22

#include "lib/main.h"
#include "stm32l432xx.h"

int main(void) {
    // Enable LED as output
    gpioEnable(GPIO_PORT_B);
    pinMode(LED_PIN, GPIO_OUTPUT);

    // Enable button as input
    gpioEnable(GPIO_PORT_A);
    pinMode(BUTTON_PIN, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD5, 0b01); // Set PA5 as pull-up

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    ////////////////////////////////////////////////////////////////////
    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    // RCC_APB2ENR, bit 0, is SYSCFGEN @ bit 0
    RCC->APB2ENR |= (1<<0);

    // 2. Configure EXTICR for the input button interrupt (PA1)
    // *** double check the syscfg pointer

    //not zero indexed
    SYSCFG->EXTICR[1] &= ~_VAL2FLD(SYSCFG_EXTICR2_EXTI5, 0b111); // Figure 29

    // Enable interrupts globally
    __enable_irq();
    ////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////
    // TODO: Configure interrupt for falling edge of GPIO pin for button

    /**
    *
    To configure a line as an interrupt source, use the following procedure:
1. Configure the corresponding mask bit in the EXTI_IMR register.
2. Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR).
3. Configure the enable and mask bits that control the NVIC IRQ channel mapped to the 
    EXTI so that an interrupt coming from one of the EXTI lines can be correctly acknowledged.
    */

    // 1. Configure mask bit for pin PA4 -- need to set it for EXTI2 ** changed to pin PA5 for now
        // EXTI: GPIO lines are bits 0-15, and are configurable
    EXTI->IMR1 |= _VAL2FLD(EXTI_IMR1_IM5, 0b1); // 1: Interrupt request from Line 4 is not masked
    
    // 2. Disable rising edge trigger
    EXTI->RTSR1 &= ~_VAL2FLD(EXTI_RTSR1_RT5, 0b1); // 0: Rising trigger disabled (for Event and Interrupt) for input line 4

    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= _VAL2FLD(EXTI_FTSR1_FT5, 0b1); // 1: Falling trigger enabled (for Event and Interrupt) for input line 4

    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn); // based of of table 46 in the user guide (https://hmc-e155.github.io/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), whould be 6 for systick
    // EXTI9_5_IRQn -- enables interrupt pins 9-5

    ////////////////////////////////////////////////////////////////////

    while(1){   
        delay_millis(TIM2, 2);
    }

}

/**
*  What to do when handling an interrupt from the input pin (i.e. pin PA5, since that's one of the only ones I can control on my MCU)
*/
void EXTI9_5_IRQHandler (void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 5)) {
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << _FLD2VAL(EXTI_PR1_PIF5, 1));

        // Then toggle the LED
        togglePin(LED_PIN);

    }
}