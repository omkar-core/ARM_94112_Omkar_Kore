/**
  ******************************************************************************
  * @file           : lcd_i2c.c
  * @brief          : I2C LCD (16x2) with PCF8574 expander implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_i2c.h"
#include "config.h"

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Private variables ---------------------------------------------------------*/
static uint8_t backlight_state = LCD_BACKLIGHT;

/* Private function prototypes -----------------------------------------------*/
static void LCD_SendCommand(uint8_t cmd);
static void LCD_SendData(uint8_t data);
static void LCD_SendNibble(uint8_t nibble, uint8_t mode);
static void LCD_Write(uint8_t data);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Write byte to I2C
  * @param  data: Byte to write
  * @retval None
  */
static void LCD_Write(uint8_t data)
{
    uint8_t buffer = data | backlight_state;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS_SHIFTED, &buffer, 1, 100);
}

/**
  * @brief  Send nibble (4 bits) to LCD
  * @param  nibble: 4-bit data (in upper nibble position)
  * @param  mode: 0 for command, LCD_RS for data
  * @retval None
  */
static void LCD_SendNibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = nibble | mode;

    /* Enable pulse */
    LCD_Write(data | LCD_EN);
    HAL_Delay(1);
    LCD_Write(data & ~LCD_EN);
    HAL_Delay(1);
}

/**
  * @brief  Send command to LCD
  * @param  cmd: Command byte
  * @retval None
  */
static void LCD_SendCommand(uint8_t cmd)
{
    /* Send high nibble */
    LCD_SendNibble(cmd & 0xF0, 0);
    /* Send low nibble */
    LCD_SendNibble((cmd << 4) & 0xF0, 0);

    /* Extra delay for clear and home commands */
    if (cmd == LCD_CMD_CLEAR || cmd == LCD_CMD_HOME)
    {
        HAL_Delay(2);
    }
}

/**
  * @brief  Send data to LCD
  * @param  data: Data byte (character)
  * @retval None
  */
static void LCD_SendData(uint8_t data)
{
    /* Send high nibble with RS=1 */
    LCD_SendNibble(data & 0xF0, LCD_RS);
    /* Send low nibble with RS=1 */
    LCD_SendNibble((data << 4) & 0xF0, LCD_RS);
}

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize LCD in 4-bit mode
  * @retval None
  */
void LCD_Init(void)
{
    /* Wait for LCD power-up (>40ms) */
    HAL_Delay(50);

    /* Initial write to ensure known state */
    LCD_Write(0x00);
    HAL_Delay(10);

    /* Initialization sequence for 4-bit mode */
    /* Step 1: Function set (8-bit mode) - 3 times */
    LCD_SendNibble(0x30, 0);
    HAL_Delay(5);
    LCD_SendNibble(0x30, 0);
    HAL_Delay(1);
    LCD_SendNibble(0x30, 0);
    HAL_Delay(1);

    /* Step 2: Set to 4-bit mode */
    LCD_SendNibble(0x20, 0);
    HAL_Delay(1);

    /* Step 3: Function set: 4-bit, 2 lines, 5x8 font */
    LCD_SendCommand(LCD_CMD_FUNCTION_SET_4BIT);

    /* Step 4: Display OFF */
    LCD_SendCommand(LCD_CMD_DISPLAY_OFF);

    /* Step 5: Clear display */
    LCD_SendCommand(LCD_CMD_CLEAR);

    /* Step 6: Entry mode set */
    LCD_SendCommand(LCD_CMD_ENTRY_MODE);

    /* Step 7: Display ON (no cursor, no blink) */
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);

    /* Clear again for good measure */
    LCD_Clear();
}

/**
  * @brief  Clear LCD display
  * @retval None
  */
void LCD_Clear(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
    HAL_Delay(2);
}

/**
  * @brief  Return cursor to home position
  * @retval None
  */
void LCD_Home(void)
{
    LCD_SendCommand(LCD_CMD_HOME);
    HAL_Delay(2);
}

/**
  * @brief  Set cursor position
  * @param  row: Row number (0-1 for 16x2 LCD)
  * @param  col: Column number (0-15 for 16x2 LCD)
  * @retval None
  */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0)
    {
        address = LCD_ROW0_ADDR + col;
    }
    else
    {
        address = LCD_ROW1_ADDR + col;
    }

    LCD_SendCommand(LCD_CMD_SET_DDRAM | address);
}

/**
  * @brief  Turn display ON
  * @retval None
  */
void LCD_DisplayOn(void)
{
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);
}

/**
  * @brief  Turn display OFF
  * @retval None
  */
void LCD_DisplayOff(void)
{
    LCD_SendCommand(LCD_CMD_DISPLAY_OFF);
}

/**
  * @brief  Turn backlight ON
  * @retval None
  */
void LCD_BacklightOn(void)
{
    backlight_state = LCD_BACKLIGHT;
    LCD_Write(0);
}

/**
  * @brief  Turn backlight OFF
  * @retval None
  */
void LCD_BacklightOff(void)
{
    backlight_state = 0;
    LCD_Write(0);
}

/**
  * @brief  Print single character
  * @param  c: Character to print
  * @retval None
  */
void LCD_PrintChar(char c)
{
    LCD_SendData((uint8_t)c);
}

/**
  * @brief  Print string
  * @param  str: Null-terminated string
  * @retval None
  */
void LCD_Print(char *str)
{
    while (*str)
    {
        LCD_SendData((uint8_t)*str);
        str++;
    }
}

/**
  * @brief  Print integer number
  * @param  number: Number to print
  * @retval None
  */
void LCD_PrintNumber(int32_t number)
{
    char buffer[12];
    int i = 0;
    uint8_t is_negative = 0;

    if (number == 0)
    {
        LCD_PrintChar('0');
        return;
    }

    if (number < 0)
    {
        is_negative = 1;
        number = -number;
    }

    while (number > 0)
    {
        buffer[i++] = '0' + (number % 10);
        number /= 10;
    }

    if (is_negative)
    {
        LCD_PrintChar('-');
    }

    while (i > 0)
    {
        LCD_PrintChar(buffer[--i]);
    }
}
