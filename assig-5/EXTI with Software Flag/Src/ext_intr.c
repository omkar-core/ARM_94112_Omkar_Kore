/*
 * ext_intr.c
 *
 *  Created on: Dec 18, 2025
 *      Author: kiran_z6dopa8
 */


#include "my_led.h"

#define BV(x) (1U << (x))

/*
 * External declaration of the flag
 * Defined in main.c
 */
extern volatile uint8_t exti0_flag;

void extint_init(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB1ENR |= BV(0);

    /* 2. Configure PA0 as input */
    GPIOA->MODER &= ~(BV(0) | BV(1));

    /* 3. Disable pull-up / pull-down */
    GPIOA->PUPDR &= ~(BV(0) | BV(1));

    /* 4. Enable SYSCFG clock */
    RCC->APB2ENR |= BV(14);

    /* 5. Map PA0 to EXTI0 */
    SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

    /* 6. Unmask EXTI0 interrupt */
    EXTI->IMR |= BV(0);

    /* 7. Trigger on rising edge (switch RELEASE) */
    EXTI->FTSR &= ~BV(0);   // Disable falling edge
    EXTI->RTSR |=  BV(0);   // Enable rising edge

    /* 8. Clear pending flag */
    EXTI->PR |= BV(0);

    /* 9. Enable EXTI0 interrupt in NVIC */
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/*
 * EXTI0 Interrupt Service Routine
 * NOTE:
 * - ISR does NOT toggle LED
 * - ISR only sets a software flag
 */
void EXTI0_IRQHandler(void)
{
    /* Clear EXTI0 pending bit */
    EXTI->PR |= BV(0);

    /* Set software flag */
    exti0_flag = 1;
}
