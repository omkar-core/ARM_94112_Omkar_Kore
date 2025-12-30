/**
  ******************************************************************************
  * @file           : dht11.h
  * @brief          : Header for DHT11 Temperature & Humidity Sensor
  ******************************************************************************
  */

#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/*------------------------------------------------------------------------------
                        TYPE DEFINITIONS
------------------------------------------------------------------------------*/

/* DHT11 Status Codes */
typedef enum {
    DHT11_OK = 0,
    DHT11_ERROR_TIMEOUT,
    DHT11_ERROR_CHECKSUM,
    DHT11_ERROR_NO_RESPONSE
} DHT11_Status_t;

/*------------------------------------------------------------------------------
                        FUNCTION PROTOTYPES
------------------------------------------------------------------------------*/

/* Initialization */
void DHT11_Init(void);

/* Basic Reading */
DHT11_Status_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);

/* Individual Readings */
uint8_t DHT11_GetTemperature(void);
uint8_t DHT11_GetHumidity(void);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */
