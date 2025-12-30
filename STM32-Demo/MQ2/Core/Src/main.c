/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : MQ-2 Gas Sensor User Controlled via UART2
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t adc_raw;
float voltage;
float gas_ppm;
char txBuf[200];
char rxBuf[20];
uint8_t measure_enable = 0;
/* USER CODE END PV */

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);

void UART_Print_Menu(void);
void MQ2_Read_And_Display(void);
void ToUpperCase(char *str);

/* ========================= MAIN ========================= */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();

  UART_Print_Menu();

  while (1)
  {
    /* Check UART input (Non-blocking) */
    if (HAL_UART_Receive(&huart2, (uint8_t *)rxBuf, sizeof(rxBuf),
                         100) == HAL_OK)
    {
      ToUpperCase(rxBuf);

      if (!strcmp(rxBuf, "1") || !strcmp(rxBuf, "ON"))
      {
        measure_enable = 1;
        sprintf(txBuf, "\r\n[INFO] MQ-2 Measurement STARTED\r\n");
      }
      else if (!strcmp(rxBuf, "0") || !strcmp(rxBuf, "OFF"))
      {
        measure_enable = 0;
        sprintf(txBuf, "\r\n[INFO] MQ-2 Measurement STOPPED\r\n");
      }
      else if (!strcmp(rxBuf, "R"))
      {
        MQ2_Read_And_Display();
        rxBuf[0] = '\0';
        continue;
      }
      else if (!strcmp(rxBuf, "H"))
      {
        UART_Print_Menu();
        rxBuf[0] = '\0';
        continue;
      }
      else
      {
        sprintf(txBuf, "\r\n[ERROR] Unknown Command. Press H for help\r\n");
      }

      HAL_UART_Transmit(&huart2, (uint8_t *)txBuf, strlen(txBuf), HAL_MAX_DELAY);
      rxBuf[0] = '\0';
    }

    /* Continuous Measurement Mode */
    if (measure_enable)
    {
      MQ2_Read_And_Display();
      HAL_Delay(1000);
    }
  }
}

/* ================= MQ-2 READ FUNCTION ================= */
void MQ2_Read_And_Display(void)
{
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  adc_raw = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  voltage = (adc_raw * 3.3f) / 4095.0f;
  gas_ppm = (voltage / 3.3f) * 1000.0f;

  sprintf(txBuf,
          "ADC: %lu | Voltage: %.2f V | Gas: %.0f PPM | %s\r\n",
          adc_raw,
          voltage,
          gas_ppm,
          (gas_ppm > 300) ? "⚠ GAS ALERT" : "SAFE");

  HAL_UART_Transmit(&huart2, (uint8_t *)txBuf, strlen(txBuf), HAL_MAX_DELAY);
}

/* ================= UART MENU ================= */
void UART_Print_Menu(void)
{
  sprintf(txBuf,
          "\r\n=======================================\r\n"
          "   MQ-2 GAS SENSOR - UART CONTROLLED   \r\n"
          "=======================================\r\n"
          "Commands:\r\n"
          "  1  or ON   -> Start Measurement\r\n"
          "  0  or OFF  -> Stop Measurement\r\n"
          "  R          -> Read Once\r\n"
          "  H          -> Help Menu\r\n"
          "---------------------------------------\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)txBuf, strlen(txBuf), HAL_MAX_DELAY);
}

/* ================= STRING UTILITY ================= */
void ToUpperCase(char *str)
{
  while (*str)
  {
    *str = toupper((unsigned char)*str);
    str++;
  }
}

/* ================= PERIPHERALS ================= */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  HAL_UART_Init(&huart2);
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* ================= CLOCK ================= */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                               RCC_CLOCKTYPE_SYSCLK |
                               RCC_CLOCKTYPE_PCLK1 |
                               RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

/* ================= ERROR ================= */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
