/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Advanced LED Control System with Multiple Patterns
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LEN 100

/* LED Pin Definitions - STM32F4 Discovery */
#define LED_GREEN_PIN     GPIO_PIN_12   // PD12
#define LED_ORANGE_PIN    GPIO_PIN_13   // PD13
#define LED_RED_PIN       GPIO_PIN_14   // PD14
#define LED_BLUE_PIN      GPIO_PIN_15   // PD15
#define LED_ALL_PINS      (LED_GREEN_PIN | LED_ORANGE_PIN | LED_RED_PIN | LED_BLUE_PIN)
#define LED_PORT          GPIOD

/* Blink delay in milliseconds */
#define BLINK_DELAY       200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_byte;              // Single received byte
char rx_buffer[MAX_LEN];      // String buffer to store received characters
uint16_t rx_index = 0;        // Current index in buffer
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Process_Command(char *cmd);
void Display_Menu(void);
void Blink_All_LEDs(uint8_t count);
void Blink_Single_LED(uint16_t pin, uint8_t count);
void Running_LED_Pattern(uint8_t count);
void Alternate_LEDs_Pattern(uint8_t count);
void Knight_Rider_Pattern(uint8_t count);
void Binary_Count_Pattern(void);
void Random_Blink_Pattern(uint8_t count);
void All_LEDs_ON(void);
void All_LEDs_OFF(void);
void Send_String(char *str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Send string via UART
  * @param  str: Pointer to string
  * @retval None
  */
void Send_String(char *str)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

/**
  * @brief  Display menu options
  * @retval None
  */
void Display_Menu(void)
{
    Send_String("\r\n");
    Send_String("╔══════════════════════════════════════════╗\r\n");
    Send_String("║     LED CONTROL SYSTEM - STM32F4         ║\r\n");
    Send_String("╠══════════════════════════════════════════╣\r\n");
    Send_String("║  SINGLE KEY COMMANDS:                    ║\r\n");
    Send_String("║  1 - Blink ALL LEDs                      ║\r\n");
    Send_String("║  2 - Blink RED LED (PD14)                ║\r\n");
    Send_String("║  3 - Blink GREEN LED (PD12)              ║\r\n");
    Send_String("║  4 - Blink BLUE LED (PD15)               ║\r\n");
    Send_String("║  5 - Blink ORANGE LED (PD13)             ║\r\n");
    Send_String("║  6 - Running LED Pattern                 ║\r\n");
    Send_String("║  7 - Alternate LEDs Pattern              ║\r\n");
    Send_String("║  8 - Knight Rider Pattern                ║\r\n");
    Send_String("║  9 - Binary Counter Pattern              ║\r\n");
    Send_String("║  0 - All LEDs OFF                        ║\r\n");
    Send_String("╠══════════════════════════════════════════╣\r\n");
    Send_String("║  STRING COMMANDS (Press Enter):          ║\r\n");
    Send_String("║  LED_ON     - Turn ALL LEDs ON           ║\r\n");
    Send_String("║  LED_OFF    - Turn ALL LEDs OFF          ║\r\n");
    Send_String("║  LED_RED    - Toggle Red LED             ║\r\n");
    Send_String("║  LED_GREEN  - Toggle Green LED           ║\r\n");
    Send_String("║  LED_BLUE   - Toggle Blue LED            ║\r\n");
    Send_String("║  LED_ORANGE - Toggle Orange LED          ║\r\n");
    Send_String("║  MENU       - Show this menu             ║\r\n");
    Send_String("╚══════════════════════════════════════════╝\r\n");
    Send_String("\r\nEnter command: ");
}

/**
  * @brief  Blink all LEDs
  * @param  count: Number of blink cycles
  * @retval None
  */
void Blink_All_LEDs(uint8_t count)
{
    Send_String("\r\n>> Blinking ALL LEDs...\r\n");
    for (uint8_t i = 0; i < count; i++)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_SET);
        HAL_Delay(BLINK_DELAY);
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
        HAL_Delay(BLINK_DELAY);
    }
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Blink single LED
  * @param  pin: GPIO pin of LED
  * @param  count: Number of blink cycles
  * @retval None
  */
void Blink_Single_LED(uint16_t pin, uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        HAL_GPIO_WritePin(LED_PORT, pin, GPIO_PIN_SET);
        HAL_Delay(BLINK_DELAY);
        HAL_GPIO_WritePin(LED_PORT, pin, GPIO_PIN_RESET);
        HAL_Delay(BLINK_DELAY);
    }
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Running LED pattern (circular)
  * @param  count: Number of complete cycles
  * @retval None
  */
void Running_LED_Pattern(uint8_t count)
{
    uint16_t leds[] = {LED_GREEN_PIN, LED_ORANGE_PIN, LED_RED_PIN, LED_BLUE_PIN};

    Send_String("\r\n>> Running LED Pattern...\r\n");

    for (uint8_t cycle = 0; cycle < count; cycle++)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            All_LEDs_OFF();
            HAL_GPIO_WritePin(LED_PORT, leds[i], GPIO_PIN_SET);
            HAL_Delay(BLINK_DELAY);
        }
    }
    All_LEDs_OFF();
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Alternate LEDs pattern
  * @param  count: Number of cycles
  * @retval None
  */
void Alternate_LEDs_Pattern(uint8_t count)
{
    Send_String("\r\n>> Alternate LEDs Pattern...\r\n");

    for (uint8_t i = 0; i < count; i++)
    {
        /* Pattern 1: Green and Red ON */
        HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN | LED_RED_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_PORT, LED_ORANGE_PIN | LED_BLUE_PIN, GPIO_PIN_RESET);
        HAL_Delay(BLINK_DELAY * 2);

        /* Pattern 2: Orange and Blue ON */
        HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN | LED_RED_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_PORT, LED_ORANGE_PIN | LED_BLUE_PIN, GPIO_PIN_SET);
        HAL_Delay(BLINK_DELAY * 2);
    }
    All_LEDs_OFF();
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Knight Rider pattern (back and forth)
  * @param  count: Number of complete cycles
  * @retval None
  */
void Knight_Rider_Pattern(uint8_t count)
{
    uint16_t leds[] = {LED_GREEN_PIN, LED_ORANGE_PIN, LED_RED_PIN, LED_BLUE_PIN};

    Send_String("\r\n>> Knight Rider Pattern...\r\n");

    for (uint8_t cycle = 0; cycle < count; cycle++)
    {
        /* Forward direction */
        for (uint8_t i = 0; i < 4; i++)
        {
            All_LEDs_OFF();
            HAL_GPIO_WritePin(LED_PORT, leds[i], GPIO_PIN_SET);
            HAL_Delay(BLINK_DELAY);
        }

        /* Backward direction */
        for (int8_t i = 2; i >= 0; i--)
        {
            All_LEDs_OFF();
            HAL_GPIO_WritePin(LED_PORT, leds[i], GPIO_PIN_SET);
            HAL_Delay(BLINK_DELAY);
        }
    }
    All_LEDs_OFF();
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Binary counter pattern (0-15)
  * @retval None
  */
void Binary_Count_Pattern(void)
{
    Send_String("\r\n>> Binary Counter Pattern (0-15)...\r\n");

    for (uint8_t i = 0; i <= 15; i++)
    {
        /* Display binary value on LEDs */
        HAL_GPIO_WritePin(LED_PORT, LED_GREEN_PIN, (i & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);  // Bit 0
        HAL_GPIO_WritePin(LED_PORT, LED_ORANGE_PIN, (i & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // Bit 1
        HAL_GPIO_WritePin(LED_PORT, LED_RED_PIN, (i & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);    // Bit 2
        HAL_GPIO_WritePin(LED_PORT, LED_BLUE_PIN, (i & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);   // Bit 3

        HAL_Delay(BLINK_DELAY * 3);
    }
    All_LEDs_OFF();
    Send_String(">> Done!\r\n");
}

/**
  * @brief  Turn all LEDs ON
  * @retval None
  */
void All_LEDs_ON(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_SET);
}

/**
  * @brief  Turn all LEDs OFF
  * @retval None
  */
void All_LEDs_OFF(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
}

/**
  * @brief  Process single character command (immediate action)
  * @param  cmd: Single character command
  * @retval None
  */
void Process_Single_Key(uint8_t cmd)
{
    switch (cmd)
    {
        case '1':
            Blink_All_LEDs(5);
            break;

        case '2':
            Send_String("\r\n>> Blinking RED LED...\r\n");
            Blink_Single_LED(LED_RED_PIN, 5);
            break;

        case '3':
            Send_String("\r\n>> Blinking GREEN LED...\r\n");
            Blink_Single_LED(LED_GREEN_PIN, 5);
            break;

        case '4':
            Send_String("\r\n>> Blinking BLUE LED...\r\n");
            Blink_Single_LED(LED_BLUE_PIN, 5);
            break;

        case '5':
            Send_String("\r\n>> Blinking ORANGE LED...\r\n");
            Blink_Single_LED(LED_ORANGE_PIN, 5);
            break;

        case '6':
            Running_LED_Pattern(3);
            break;

        case '7':
            Alternate_LEDs_Pattern(5);
            break;

        case '8':
            Knight_Rider_Pattern(3);
            break;

        case '9':
            Binary_Count_Pattern();
            break;

        case '0':
            All_LEDs_OFF();
            Send_String("\r\n>> All LEDs OFF\r\n");
            break;

        default:
            /* Not a single key command, ignore */
            break;
    }
}

/**
  * @brief  Process string command (requires Enter)
  * @param  cmd: Pointer to command string
  * @retval None
  */
void Process_Command(char *cmd)
{
    char response[50];

    /* Show received command */
    Send_String("\r\nReceived: ");
    Send_String(cmd);

    if (strcmp(cmd, "LED_ON") == 0)
    {
        All_LEDs_ON();
        Send_String("\r\n>> All LEDs turned ON\r\n");
    }
    else if (strcmp(cmd, "LED_OFF") == 0)
    {
        All_LEDs_OFF();
        Send_String("\r\n>> All LEDs turned OFF\r\n");
    }
    else if (strcmp(cmd, "LED_RED") == 0)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_RED_PIN);
        Send_String("\r\n>> Red LED Toggled\r\n");
    }
    else if (strcmp(cmd, "LED_GREEN") == 0)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_GREEN_PIN);
        Send_String("\r\n>> Green LED Toggled\r\n");
    }
    else if (strcmp(cmd, "LED_BLUE") == 0)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_BLUE_PIN);
        Send_String("\r\n>> Blue LED Toggled\r\n");
    }
    else if (strcmp(cmd, "LED_ORANGE") == 0)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_ORANGE_PIN);
        Send_String("\r\n>> Orange LED Toggled\r\n");
    }
    else if (strcmp(cmd, "MENU") == 0)
    {
        Display_Menu();
    }
    else if (strcmp(cmd, "BLINK_ALL") == 0)
    {
        Blink_All_LEDs(10);
    }
    else if (strcmp(cmd, "RUNNING") == 0)
    {
        Running_LED_Pattern(5);
    }
    else if (strcmp(cmd, "KNIGHT") == 0)
    {
        Knight_Rider_Pattern(5);
    }
    else if (strcmp(cmd, "BINARY") == 0)
    {
        Binary_Count_Pattern();
    }
    else if (strcmp(cmd, "ALTERNATE") == 0)
    {
        Alternate_LEDs_Pattern(5);
    }
    else if (strlen(cmd) == 1)
    {
        /* Single character entered with Enter - process as key command */
        Process_Single_Key(cmd[0]);
    }
    else
    {
        Send_String("\r\n>> Invalid Command! Type MENU for help.\r\n");
    }

    Send_String("\r\nEnter command: ");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  /* Display welcome menu */
  Display_Menu();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Receive one character using polling mode */
    HAL_UART_Receive(&huart2, &rx_byte, 1, HAL_MAX_DELAY);

    /* Check if ENTER key is pressed */
    if (rx_byte == '\r')
    {
      /* Terminate string with null character */
      rx_buffer[rx_index] = '\0';

      /* Process the command string */
      Process_Command(rx_buffer);

      /* Reset buffer index for next command */
      rx_index = 0;
    }
    else if (rx_byte >= '0' && rx_byte <= '9')
    {
      /* Immediate single key command (numbers 0-9) */
      /* Echo the key */
      HAL_UART_Transmit(&huart2, &rx_byte, 1, HAL_MAX_DELAY);

      /* Process immediately without waiting for Enter */
      Process_Single_Key(rx_byte);

      Send_String("\r\nEnter command: ");

      /* Reset buffer */
      rx_index = 0;
    }
    else
    {
      /* Store character in buffer (with overflow protection) */
      if (rx_index < MAX_LEN - 1)
      {
        rx_buffer[rx_index++] = rx_byte;
      }

      /* Echo character back for user feedback */
      HAL_UART_Transmit(&huart2, &rx_byte, 1, HAL_MAX_DELAY);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
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
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure GPIO pin Output Level - LEDs OFF initially */
  HAL_GPIO_WritePin(GPIOD, LED_ALL_PINS, GPIO_PIN_RESET);

  /* Configure GPIO pins : PD12, PD13, PD14, PD15 as Output */
  GPIO_InitStruct.Pin = LED_ALL_PINS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
