/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
enum measure_type_ind{
	Measure_IN,
	Measure_OUT,
};

enum measure_ind{
	Humi_IN,
	Temp_IN,
	CO2_IN,
	Humi_OUT,
	Temp_OUT,
	Pres_OUT,
	Dust_OUT,
};

enum dzien{
	dzisiaj,
	wczoraj,
	przedwczoraj
};

struct sensor_table{
	uint8_t num;
	uint8_t tenths;
};

struct measure_type{
	uint8_t ind[4];
	uint8_t iter;
	struct sensor_table pomiar[4];
	uint32_t bytes_writen[4];
};
struct kolejka{
	uint16_t etap;
	uint16_t krok;
};

struct polecenie{
	uint16_t time_init;
	char msg[90];
	uint16_t time_response;
	char rsp[20];
	struct kolejka powrot;
	uint16_t time_exit;
	uint8_t iter;
	uint8_t terminate;
};

enum tryb{
	HOTSPOT,
	ROUTER_MODE,
	WIFI_CHANGE,
	RESET_MODE_TO_HOTSPOT,
	RESET_MODE_TO_ROUTER,
	RESET_TO_WIFI_CHANGE
};

enum communication_menage{
	AT_COMMAND_IDLE_WAITING,
	AT_COMMAND_SENDING,
	AT_COMMAND_FIND_RESPONSE,
	AT_COMMAND_WAIT_AND_EXIT
};

typedef struct{
	int16_t x;
	int16_t y;
}calib;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}touch;

typedef struct{
	int DEN;
	int A;
	int B;
	int C;
	int D;
	int E;
	int F;
}wsp;
typedef enum
{
	XPT2046_IDLE,		// 0
	XPT2046_PRESAMPLING, // 1
	XPT2046_TOUCHED,	// 2
	XPT2046_RELEASED	// 3
}XPT2046_State;

struct search_letter_table{
	uint8_t ind;
	char letter;
	uint32_t pomiar;
	char data_label[7];
};

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_PC13_Pin GPIO_PIN_13
#define LED_PC13_GPIO_Port GPIOC
#define TCH_CS_Pin GPIO_PIN_1
#define TCH_CS_GPIO_Port GPIOA
#define BACKLIGHT_Pin GPIO_PIN_6
#define BACKLIGHT_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_0
#define LCD_DC_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOB
#define TCH_IRQ_Pin GPIO_PIN_2
#define TCH_IRQ_GPIO_Port GPIOB
#define TCH_IRQ_EXTI_IRQn EXTI2_IRQn
#define LCD_CS_Pin GPIO_PIN_10
#define LCD_CS_GPIO_Port GPIOB
#define esp_reset_Pin GPIO_PIN_12
#define esp_reset_GPIO_Port GPIOB
#define ESP_ENABLE_Pin GPIO_PIN_8
#define ESP_ENABLE_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_11
#define SPI2_CS_GPIO_Port GPIOA
#define SD_RELAY_Pin GPIO_PIN_6
#define SD_RELAY_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RxBuf_SIZE   1000
#define RxMainBuf_SIZE 1000
#define USART_FLAG_IDLE (1 << 4)
#define USART_SR_TC (1 << 6)
#define RTC_CR_ALRAIE (1<<12)
#define I2C_HANDLE &hi2c1
#define SCD40_ADDRESS (0x62)<<1// I2C address of the SCD40 sensor

#define XPT2046_SAMPLE 25
#define MAXSAMPLE 3
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
