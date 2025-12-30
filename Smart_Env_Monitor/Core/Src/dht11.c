/**
  ******************************************************************************
  * @file           : dht11.c
  * @brief          : DHT11 Temperature & Humidity Sensor implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dht11.h"
#include "main.h"
#include "config.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t last_temperature = 0;
static uint8_t last_humidity = 0;

/* Private function prototypes -----------------------------------------------*/
static void DHT11_SetPinOutput(void);
static void DHT11_SetPinInput(void);
static uint8_t DHT11_ReadPin(void);
static void DHT11_DelayUs(uint32_t us);
static uint8_t DHT11_WaitFor(uint8_t state, uint32_t timeout_us);
static uint8_t DHT11_ReadByte(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Microsecond delay using loop
  * @param  us: Microseconds to delay
  * @retval None
  * @note   Calibrated for 168MHz clock
  */
static void DHT11_DelayUs(uint32_t us)
{
    /* At 168MHz, approximately 42 cycles per microsecond */
    volatile uint32_t count = us * 42;
    while(count--)
    {
        __NOP();
    }
}

/**
  * @brief  Configure DHT11 pin as output
  * @retval None
  */
static void DHT11_SetPinOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/**
  * @brief  Configure DHT11 pin as input
  * @retval None
  */
static void DHT11_SetPinInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/**
  * @brief  Read DHT11 data pin state
  * @retval Pin state (0 or 1)
  */
static uint8_t DHT11_ReadPin(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN);
}

/**
  * @brief  Wait for pin to reach specified state with timeout
  * @param  state: Expected state (0 or 1)
  * @param  timeout_us: Timeout in microseconds
  * @retval 1 if state reached, 0 if timeout
  */
static uint8_t DHT11_WaitFor(uint8_t state, uint32_t timeout_us)
{
    uint32_t count = 0;
    uint32_t max_count = timeout_us * 42;  /* Calibrated for 168MHz */

    while (DHT11_ReadPin() != state)
    {
        count++;
        if (count > max_count)
        {
            return 0;  /* Timeout */
        }
    }

    return 1;  /* Success */
}

/**
  * @brief  Read one byte from DHT11
  * @retval Byte value
  */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t byte = 0;
    uint32_t count;

    for (uint8_t i = 0; i < 8; i++)
    {
        /* Wait for pin to go HIGH (start of bit) */
        if (!DHT11_WaitFor(1, 100))
        {
            return 0;
        }

        /* Measure HIGH duration */
        count = 0;
        while (DHT11_ReadPin() == 1)
        {
            count++;
            if (count > 5000)
            {
                return 0;  /* Timeout */
            }
        }

        /* Shift byte and add bit */
        byte <<= 1;

        /* If HIGH duration > threshold, it's a '1' */
        /* At 168MHz, count > ~1680 means > 40us */
        if (count > 1680)
        {
            byte |= 0x01;
        }
    }

    return byte;
}

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize DHT11 sensor
  * @retval None
  */
void DHT11_Init(void)
{
    /* Set pin high (idle state) */
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);

    /* Wait for sensor to stabilize (at least 1 second) */
    HAL_Delay(1000);
}

/**
  * @brief  Read temperature and humidity from DHT11
  * @param  temperature: Pointer to store temperature value
  * @param  humidity: Pointer to store humidity value
  * @retval DHT11_Status_t status
  */
DHT11_Status_t DHT11_Read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0};
    uint8_t checksum;

    /*--------------------------------------------------------------------------
                            STEP 1: SEND START SIGNAL
    --------------------------------------------------------------------------*/

    /* Set pin as output */
    DHT11_SetPinOutput();

    /* Pull LOW for at least 18ms */
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(DHT11_START_SIGNAL_MS);

    /* Release the bus (set HIGH) */
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    DHT11_DelayUs(30);

    /*--------------------------------------------------------------------------
                            STEP 2: WAIT FOR RESPONSE
    --------------------------------------------------------------------------*/

    /* Switch to input mode */
    DHT11_SetPinInput();

    /* Wait for DHT11 to pull LOW (response start) */
    if (!DHT11_WaitFor(0, 100))
    {
        /* Return pin to output mode */
        DHT11_SetPinOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        return DHT11_ERROR_NO_RESPONSE;
    }

    /* Wait for LOW period (~80us) to end */
    if (!DHT11_WaitFor(1, 100))
    {
        DHT11_SetPinOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        return DHT11_ERROR_TIMEOUT;
    }

    /* Wait for HIGH period (~80us) to end */
    if (!DHT11_WaitFor(0, 100))
    {
        DHT11_SetPinOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        return DHT11_ERROR_TIMEOUT;
    }

    /*--------------------------------------------------------------------------
                            STEP 3: READ 5 BYTES OF DATA
    --------------------------------------------------------------------------*/

    for (uint8_t i = 0; i < 5; i++)
    {
        data[i] = DHT11_ReadByte();
    }

    /*--------------------------------------------------------------------------
                            STEP 4: VERIFY CHECKSUM
    --------------------------------------------------------------------------*/

    checksum = data[0] + data[1] + data[2] + data[3];

    if (checksum != data[4])
    {
        /* Return pin to output mode */
        DHT11_SetPinOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        return DHT11_ERROR_CHECKSUM;
    }

    /*--------------------------------------------------------------------------
                            STEP 5: EXTRACT DATA
    --------------------------------------------------------------------------*/

    /* Data format:
     * data[0] = Humidity integer part
     * data[1] = Humidity decimal part (always 0 for DHT11)
     * data[2] = Temperature integer part
     * data[3] = Temperature decimal part (always 0 for DHT11)
     * data[4] = Checksum
     */

    last_humidity = data[0];
    last_temperature = data[2];

    /* Return values through pointers */
    if (temperature != NULL)
    {
        *temperature = last_temperature;
    }

    if (humidity != NULL)
    {
        *humidity = last_humidity;
    }

    /*--------------------------------------------------------------------------
                            STEP 6: CLEANUP
    --------------------------------------------------------------------------*/

    /* Return pin to output mode (idle HIGH) */
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);

    return DHT11_OK;
}

/**
  * @brief  Get last temperature reading
  * @retval Temperature in °C
  */
uint8_t DHT11_GetTemperature(void)
{
    return last_temperature;
}

/**
  * @brief  Get last humidity reading
  * @retval Humidity in %
  */
uint8_t DHT11_GetHumidity(void)
{
    return last_humidity;
}
