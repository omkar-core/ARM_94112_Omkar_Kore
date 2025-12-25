/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : UART String Processing with Enhanced Display
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
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LEN 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_byte;                  // Single incoming byte
char rx_buffer[MAX_LEN];          // Store received string
char upper_buffer[MAX_LEN];       // Store uppercase string
char lower_buffer[MAX_LEN];       // Store lowercase string
uint16_t rx_index = 0;            // Current position
char tx_buffer[MAX_LEN];          // Buffer for sprintf
uint32_t string_count = 0;        // Count of processed strings
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Send_String(char *str);
void Convert_To_Uppercase(char *src, char *dest);
void Convert_To_Lowercase(char *src, char *dest);
uint16_t Count_Vowels(char *str);
uint16_t Count_Consonants(char *str);
uint16_t Count_Digits(char *str);
uint16_t Count_Spaces(char *str);
uint16_t Count_Special(char *str);
void Display_Welcome_Screen(void);
void Display_Result_Box(char *original);
void Display_Prompt(void);
void Clear_Screen(void);
void Display_Divider(void);
void Display_Header(char *title);
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
  * @brief  Convert string to uppercase
  * @param  src: Source string
  * @param  dest: Destination string
  * @retval None
  */
void Convert_To_Uppercase(char *src, char *dest)
{
    uint16_t i = 0;
    while (src[i] != '\0')
    {
        if (src[i] >= 'a' && src[i] <= 'z')
        {
            dest[i] = src[i] - 32;
        }
        else
        {
            dest[i] = src[i];
        }
        i++;
    }
    dest[i] = '\0';
}

/**
  * @brief  Convert string to lowercase
  * @param  src: Source string
  * @param  dest: Destination string
  * @retval None
  */
void Convert_To_Lowercase(char *src, char *dest)
{
    uint16_t i = 0;
    while (src[i] != '\0')
    {
        if (src[i] >= 'A' && src[i] <= 'Z')
        {
            dest[i] = src[i] + 32;
        }
        else
        {
            dest[i] = src[i];
        }
        i++;
    }
    dest[i] = '\0';
}

/**
  * @brief  Count vowels in string
  * @param  str: Input string
  * @retval Number of vowels
  */
uint16_t Count_Vowels(char *str)
{
    uint16_t count = 0;
    for (uint16_t i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') c += 32;  // Convert to lowercase
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        {
            count++;
        }
    }
    return count;
}

/**
  * @brief  Count consonants in string
  * @param  str: Input string
  * @retval Number of consonants
  */
uint16_t Count_Consonants(char *str)
{
    uint16_t count = 0;
    for (uint16_t i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') c += 32;
        if (c >= 'a' && c <= 'z')
        {
            if (!(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'))
            {
                count++;
            }
        }
    }
    return count;
}

/**
  * @brief  Count digits in string
  * @param  str: Input string
  * @retval Number of digits
  */
uint16_t Count_Digits(char *str)
{
    uint16_t count = 0;
    for (uint16_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            count++;
        }
    }
    return count;
}

/**
  * @brief  Count spaces in string
  * @param  str: Input string
  * @retval Number of spaces
  */
uint16_t Count_Spaces(char *str)
{
    uint16_t count = 0;
    for (uint16_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

/**
  * @brief  Count special characters in string
  * @param  str: Input string
  * @retval Number of special characters
  */
uint16_t Count_Special(char *str)
{
    uint16_t count = 0;
    for (uint16_t i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == ' '))
        {
            count++;
        }
    }
    return count;
}

/**
  * @brief  Display horizontal divider
  * @retval None
  */
void Display_Divider(void)
{
    Send_String("\r\n+--------------------------------------------------+\r\n");
}

/**
  * @brief  Display section header
  * @param  title: Header title
  * @retval None
  */
void Display_Header(char *title)
{
    Send_String("\r\n");
    Send_String("+--------------------------------------------------+\r\n");
    Send_String("|                                                  |\r\n");
    sprintf(tx_buffer, "|%*s%s%*s|\r\n",
            (25 - strlen(title)/2), "", title, (25 - (strlen(title)+1)/2), "");
    Send_String(tx_buffer);
    Send_String("|                                                  |\r\n");
    Send_String("+--------------------------------------------------+\r\n");
}

/**
  * @brief  Display welcome screen
  * @retval None
  */
void Display_Welcome_Screen(void)
{
    Send_String("\r\n\r\n");
    Send_String("    ╔═══════════════════════════════════════════════════╗\r\n");
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ║   ███████╗████████╗███╗   ███╗██████╗ ██████╗     ║\r\n");
    Send_String("    ║   ██╔════╝╚══██╔══╝████╗ ████║╚════██╗╚════██╗    ║\r\n");
    Send_String("    ║   ███████╗   ██║   ██╔████╔██║ █████╔╝ █████╔╝    ║\r\n");
    Send_String("    ║   ╚════██║   ██║   ██║╚██╔╝██║ ╚═══██╗██╔═══╝     ║\r\n");
    Send_String("    ║   ███████║   ██║   ██║ ╚═╝ ██║██████╔╝███████╗    ║\r\n");
    Send_String("    ║   ╚══════╝   ╚═╝   ╚═╝     ╚═╝╚═════╝ ╚══════╝    ║\r\n");
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║       STRING PROCESSING TERMINAL v1.0             ║\r\n");
    Send_String("    ║              Polling Mode - USART2                ║\r\n");
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ║   Features:                                       ║\r\n");
    Send_String("    ║   [✓] Uppercase Conversion                        ║\r\n");
    Send_String("    ║   [✓] Lowercase Conversion                        ║\r\n");
    Send_String("    ║   [✓] Character Analysis                          ║\r\n");
    Send_String("    ║   [✓] Real-time Echo                              ║\r\n");
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ╚═══════════════════════════════════════════════════╝\r\n");
    Send_String("\r\n");
}

/**
  * @brief  Display input prompt
  * @retval None
  */
void Display_Prompt(void)
{
    Send_String("\r\n");
    Send_String("    ┌─────────────────────────────────────────────┐\r\n");
    sprintf(tx_buffer, "    │  String #%lu - Enter text below:            │\r\n", string_count + 1);
    Send_String(tx_buffer);
    Send_String("    │  (Press ENTER to process)                  │\r\n");
    Send_String("    └─────────────────────────────────────────────┘\r\n");
    Send_String("\r\n    >> ");
}

/**
  * @brief  Display processing animation
  * @retval None
  */
void Display_Processing(void)
{
    Send_String("\r\n\r\n    Processing");
    for (int i = 0; i < 3; i++)
    {
        HAL_Delay(150);
        Send_String(".");
    }
    HAL_Delay(150);
    Send_String(" Done!\r\n");
}

/**
  * @brief  Display result box with all information
  * @param  original: Original string
  * @retval None
  */
void Display_Result_Box(char *original)
{
    uint16_t len = strlen(original);
    uint16_t vowels = Count_Vowels(original);
    uint16_t consonants = Count_Consonants(original);
    uint16_t digits = Count_Digits(original);
    uint16_t spaces = Count_Spaces(original);
    uint16_t special = Count_Special(original);

    Convert_To_Uppercase(original, upper_buffer);
    Convert_To_Lowercase(original, lower_buffer);

    /* Display processing animation */
    Display_Processing();

    /* Result Box */
    Send_String("\r\n    ╔═══════════════════════════════════════════════════╗\r\n");
    Send_String("    ║              📊 ANALYSIS RESULTS                   ║\r\n");
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");

    /* Original String */
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ║  📝 ORIGINAL STRING:                              ║\r\n");
    sprintf(tx_buffer, "    ║     \"%s\"\r\n", original);
    Send_String(tx_buffer);
    Send_String("    ║                                                   ║\r\n");

    /* Separator */
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");

    /* Uppercase */
    Send_String("    ║                                                   ║\r\n");
    Send_String("    ║  🔼 UPPERCASE:                                    ║\r\n");
    sprintf(tx_buffer, "    ║     \"%s\"\r\n", upper_buffer);
    Send_String(tx_buffer);
    Send_String("    ║                                                   ║\r\n");

    /* Lowercase */
    Send_String("    ║  🔽 LOWERCASE:                                    ║\r\n");
    sprintf(tx_buffer, "    ║     \"%s\"\r\n", lower_buffer);
    Send_String(tx_buffer);
    Send_String("    ║                                                   ║\r\n");

    /* Separator */
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║              📈 CHARACTER STATISTICS              ║\r\n");
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║                                                   ║\r\n");

    /* Statistics */
    sprintf(tx_buffer, "    ║    📏 Total Length    : %-3d characters          ║\r\n", len);
    Send_String(tx_buffer);

    sprintf(tx_buffer, "    ║    🅰️  Vowels          : %-3d                     ║\r\n", vowels);
    Send_String(tx_buffer);

    sprintf(tx_buffer, "    ║    🅱️  Consonants      : %-3d                     ║\r\n", consonants);
    Send_String(tx_buffer);

    sprintf(tx_buffer, "    ║    🔢 Digits          : %-3d                     ║\r\n", digits);
    Send_String(tx_buffer);

    sprintf(tx_buffer, "    ║    ⬜ Spaces          : %-3d                     ║\r\n", spaces);
    Send_String(tx_buffer);

    sprintf(tx_buffer, "    ║    ✨ Special Chars   : %-3d                     ║\r\n", special);
    Send_String(tx_buffer);

    Send_String("    ║                                                   ║\r\n");

    /* Visual Bar Graph */
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║              📊 VISUAL BREAKDOWN                  ║\r\n");
    Send_String("    ╠═══════════════════════════════════════════════════╣\r\n");
    Send_String("    ║                                                   ║\r\n");

    /* Vowels bar */
    Send_String("    ║  Vowels     [");
    for (int i = 0; i < 20; i++)
    {
        if (i < (vowels * 20) / (len > 0 ? len : 1))
            Send_String("█");
        else
            Send_String("░");
    }
    sprintf(tx_buffer, "] %d%%\r\n", len > 0 ? (vowels * 100) / len : 0);
    Send_String(tx_buffer);

    /* Consonants bar */
    Send_String("    ║  Consonants [");
    for (int i = 0; i < 20; i++)
    {
        if (i < (consonants * 20) / (len > 0 ? len : 1))
            Send_String("█");
        else
            Send_String("░");
    }
    sprintf(tx_buffer, "] %d%%\r\n", len > 0 ? (consonants * 100) / len : 0);
    Send_String(tx_buffer);

    /* Digits bar */
    Send_String("    ║  Digits     [");
    for (int i = 0; i < 20; i++)
    {
        if (i < (digits * 20) / (len > 0 ? len : 1))
            Send_String("█");
        else
            Send_String("░");
    }
    sprintf(tx_buffer, "] %d%%\r\n", len > 0 ? (digits * 100) / len : 0);
    Send_String(tx_buffer);

    /* Others bar */
    uint16_t others = spaces + special;
    Send_String("    ║  Others     [");
    for (int i = 0; i < 20; i++)
    {
        if (i < (others * 20) / (len > 0 ? len : 1))
            Send_String("█");
        else
            Send_String("░");
    }
    sprintf(tx_buffer, "] %d%%\r\n", len > 0 ? (others * 100) / len : 0);
    Send_String(tx_buffer);

    Send_String("    ║                                                   ║\r\n");
    Send_String("    ╚═══════════════════════════════════════════════════╝\r\n");

    /* Update counter */
    string_count++;

    /* Summary line */
    Send_String("\r\n    ✅ Processing complete!\r\n");
    sprintf(tx_buffer, "    📊 Total strings processed: %lu\r\n", string_count);
    Send_String(tx_buffer);
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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
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
  /* Display welcome screen */
  Display_Welcome_Screen();
  Display_Prompt();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* Receive one byte (blocking polling) */
      HAL_UART_Receive(&huart2, &rx_byte, 1, HAL_MAX_DELAY);

      /* ENTER key handling */
      if (rx_byte == '\r')
      {
          /* Terminate string */
          rx_buffer[rx_index] = '\0';

          /* Check if string is not empty */
          if (rx_index > 0)
          {
              /* Display results */
              Display_Result_Box(rx_buffer);
          }
          else
          {
              Send_String("\r\n    ⚠️  Empty input! Please enter some text.\r\n");
          }

          /* Reset buffer and show new prompt */
          rx_index = 0;
          Display_Prompt();
      }
      /* Backspace handling */
      else if (rx_byte == 0x7F || rx_byte == 0x08)
      {
          if (rx_index > 0)
          {
              rx_index--;
              /* Send backspace sequence to terminal */
              Send_String("\b \b");
          }
      }
      /* Normal character */
      else
      {
          /* Store character in buffer (with overflow protection) */
          if (rx_index < MAX_LEN - 1)
          {
              rx_buffer[rx_index++] = rx_byte;
              /* Echo received byte */
              HAL_UART_Transmit(&huart2, &rx_byte, 1, HAL_MAX_DELAY);
          }
          else
          {
              /* Buffer full warning */
              Send_String("\r\n    ⚠️  Buffer full! Press ENTER to process.\r\n    >> ");
          }
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
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

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
