/*
 * ext_intr.c
 *
 *  Created on: Dec 18, 2025
 *      Author: kiran_z6dopa8
 */


#include "my_led.h"

/* Bit value macro */
#define BV(x) (1U << (x))

void extint_init(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB1ENR |= BV(0);

    /* 2. Configure PA0 as input */
    GPIOA->MODER &= ~(BV(0) | BV(1));

    /* 3. Disable pull-up / pull-down (external pull-down present) */
    GPIOA->PUPDR &= ~(BV(0) | BV(1));

    /* 4. Enable SYSCFG clock */
    RCC->APB2ENR |= BV(14);

    /* 5. Map PA0 to EXTI0 */
    SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

    /* 6. Unmask EXTI0 interrupt */
    EXTI->IMR |= BV(0);

    /* 7. Configure EXTI0 for SWITCH RELEASE */
    /* Disable falling edge (press) */
    EXTI->FTSR &= ~BV(0);

    /* Enable rising edge (release) */
    EXTI->RTSR |= BV(0);

    /* 8. Clear pending interrupt */
    EXTI->PR |= BV(0);

    /* 9. Enable EXTI0 interrupt in NVIC */
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    /* Clear EXTI0 pending flag */
    EXTI->PR |= BV(0);

    /* Action on switch RELEASE */
    led_toggle(14);
}
