/*
 * led.h - LED Driver
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f4xx_hal.h"

void LED_Init(void);
void LED_Green_On(void);
void LED_Green_Off(void);
void LED_Orange_On(void);
void LED_Orange_Off(void);
void LED_Red_On(void);
void LED_Red_Off(void);
void LED_Blue_On(void);
void LED_Blue_Off(void);
void LED_All_On(void);
void LED_All_Off(void);

#endif /* INC_LED_H_ */
