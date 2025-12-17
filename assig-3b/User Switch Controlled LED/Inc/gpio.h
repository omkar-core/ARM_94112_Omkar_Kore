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
void LED_On(void);
void LED_Off(void);

#endif
