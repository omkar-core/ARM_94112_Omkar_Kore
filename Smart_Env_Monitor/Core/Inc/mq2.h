/**
  ******************************************************************************
  * @file           : mq2.h
  * @brief          : Header for MQ-2 Gas/Smoke Sensor (ADC based)
  ******************************************************************************
  */

#ifndef __MQ2_H
#define __MQ2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/*------------------------------------------------------------------------------
                        FUNCTION PROTOTYPES
------------------------------------------------------------------------------*/

/* Initialization */
void MQ2_Init(void);

/* Basic Reading */
uint16_t MQ2_Read(void);
uint16_t MQ2_ReadFiltered(uint8_t samples);

/* Status Check */
uint8_t MQ2_IsAboveThreshold(uint16_t value);
uint8_t MQ2_IsWarning(uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* __MQ2_H */
