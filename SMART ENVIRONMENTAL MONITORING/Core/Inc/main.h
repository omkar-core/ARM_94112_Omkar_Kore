/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* ==================== THRESHOLD VALUES ==================== */
#define SMOKE_THRESHOLD             1000
#define TEMP_HIGH_THRESHOLD         40
#define TEMP_LOW_THRESHOLD          10
#define HUMIDITY_HIGH_THRESHOLD     80
#define HUMIDITY_LOW_THRESHOLD      20

/* ==================== LED PINS ==================== */
#define LED_GREEN_PIN               GPIO_PIN_12
#define LED_ORANGE_PIN              GPIO_PIN_13
#define LED_RED_PIN                 GPIO_PIN_14
#define LED_BLUE_PIN                GPIO_PIN_15
#define LED_PORT                    GPIOD

/* ==================== BUZZER PIN ==================== */
#define BUZZER_PIN                  GPIO_PIN_0
#define BUZZER_PORT                 GPIOB

/* ==================== DHT11 PIN ==================== */
#define DHT11_PIN                   GPIO_PIN_5
#define DHT11_PORT                  GPIOA

/* ==================== LCD I2C ADDRESS ==================== */
#define LCD_I2C_SLAVE_ADDRESS_0     0x4E
#define LCD_I2C_SLAVE_ADDRESS_1     0x7E

/* ==================== BUFFER SIZE ==================== */
#define UART_BUFFER_SIZE            128

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint32_t us);
void UART_SendString(const char *str);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
