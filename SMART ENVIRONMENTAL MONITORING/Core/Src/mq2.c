/*
 * mq2.c - MQ-2 Smoke Sensor Driver
 */

#include "mq2.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

void MQ2_Init(void)
{
    HAL_Delay(100);
}

uint16_t MQ2_ReadRaw(void)
{
    uint16_t val = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return val;
}

uint16_t MQ2_ReadAverage(uint8_t samples)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++)
    {
        sum += MQ2_ReadRaw();
        HAL_Delay(5);
    }
    return (uint16_t)(sum / samples);
}

bool MQ2_IsSmokeDetected(void)
{
    return (MQ2_ReadAverage(5) > SMOKE_THRESHOLD);
}
