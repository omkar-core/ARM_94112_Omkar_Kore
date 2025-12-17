/*
 * gpio.c
 *
 *  Created on: Dec 17, 2025
 *      Author: Omkar
 */
#include "gpio.h"

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)

#define GPIOA_MODER  (*(volatile uint32_t*)0x40020000)
#define GPIOA_IDR    (*(volatile uint32_t*)0x40020010)

#define GPIOD_MODER  (*(volatile uint32_t*)0x40020C00)
#define GPIOD_ODR    (*(volatile uint32_t*)0x40020C14)

#define GPIOA_EN     (1 << 0)
#define GPIOD_EN     (1 << 3)

#define LED_PIN      12
#define SWITCH_PIN   0

void GPIO_Init(void)
{

    RCC_AHB1ENR |= GPIOA_EN;
    RCC_AHB1ENR |= GPIOD_EN;


    GPIOA_MODER &= ~(3 << (SWITCH_PIN * 2));

    GPIOD_MODER &= ~(3 << (LED_PIN * 2));
    GPIOD_MODER |=  (1 << (LED_PIN * 2));
}

uint8_t Read_Switch(void)
{
    return (GPIOA_IDR & (1 << SWITCH_PIN)) ? 1 : 0;
}

void LED_On(void)
{
    GPIOD_ODR |= (1 << LED_PIN);
}

void LED_Off(void)
{
    GPIOD_ODR &= ~(1 << LED_PIN);
}


