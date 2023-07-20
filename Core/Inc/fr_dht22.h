/*
 * dht22.h
 *
 *  Created on: Oct 12, 2022
 *      Author: Filip
 */

#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stdint.h"
#include "stdio.h"
#include "main.h"
#include "fr_sd_logger.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "fr_global.h"

#define DHT22_PORT GPIOA
#define DHT22_PIN GPIO_PIN_6


void DHT22_Start(void);
uint8_t DHT22_Check_Response(void);
uint8_t DHT22_Read(void);
extern TIM_HandleTypeDef htim3;
void delay(uint32_t time);
void DHT22_Measure(void);
extern long double TemperatureIN;
extern long double HumidityIN;
extern char str_temp_in[12];
extern char str_hum_in[12];
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern int ind;
extern RTC_TimeTypeDef time_measure;
extern RTC_HandleTypeDef hrtc;

extern struct measure_type measure_tab[2];

#endif /* INC_DHT22_H_ */

