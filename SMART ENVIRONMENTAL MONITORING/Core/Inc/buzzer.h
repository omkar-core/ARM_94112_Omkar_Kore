/*
 * buzzer.h - Buzzer Driver
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);
void Buzzer_Beep(uint32_t duration_ms);
void Buzzer_Alert(void);

#endif /* INC_BUZZER_H_ */
