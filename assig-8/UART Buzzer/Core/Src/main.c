/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.c
  * @brief   UART2 Buzzer Control with Menu (STM32F407)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
uint8_t rx_byte;

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

void Show_Menu(void);
void Buzzer_Action(uint8_t cmd);
void UART_Send(char *msg);

/* Main program --------------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  Show_Menu();

  while (1)
  {
    HAL_UART_Receive(&huart2, &rx_byte, 1, HAL_MAX_DELAY);

    /* Echo key */
    HAL_UART_Transmit(&huart2, &rx_byte, 1, HAL_MAX_DELAY);

    Buzzer_Action(rx_byte);
  }
}

/* UART Send String ----------------------------------------------------------*/
void UART_Send(char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

/* Display Menu --------------------------------------------------------------*/
void Show_Menu(void)
{
  UART_Send("\r\n========================================\r\n");
  UART_Send("        UART BUZZER CONTROL MENU        \r\n");
  UART_Send("========================================\r\n");
  UART_Send("Press 1 : Small Beep\r\n");
  UART_Send("Press 2 : Medium Beep\r\n");
  UART_Send("Press 3 : Fast Beep\r\n");
  UART_Send("Press 4 : Double Beep\r\n");
  UART_Send("Press 0 : Buzzer OFF\r\n");
  UART_Send("Press M : Show Menu Again\r\n");
  UART_Send("========================================\r\n");
  UART_Send("Enter choice: ");
}

/* Buzzer Control Logic ------------------------------------------------------*/
void Buzzer_Action(uint8_t cmd)
{
  switch (cmd)
  {
    case '1':   // Small Beep
      UART_Send("\r\n>> Small Beep\r\n");
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
      HAL_Delay(200);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
      HAL_Delay(200);
      break;

    case '2':   // Medium Beep
      UART_Send("\r\n>> Medium Beep\r\n");
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
      HAL_Delay(500);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
      HAL_Delay(500);
      break;

    case '3':   // Fast Beep
      UART_Send("\r\n>> Fast Beep\r\n");
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
      HAL_Delay(100);
      break;

    case '4':   // Double Beep
      UART_Send("\r\n>> Double Beep\r\n");
      for (int i = 0; i < 2; i++)
      {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_Delay(200);
      }
      break;

    case '0':   // OFF
      UART_Send("\r\n>> Buzzer OFF\r\n");
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
      break;

    case 'M':
    case 'm':
      Show_Menu();
      break;

    default:
      UART_Send("\r\n>> Invalid Key! Press M for Menu\r\n");
      break;
  }

  UART_Send("Enter choice: ");
}

/* USART2 Initialization -----------------------------------------------------*/
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
    Error_Handler();
}

/* GPIO Initialization -------------------------------------------------------*/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* PA2 -> TX, PA3 -> RX */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PD12 -> Buzzer */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* System Clock Configuration ------------------------------------------------*/
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
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                               RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

/* Error Handler -------------------------------------------------------------*/
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
