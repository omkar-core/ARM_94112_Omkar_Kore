/**
  ******************************************************************************
  * @file           : config.h
  * @brief          : System configuration and threshold values
  ******************************************************************************
  */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
                        SYSTEM CONFIGURATION
------------------------------------------------------------------------------*/

/* Sensor Reading Interval (milliseconds) */
#define SENSOR_READ_INTERVAL_MS     2000    /* Read every 2 seconds */

/*------------------------------------------------------------------------------
                        MQ-2 GAS SENSOR THRESHOLDS
------------------------------------------------------------------------------*/

/* ADC is 12-bit: 0 - 4095 */
#define MQ2_ADC_MAX                 4095

/* Gas detection thresholds (ADC values) */
#define MQ2_GAS_THRESHOLD           1800    /* Alert threshold */
#define MQ2_GAS_WARNING             1200    /* Warning threshold */
#define MQ2_GAS_SAFE                800     /* Safe level */

/*------------------------------------------------------------------------------
                        DHT11 SENSOR THRESHOLDS
------------------------------------------------------------------------------*/

/* Temperature thresholds (°C) */
#define TEMP_THRESHOLD_HIGH         40      /* High temperature alert */
#define TEMP_THRESHOLD_LOW          10      /* Low temperature alert */
#define TEMP_NORMAL_MIN             18      /* Normal range minimum */
#define TEMP_NORMAL_MAX             30      /* Normal range maximum */

/* Humidity thresholds (%) */
#define HUMIDITY_THRESHOLD_HIGH     80      /* High humidity alert */
#define HUMIDITY_THRESHOLD_LOW      20      /* Low humidity alert */
#define HUMIDITY_NORMAL_MIN         30      /* Normal range minimum */
#define HUMIDITY_NORMAL_MAX         60      /* Normal range maximum */

/*------------------------------------------------------------------------------
                        LCD CONFIGURATION
------------------------------------------------------------------------------*/

/* I2C LCD Address (PCF8574 based) */
#define LCD_I2C_ADDRESS             0x27    /* Common address: 0x27 or 0x3F */
#define LCD_I2C_ADDRESS_SHIFTED     (LCD_I2C_ADDRESS << 1)

/* LCD Dimensions */
#define LCD_ROWS                    2
#define LCD_COLS                    16

/*------------------------------------------------------------------------------
                        BUZZER CONFIGURATION
------------------------------------------------------------------------------*/

/* Buzzer patterns (in milliseconds) */
#define BUZZER_SHORT_BEEP           100
#define BUZZER_LONG_BEEP            500
#define BUZZER_ALARM_ON             200
#define BUZZER_ALARM_OFF            200

/*------------------------------------------------------------------------------
                        DHT11 TIMING (microseconds)
------------------------------------------------------------------------------*/
#define DHT11_START_SIGNAL_MS       20      /* Start signal duration */
#define DHT11_WAIT_RESPONSE_US      40      /* Wait for response */
#define DHT11_BIT_THRESHOLD_US      40      /* Threshold for 0/1 bit */

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */
