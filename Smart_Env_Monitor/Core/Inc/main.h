/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file
  *                   Smart Environmental Monitoring System
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/

/*------------------------------------------------------------------------------
                        PIN DEFINITIONS
------------------------------------------------------------------------------*/

/* MQ-2 Gas Sensor (ADC Input) */
#define MQ2_ADC_PIN                 GPIO_PIN_0
#define MQ2_ADC_PORT                GPIOA

/* DHT11 Temperature & Humidity Sensor (GPIO) */
#define DHT11_PIN                   GPIO_PIN_1
#define DHT11_PORT                  GPIOA

/* I2C LCD (I2C1) */
#define LCD_I2C_SCL_PIN             GPIO_PIN_6
#define LCD_I2C_SCL_PORT            GPIOB
#define LCD_I2C_SDA_PIN             GPIO_PIN_7
#define LCD_I2C_SDA_PORT            GPIOB

/* Buzzer (GPIO Output) */
#define BUZZER_PIN                  GPIO_PIN_13
#define BUZZER_PORT                 GPIOD

/* Status LED (Optional - Onboard LED) */
#define STATUS_LED_PIN              GPIO_PIN_12
#define STATUS_LED_PORT             GPIOD

/*------------------------------------------------------------------------------
                        PERIPHERAL HANDLES (extern)
------------------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
