/**
  ******************************************************************************
  * @file           : buzzer.c
  * @brief          : Buzzer control implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "buzzer.h"
#include "main.h"
#include "config.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t buzzer_state = 0;

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize buzzer
  * @retval None
  */
void Buzzer_Init(void)
{
    /* Ensure buzzer is OFF */
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    buzzer_state = 0;
}

/**
  * @brief  Turn buzzer ON
  * @retval None
  */
void Buzzer_On(void)
{
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
    buzzer_state = 1;
}

/**
  * @brief  Turn buzzer OFF
  * @retval None
  */
void Buzzer_Off(void)
{
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    buzzer_state = 0;
}

/**
  * @brief  Toggle buzzer state
  * @retval None
  */
void Buzzer_Toggle(void)
{
    if (buzzer_state)
    {
        Buzzer_Off();
    }
    else
    {
        Buzzer_On();
    }
}

/**
  * @brief  Beep for specified duration (blocking)
  * @param  duration_ms: Duration in milliseconds
  * @retval None
  */
void Buzzer_Beep(uint16_t duration_ms)
{
    Buzzer_On();
    HAL_Delay(duration_ms);
    Buzzer_Off();
}

/**
  * @brief  Play alarm pattern (multiple beeps)
  * @param  count: Number of beeps
  * @retval None
  */
void Buzzer_Alarm(uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        Buzzer_Beep(BUZZER_ALARM_ON);
        HAL_Delay(BUZZER_ALARM_OFF);
    }
}

/**
  * @brief  Play warning pattern (2 short beeps)
  * @retval None
  */
void Buzzer_Warning(void)
{
    Buzzer_Beep(BUZZER_SHORT_BEEP);
    HAL_Delay(100);
    Buzzer_Beep(BUZZER_SHORT_BEEP);
}

/**
  * @brief  Play success pattern (1 long beep)
  * @retval None
  */
void Buzzer_Success(void)
{
    Buzzer_Beep(BUZZER_LONG_BEEP);
}
