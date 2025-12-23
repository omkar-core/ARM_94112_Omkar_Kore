/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Switch controlled LED patterns
  ******************************************************************************
  */
 /* USER CODE END Header */

#include "main.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
uint8_t mode = 0;
GPIO_PinState lastSwitch = GPIO_PIN_RESET;
GPIO_PinState currentSwitch;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
void LED_All_On(void);
void LED_All_Off(void);
void LED_All_Blink(void);
void LED_Running(void);
void LED_Alternate(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#define LED_ALL (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  while (1)
  {
    currentSwitch = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    /* Detect button press (rising edge) */
    if ((currentSwitch == GPIO_PIN_SET) &&
        (lastSwitch == GPIO_PIN_RESET))
    {
      HAL_Delay(20); // debounce

      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
      {
        mode++;
        if (mode > 4) mode = 0;
      }
    }

    lastSwitch = currentSwitch;

    /* Execute LED pattern */
    switch (mode)
    {
      case 0: LED_All_On(); break;
      case 1: LED_All_Blink(); break;
      case 2: LED_Running(); break;
      case 3: LED_Alternate(); break;
      case 4: LED_All_Off(); break;
    }
  }
}

/* ================= LED PATTERN FUNCTIONS ================= */

void LED_All_On(void)
{
  HAL_GPIO_WritePin(GPIOD, LED_ALL, GPIO_PIN_SET);
}

void LED_All_Off(void)
{
  HAL_GPIO_WritePin(GPIOD, LED_ALL, GPIO_PIN_RESET);
}

void LED_All_Blink(void)
{
  HAL_GPIO_TogglePin(GPIOD, LED_ALL);
  HAL_Delay(300);
}

void LED_Running(void)
{
  static uint8_t index = 0;

  HAL_GPIO_WritePin(GPIOD, LED_ALL, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, (GPIO_PIN_12 << index), GPIO_PIN_SET);

  index++;
  if (index > 3) index = 0;

  HAL_Delay(200);
}

void LED_Alternate(void)
{
  static uint8_t toggle = 0;

  if (toggle == 0)
  {
    HAL_GPIO_WritePin(GPIOD,
      GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,
      GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOD,
      GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,
      GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_RESET);
  }

  toggle ^= 1;
  HAL_Delay(300);
}

/* ================= SYSTEM FUNCTIONS (UNCHANGED) ================= */

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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* PA0 - Switch */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PD12–PD15 - LEDs */
  GPIO_InitStruct.Pin = LED_ALL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
