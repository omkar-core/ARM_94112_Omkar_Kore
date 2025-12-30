/**
  ******************************************************************************
  * @file           : mq2.c
  * @brief          : MQ-2 Gas/Smoke Sensor implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mq2.h"
#include "config.h"

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* Private variables ---------------------------------------------------------*/
static uint16_t last_reading = 0;

/* Private function prototypes -----------------------------------------------*/
static uint16_t MQ2_ReadADC(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Read raw ADC value
  * @retval ADC value (0-4095)
  */
static uint16_t MQ2_ReadADC(void)
{
    uint16_t adc_value = 0;

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
        adc_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);

    return adc_value;
}

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize MQ-2 sensor
  * @retval None
  */
void MQ2_Init(void)
{
    /* Take initial reading to stabilize */
    last_reading = MQ2_ReadADC();
    HAL_Delay(10);
    last_reading = MQ2_ReadADC();
}

/**
  * @brief  Read single ADC value from MQ-2
  * @retval ADC value (0-4095)
  */
uint16_t MQ2_Read(void)
{
    last_reading = MQ2_ReadADC();
    return last_reading;
}

/**
  * @brief  Read filtered ADC value (average of multiple samples)
  * @param  samples: Number of samples to average (1-20)
  * @retval Filtered ADC value
  */
uint16_t MQ2_ReadFiltered(uint8_t samples)
{
    uint32_t sum = 0;

    /* Clamp samples to valid range */
    if (samples == 0)
    {
        samples = 1;
    }
    if (samples > 20)
    {
        samples = 20;
    }

    /* Take multiple readings */
    for (uint8_t i = 0; i < samples; i++)
    {
        sum += MQ2_ReadADC();
        HAL_Delay(2);
    }

    /* Calculate average */
    last_reading = (uint16_t)(sum / samples);

    return last_reading;
}

/**
  * @brief  Check if value is above danger threshold
  * @param  value: ADC value to check
  * @retval 1 if above threshold, 0 otherwise
  */
uint8_t MQ2_IsAboveThreshold(uint16_t value)
{
    return (value > MQ2_GAS_THRESHOLD) ? 1 : 0;
}

/**
  * @brief  Check if value is in warning range
  * @param  value: ADC value to check
  * @retval 1 if in warning range, 0 otherwise
  */
uint8_t MQ2_IsWarning(uint16_t value)
{
    return (value > MQ2_GAS_WARNING && value <= MQ2_GAS_THRESHOLD) ? 1 : 0;
}
