/*
 * lcd.h - I2C LCD 16x2 Driver
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/* LCD Commands */
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_FUNCTIONSET         0x20
#define LCD_SETDDRAMADDR        0x80

/* Entry Mode Flags */
#define LCD_ENTRY_ID            0x02

/* Display Control Flags */
#define LCD_DISPLAY_D           0x04

/* Function Set Flags */
#define LCD_FUNCTION_N          0x08

/* PCF8574 Control Bits */
#define LCD_RS                  (1 << 0)
#define LCD_EN                  (1 << 2)
#define LCD_BK_LIGHT            (1 << 3)

/* Function Prototypes */
bool lcd16x2_i2c_init(I2C_HandleTypeDef *pI2cHandle);
void lcd16x2_i2c_sendCommand(uint8_t command);
void lcd16x2_i2c_sendData(uint8_t data);
void lcd16x2_i2c_setCursor(uint8_t row, uint8_t col);
void lcd16x2_i2c_clear(void);
void lcd16x2_i2c_printf(const char *str);

#endif /* INC_LCD_H_ */
