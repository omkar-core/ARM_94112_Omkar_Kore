/**
  ******************************************************************************
  * @file           : buzzer.h
  * @brief          : Header for Buzzer control
  ******************************************************************************
  */

#ifndef __BUZZER_H
#define __BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/*------------------------------------------------------------------------------
                        FUNCTION PROTOTYPES
------------------------------------------------------------------------------*/

/* Initialization */
void Buzzer_Init(void);

/* Basic Control */
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);

/* Timed Beeps */
void Buzzer_Beep(uint16_t duration_ms);

/* Patterns */
void Buzzer_Alarm(uint8_t count);
void Buzzer_Warning(void);
void Buzzer_Success(void);

#ifdef __cplusplus
}
#endif

#endif /* __BUZZER_H */
