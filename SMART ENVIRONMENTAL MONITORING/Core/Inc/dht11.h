/*
 * dht11.h - DHT11 Temperature/Humidity Sensor Driver
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void DHT11_Init(void);
bool DHT11_Read(uint8_t *temperature, uint8_t *humidity);
uint8_t DHT11_GetTemperature(void);
uint8_t DHT11_GetHumidity(void);

#endif /* INC_DHT11_H_ */
