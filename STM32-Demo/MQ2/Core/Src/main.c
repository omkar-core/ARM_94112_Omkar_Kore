/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : MQ-2 Gas Sensor ADC with UART User Control
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char txBuf[150];
char rxBuf[10];

uint32_t ADC_val;
float voltage;
float gas_ppm;

uint8_t measure_enable = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
void MQ2_Read_Display(void);
void Print_Menu(void);
/* USER CODE END PFP */

/* ========================== MAIN ========================== */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();

  Print_Menu();

  while (1)
  {
    /* ---------- UART COMMAND RECEIVE ---------- */
    if (HAL_UART_Receive(&huart2, (uint8_t *)rxBuf, sizeof(rxBuf),
                         100) == HAL_OK)
    {
      rxBuf[strcspn(rxBuf, "\r\n")] = 0;   // remove enter
      for (int i = 0; rxBuf[i]; i++) rxBuf[i] = toupper(rxBuf[i]);

      if (!strcmp(rxBuf, "1"))
      {
        measure_enable = 1;
        sprintf(txBuf, "\r\n[INFO] MQ-2 Measurement STARTED\r\n");
      }
      else if (!strcmp(rxBuf, "0"))
      {
        measure_enable = 0;
        sprintf(txBuf, "\r\n[INFO] MQ-2 Measurement STOPPED\r\n");
      }
      else if (!strcmp(rxBuf, "R"))
      {
        MQ2_Read_Display();
        rxBuf[0] = 0;
        continue;
      }
      else if (!strcmp(rxBuf, "H"))
      {
        Print_Menu();
        rxBuf[0] = 0;
        continue;
      }
      else
      {
        sprintf(txBuf, "\r\n[ERROR] Invalid Command (Press H)\r\n");
      }

      HAL_UART_Transmit(&huart2, (uint8_t *)txBuf,
                        strlen(txBuf), HAL_MAX_DELAY);
      rxBuf[0] = 0;
    }

    /* ---------- CONTINUOUS MEASUREMENT ---------- */
    if (measure_enable)
    {
      MQ2_Read_Display();
      HAL_Delay(1000);
    }
  }
}

/* ================= MQ-2 READ FUNCTION ================= */
void MQ2_Read_Display(void)
{
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  ADC_val = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  voltage = (ADC_val * 3.3f) / 4095.0f;
  gas_ppm = (voltage / 3.3f) * 1000.0f;   // Approximate

  sprintf(txBuf,
          "ADC: %lu | Voltage: %.2f V | Gas: %.0f PPM | %s\r\n",
          ADC_val,
          voltage,
          gas_ppm,
          (gas_ppm > 300) ? "GAS DETECTED" : "SAFE");

  HAL_UART_Transmit(&huart2, (uint8_t *)txBuf,
                    strlen(txBuf), HAL_MAX_DELAY);
}

/* ================= UART MENU ================= */
void Print_Menu(void)
{
  sprintf(txBuf,
          "\r\n====================================\r\n"
          "   MQ-2 GAS SENSOR (UART CONTROL)   \r\n"
          "====================================\r\n"
          "1  -> Start Measurement\r\n"
          "0  -> Stop Measurement\r\n"
          "R  -> Read Once\r\n"
          "H  -> Help Menu\r\n"
          "------------------------------------\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)txBuf,
                    strlen(txBuf), HAL_MAX_DELAY);
}

/* ================= ADC INIT ================= */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

  sConfig.Channel = ADC_CHANNEL_0;   // PA0
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/* ================= UART INIT ================= */
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

/* ================= GPIO ================= */
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                               RCC_CLOCKTYPE_SYSCLK |
                               RCC_CLOCKTYPE_PCLK1 |
                               RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/* ================= ERROR ================= */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
