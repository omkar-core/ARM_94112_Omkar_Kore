/*
 * lcd.c - I2C LCD Driver
 */

#include "lcd.h"
#include "main.h"

static I2C_HandleTypeDef *lcd_i2c = NULL;
static uint8_t lcd_addr = 0;

static HAL_StatusTypeDef lcd_send_nibble(uint8_t nibble)
{
    uint8_t data[2];
    data[0] = nibble | LCD_EN;
    data[1] = nibble;
    return HAL_I2C_Master_Transmit(lcd_i2c, lcd_addr, data, 2, 100);
}

void lcd16x2_i2c_sendCommand(uint8_t cmd)
{
    uint8_t high = cmd & 0xF0;
    uint8_t low = (cmd << 4) & 0xF0;
    lcd_send_nibble(high | LCD_BK_LIGHT);
    lcd_send_nibble(low | LCD_BK_LIGHT);
}

void lcd16x2_i2c_sendData(uint8_t data)
{
    uint8_t high = data & 0xF0;
    uint8_t low = (data << 4) & 0xF0;
    lcd_send_nibble(high | LCD_BK_LIGHT | LCD_RS);
    lcd_send_nibble(low | LCD_BK_LIGHT | LCD_RS);
}

bool lcd16x2_i2c_init(I2C_HandleTypeDef *hi2c)
{
    lcd_i2c = hi2c;
    HAL_Delay(50);

    if (HAL_I2C_IsDeviceReady(lcd_i2c, LCD_I2C_SLAVE_ADDRESS_0, 3, 100) == HAL_OK)
        lcd_addr = LCD_I2C_SLAVE_ADDRESS_0;
    else if (HAL_I2C_IsDeviceReady(lcd_i2c, LCD_I2C_SLAVE_ADDRESS_1, 3, 100) == HAL_OK)
        lcd_addr = LCD_I2C_SLAVE_ADDRESS_1;
    else
        return false;

    HAL_Delay(45);
    lcd16x2_i2c_sendCommand(0x30); HAL_Delay(5);
    lcd16x2_i2c_sendCommand(0x30); HAL_Delay(1);
    lcd16x2_i2c_sendCommand(0x30); HAL_Delay(1);
    lcd16x2_i2c_sendCommand(0x20); HAL_Delay(1);
    lcd16x2_i2c_sendCommand(LCD_FUNCTIONSET | LCD_FUNCTION_N);
    lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL);
    lcd16x2_i2c_sendCommand(LCD_CLEARDISPLAY); HAL_Delay(2);
    lcd16x2_i2c_sendCommand(LCD_ENTRYMODESET | LCD_ENTRY_ID);
    lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_D);

    return true;
}

void lcd16x2_i2c_setCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd16x2_i2c_sendCommand(addr);
}

void lcd16x2_i2c_clear(void)
{
    lcd16x2_i2c_sendCommand(LCD_CLEARDISPLAY);
    HAL_Delay(2);
}

void lcd16x2_i2c_printf(const char *str)
{
    while (*str) lcd16x2_i2c_sendData(*str++);
}
