/*
 * gpio.h
 *
 *  Created on: Dec 17, 2025
 *      Author: Omkar
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void GPIO_Init(void);
uint8_t Read_Switch(void);
void Green_LED_On(void);
void Blue_LED_On(void);
void LEDs_Off(void);
void Delay_ms(uint32_t ms);

#endif
