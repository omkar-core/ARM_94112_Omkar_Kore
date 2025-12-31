/*
 * led.c - LED Driver
 */

#include "led.h"
#include "main.h"

void LED_Init(void) { LED_All_Off(); }
void LED_Green_On(void) { HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN, GPIO_PIN_SET); }
void LED_Green_Off(void) { HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN, GPIO_PIN_RESET); }
void LED_Orange_On(void) { HAL_GPIO_WritePin(LED_PORT, LED_ORANGE_PIN, GPIO_PIN_SET); }
void LED_Orange_Off(void) { HAL_GPIO_WritePin(LED_PORT, LED_ORANGE_PIN, GPIO_PIN_RESET); }
void LED_Red_On(void) { HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, GPIO_PIN_SET); }
void LED_Red_Off(void) { HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, GPIO_PIN_RESET); }
void LED_Blue_On(void) { HAL_GPIO_WritePin(LED_PORT, LED_BLUE_PIN, GPIO_PIN_SET); }
void LED_Blue_Off(void) { HAL_GPIO_WritePin(LED_PORT, LED_BLUE_PIN, GPIO_PIN_RESET); }

void LED_All_On(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN, GPIO_PIN_SET);
}

void LED_All_Off(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN, GPIO_PIN_RESET);
}
