/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  *                   Smart Environmental Monitoring & Alert System
  ******************************************************************************
  * @attention
  *
  * Hardware Configuration:
  * - MQ-2 Gas Sensor    -> PA0 (ADC1_CH0)
  * - DHT11 Sensor       -> PA1 (GPIO)
  * - I2C LCD            -> PB6 (SCL), PB7 (SDA)
  * - Buzzer             -> PD13 (GPIO Output)
  * - Status LED         -> PD12 (GPIO Output)
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "config.h"
#include "lcd_i2c.h"
#include "mq2.h"
#include "dht11.h"
#include "buzzer.h"
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Peripheral Handles */
ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;

/* Sensor Data */
static uint8_t temperature = 0;
static uint8_t humidity = 0;
static uint16_t gas_value = 0;
static DHT11_Status_t dht_status = DHT11_OK;

/* Timing */
static uint32_t last_sensor_read_time = 0;

/* Display Buffers */
static char lcd_line1[17];
static char lcd_line2[17];

/* Alert State */
static uint8_t alert_active = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);

/* Application Functions */
static void System_Init(void);
static void Read_Sensors(void);
static void Update_Display(void);
static void Check_Alerts(void);
static void Display_Startup_Screen(void);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    HAL_Init();
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();

    /* Initialize Application */
    System_Init();

    /* Display startup screen */
    Display_Startup_Screen();
    HAL_Delay(2000);

    /* Clear display before main loop */
    LCD_Clear();

    /* Initialize timing */
    last_sensor_read_time = HAL_GetTick();

    /* Infinite loop */
    while (1)
    {
        /* Check if it's time to read sensors */
        if ((HAL_GetTick() - last_sensor_read_time) >= SENSOR_READ_INTERVAL_MS)
        {
            last_sensor_read_time = HAL_GetTick();

            /* Read all sensors */
            Read_Sensors();

            /* Check for alert conditions */
            Check_Alerts();

            /* Update LCD display */
            Update_Display();

            /* Toggle status LED to show activity */
            HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
        }

        /* Small delay to prevent CPU hogging */
        HAL_Delay(10);
    }
}

/**
  * @brief  Initialize application components
  * @retval None
  */
static void System_Init(void)
{
    /* Initialize LCD */
    LCD_Init();
    LCD_BacklightOn();

    /* Initialize MQ-2 */
    MQ2_Init();

    /* Initialize DHT11 */
    DHT11_Init();

    /* Initialize Buzzer */
    Buzzer_Init();

    /* Startup beep */
    Buzzer_Beep(100);
}

/**
  * @brief  Display startup screen
  * @retval None
  */
static void Display_Startup_Screen(void)
{
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("  Environment  ");
    LCD_SetCursor(1, 0);
    LCD_Print(" Monitor v1.0  ");
}

/**
  * @brief  Read all sensors
  * @retval None
  */
static void Read_Sensors(void)
{
    /* Read DHT11 */
    dht_status = DHT11_Read(&temperature, &humidity);

    /* Read MQ-2 with filtering */
    gas_value = MQ2_ReadFiltered(5);
}

/**
  * @brief  Update LCD display
  * @retval None
  */
static void Update_Display(void)
{
    /* Line 1: Temperature and Humidity */
    if (dht_status == DHT11_OK)
    {
        snprintf(lcd_line1, sizeof(lcd_line1), "T:%2dC  H:%2d%%   ",
                 temperature, humidity);
    }
    else
    {
        snprintf(lcd_line1, sizeof(lcd_line1), "DHT11 Error!    ");
    }

    LCD_SetCursor(0, 0);
    LCD_Print(lcd_line1);

    /* Line 2: Gas value and status */
    if (alert_active)
    {
        snprintf(lcd_line2, sizeof(lcd_line2), "Gas:%4d ALERT!", gas_value);
    }
    else if (MQ2_IsWarning(gas_value))
    {
        snprintf(lcd_line2, sizeof(lcd_line2), "Gas:%4d WARN  ", gas_value);
    }
    else
    {
        snprintf(lcd_line2, sizeof(lcd_line2), "Gas:%4d OK    ", gas_value);
    }

    LCD_SetCursor(1, 0);
    LCD_Print(lcd_line2);
}

/**
  * @brief  Check sensor values against thresholds
  * @retval None
  */
static void Check_Alerts(void)
{
    uint8_t new_alert_state = 0;

    /* Check gas level */
    if (MQ2_IsAboveThreshold(gas_value))
    {
        new_alert_state = 1;
    }

    /* Check temperature */
    if (temperature > TEMP_THRESHOLD_HIGH)
    {
        new_alert_state = 1;
    }

    /* Check humidity */
    if (humidity > HUMIDITY_THRESHOLD_HIGH)
    {
        new_alert_state = 1;
    }

    /* Handle alert state changes */
    if (new_alert_state && !alert_active)
    {
        /* New alert - turn on buzzer */
        Buzzer_On();
    }
    else if (!new_alert_state && alert_active)
    {
        /* Alert cleared - turn off buzzer and beep success */
        Buzzer_Off();
        HAL_Delay(100);
        Buzzer_Success();
    }

    alert_active = new_alert_state;

    /* Keep buzzer on during active alert */
    if (alert_active)
    {
        Buzzer_On();
    }
}

/**
  * @brief  System Clock Configuration
  *         HSI = 16MHz, SYSCLK = 168MHz
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initialize HSI Oscillator and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Initialize CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  ADC1 Initialization Function
  * @retval None
  */
static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /* Enable ADC Clock */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* Configure ADC1 */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure ADC Channel 0 (PA0) */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  I2C1 Initialization Function
  * @retval None
  */
static void MX_I2C1_Init(void)
{
    /* Enable I2C Clock */
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO Clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*--------------------------------------------------------------------------
                            CONFIGURE OUTPUT PINS
    --------------------------------------------------------------------------*/

    /* Configure Buzzer Pin (PD13) as Output */
    GPIO_InitStruct.Pin = BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);

    /* Configure Status LED Pin (PD12) as Output */
    GPIO_InitStruct.Pin = STATUS_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(STATUS_LED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(STATUS_LED_PORT, STATUS_LED_PIN, GPIO_PIN_RESET);

    /*--------------------------------------------------------------------------
                            CONFIGURE DHT11 PIN
    --------------------------------------------------------------------------*/

    /* Configure DHT11 Pin (PA1) as Open-Drain Output */
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);

    /*--------------------------------------------------------------------------
                            CONFIGURE ADC PIN
    --------------------------------------------------------------------------*/

    /* Configure ADC Pin (PA0) as Analog */
    GPIO_InitStruct.Pin = MQ2_ADC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MQ2_ADC_PORT, &GPIO_InitStruct);

    /*--------------------------------------------------------------------------
                            CONFIGURE I2C PINS
    --------------------------------------------------------------------------*/

    /* Configure I2C Pins (PB6, PB7) */
    GPIO_InitStruct.Pin = LCD_I2C_SCL_PIN | LCD_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();

    /* Blink LED rapidly to indicate error */
    while (1)
    {
        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
        /* Simple delay without HAL */
        for(volatile uint32_t i = 0; i < 500000; i++);
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation */
}
#endif
