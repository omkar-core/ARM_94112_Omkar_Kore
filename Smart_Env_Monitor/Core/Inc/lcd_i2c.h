/**
  ******************************************************************************
  * @file           : lcd_i2c.h
  * @brief          : Header for I2C LCD (16x2) with PCF8574 expander
  ******************************************************************************
  */

#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/*------------------------------------------------------------------------------
                        LCD COMMAND DEFINITIONS
------------------------------------------------------------------------------*/

/* LCD Commands */
#define LCD_CMD_CLEAR               0x01
#define LCD_CMD_HOME                0x02
#define LCD_CMD_ENTRY_MODE          0x06
#define LCD_CMD_DISPLAY_OFF         0x08
#define LCD_CMD_DISPLAY_ON          0x0C
#define LCD_CMD_DISPLAY_ON_CURSOR   0x0E
#define LCD_CMD_DISPLAY_ON_BLINK    0x0F
#define LCD_CMD_FUNCTION_SET_4BIT   0x28
#define LCD_CMD_FUNCTION_SET_8BIT   0x38
#define LCD_CMD_SET_CGRAM           0x40
#define LCD_CMD_SET_DDRAM           0x80

/* LCD Control Bits (for PCF8574) */
#define LCD_RS                      0x01    /* Register Select */
#define LCD_RW                      0x02    /* Read/Write */
#define LCD_EN                      0x04    /* Enable */
#define LCD_BACKLIGHT               0x08    /* Backlight */

/* Row Addresses */
#define LCD_ROW0_ADDR               0x00
#define LCD_ROW1_ADDR               0x40

/*------------------------------------------------------------------------------
                        FUNCTION PROTOTYPES
------------------------------------------------------------------------------*/

/* Initialization */
void LCD_Init(void);

/* Basic Operations */
void LCD_Clear(void);
void LCD_Home(void);
void LCD_SetCursor(uint8_t row, uint8_t col);

/* Display Control */
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_BacklightOn(void);
void LCD_BacklightOff(void);

/* Print Functions */
void LCD_PrintChar(char c);
void LCD_Print(char *str);
void LCD_PrintNumber(int32_t number);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_I2C_H */
