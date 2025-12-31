/*
 * mq2.h - MQ-2 Smoke Sensor Driver
 */

#ifndef INC_MQ2_H_
#define INC_MQ2_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void MQ2_Init(void);
uint16_t MQ2_ReadRaw(void);
uint16_t MQ2_ReadAverage(uint8_t samples);
bool MQ2_IsSmokeDetected(void);

#endif /* INC_MQ2_H_ */
