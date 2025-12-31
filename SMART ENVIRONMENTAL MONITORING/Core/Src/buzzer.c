/*
 * buzzer.c - Buzzer Driver
 */

#include "buzzer.h"
#include "main.h"

void Buzzer_Init(void) { Buzzer_Off(); }
void Buzzer_On(void) { HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET); }
void Buzzer_Off(void) { HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET); }
void Buzzer_Toggle(void) { HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN); }

void Buzzer_Beep(uint32_t ms)
{
    Buzzer_On();
    HAL_Delay(ms);
    Buzzer_Off();
}

void Buzzer_Alert(void)
{
    static uint32_t last = 0;
    if (HAL_GetTick() - last >= 100)
    {
        Buzzer_Toggle();
        last = HAL_GetTick();
    }
}
