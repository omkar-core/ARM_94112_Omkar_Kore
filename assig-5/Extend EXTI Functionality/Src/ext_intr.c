/*
 * ext_intr.c
 *
 *  Created on: Dec 18, 2025
 *      Author: kiran_z6dopa8
 */


#include "my_led.h"

#define BV(x) (1U << (x))

/* Extern declarations */
extern volatile uint8_t exti_event_flag;

void extint_init(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= BV(0);

    /* Configure PA0 as input */
    GPIOA->MODER &= ~(BV(0) | BV(1));

    /* Disable pull-up / pull-down */
    GPIOA->PUPDR &= ~(BV(0) | BV(1));

    /* Enable SYSCFG clock */
    RCC->APB2ENR |= BV(14);

    /* Map PA0 to EXTI0 */
    SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

    /* Unmask EXTI0 */
    EXTI->IMR |= BV(0);

    /* Trigger on rising edge (button release) */
    EXTI->FTSR &= ~BV(0);
    EXTI->RTSR |=  BV(0);

    /* Clear pending bit */
    EXTI->PR |= BV(0);

    /* Enable EXTI0 interrupt */
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/*
 * EXTI0 Interrupt Service Routine
 * - Short
 * - Non-blocking
 * - No LED control
 */
void EXTI0_IRQHandler(void)
{
    /* Clear EXTI pending flag */
    EXTI->PR |= BV(0);

    /* Notify main() that event occurred */
    exti_event_flag = 1;
}
