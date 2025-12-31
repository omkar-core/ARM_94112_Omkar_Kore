/*
 * dht11.c - DHT11 Sensor Driver
 */

#include "dht11.h"
#include "main.h"

static uint8_t last_temp = 25;
static uint8_t last_hum = 50;

static void SetOutput(void)
{
    GPIO_InitTypeDef g = {0};
    g.Pin = DHT11_PIN;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &g);
}

static void SetInput(void)
{
    GPIO_InitTypeDef g = {0};
    g.Pin = DHT11_PIN;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &g);
}

static uint8_t ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
        delay_us(40);
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
        {
            data |= (1 << (7 - i));
            while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
        }
    }
    return data;
}

void DHT11_Init(void)
{
    SetOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    HAL_Delay(1000);
}

bool DHT11_Read(uint8_t *temp, uint8_t *hum)
{
    uint8_t bytes[5] = {0};
    uint8_t timeout;

    SetOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    delay_us(30);
    SetInput();

    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100) { delay_us(1); timeout++; }
    if (timeout >= 100) { *temp = last_temp; *hum = last_hum; return false; }

    timeout = 0;
    while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100) { delay_us(1); timeout++; }

    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100) { delay_us(1); timeout++; }

    for (uint8_t i = 0; i < 5; i++)
        bytes[i] = ReadByte();

    if ((bytes[0] + bytes[1] + bytes[2] + bytes[3]) == bytes[4])
    {
        *hum = bytes[0];
        *temp = bytes[2];
        last_temp = *temp;
        last_hum = *hum;
        return true;
    }

    *temp = last_temp;
    *hum = last_hum;
    return false;
}

uint8_t DHT11_GetTemperature(void) { return last_temp; }
uint8_t DHT11_GetHumidity(void) { return last_hum; }
