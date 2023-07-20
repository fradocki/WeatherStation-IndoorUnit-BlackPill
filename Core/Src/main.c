/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
  *****************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ui.h>
#include "fr_lcd.h"
#include "../../lvgl/lvgl.h"
#include "../../lvgl/examples/lv_examples.h"
#include "stdio.h"
#include "stdint.h"
#include "fr_touch.h"
#include "fr_lv_drv_touch.h"
#include "fr_dht22.h"
#include "fr_lcd_lvgl_touch.h"
#include "fr_sd_handle.h"
#include <stdlib.h>
#include <ctype.h>
#include <fr_battery.h>
#include <fr_eeprom.h>
#include <fr_ESP8266.h>
#include <fr_ESP8266_messages.h>
#include <fatfs_sd.h>
#include "fr_scd40.h"
#include "fr_stm32.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  // ask for sensor data and wait for interrupt

  SCD40_writecommand();
  /* turn on led indication for work status */
  HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, GPIO_PIN_RESET);

  /* turn on power for SD card reader*/
  HAL_GPIO_WritePin(SD_RELAY_GPIO_Port, SD_RELAY_Pin, GPIO_PIN_SET);


  /* check what waked up device */

  if ((backlight)&&(init_lv==0)){
	/* in case wake up from touch screen initialize lvgl,
	 * display and turn on lvgl driver */
	HAL_GPIO_WritePin(TCH_CS_GPIO_Port, TCH_CS_Pin, GPIO_PIN_SET);
	init_lvgl();
	lv_task_handler();
	dotyk(&hspi3);

	/* enable tim1 timer for screen backlight turn off delay if wakedup from alarm */
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_SR_UIF);
	HAL_TIM_Base_Start_IT(&htim1);

  }

  /* in case wakeup by sensor timer initialize lvgl, and display */
  else{
	init_lvgl();
  }

  if(!PomiarFlag){

	/* tim4 timer ESP messages delay */
	HAL_TIM_Base_Start(&htim4);

	/* reset esp8266 module by output pin */
	ESP_Server_Init();

	/* start receiving data from esp8266 by USART1 circular DMA with line
	 * switch to idle interrupt, disable half transfer interrupt */
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) RxBuf, RxBuf_SIZE);

  }

  /* tim1 timer screen backlight */
  /* tim3 timer for us delay */

  /* tim9 timer for textbox color change */
  /* tim10 timer for msg delay */        ///do usunięcia
  /* tim11 timer for loading screen delay */

  //HAL_TIM_Base_Start(&htim3); //timer in case DHT22 usage;

  /* enable debugging in standby mode */
  HAL_DBGMCU_EnableDBGStandbyMode();

  /* initialize measurement structures */
  measure_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/* ESP8266 communication handle */
	if(PomiarFlag==0){
		switch(tryb_wifi){
		case HOTSPOT:
			load_hotspot();
		break;
		case ROUTER_MODE:
			load_msg();
		break;
		case RESET_TO_WIFI_CHANGE:
			wifi_reset();
		break;
		case WIFI_CHANGE:
			change_wifi();
		break;
		case RESET_MODE_TO_HOTSPOT:
			wifi_reset();
		break;
		case RESET_MODE_TO_ROUTER:
			wifi_reset();
		break;
		}
		send_msg();
	}

	/* check if circural buffor is not full and transfer data from to USART2 */
	if((USART2->SR & USART_SR_TC) && (ogon!=glowa)&&(ogon<1000)){
	  transfer_bufor();
	}

	/* update clock and calendar data for ui */
	pobierz_czas();

	/* update time left to sensor measure*/
	pobierz_alarm();

	/* update ui data */
	if(init_lv){
		if(!wifi_set){
			update_ui_data();
			lv_task_handler();
			dotyk(&hspi3);
		}
		/* in wi-fi change mode for loading screen and freeze UI */
		else{
			sprintf(wifi_change_bar_label, "%3d\%", wifi_change_bar);
		    lv_bar_set_value(ui_Bar11, wifi_change_bar, LV_ANIM_OFF);
		    lv_label_set_text(ui_Label62, wifi_change_bar_label);
			lv_task_handler();
			dotyk(&hspi3);
		}
	}

	/* check condition and turn on stanby mode */
	if((backlight==0)&&(PomiarFlag==1)){
		if(CheckAlarm()){
			sleep(120);
		}
	}

	/* in case wifi change, stop communication with sensor and check for available WLAN networks */
	if((wifi_list_send==1)&&(PomiarFlag==1)){
		wifi_list_send=0;
		ESP_Server_Init();
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) RxBuf, RxBuf_SIZE);
		HAL_UART_Transmit_DMA(&huart1, (uint8_t*) "AT+CWLAP\r\n",strlen("AT+CWLAP\r\n"));
	}

	/* turn on communication with ESP caused by sensor measure alarm */
	if(AlarmFlag){
		ESP_Server_Init();
		HAL_TIM_Base_Start(&htim4); //timer for ESP communication messages delays
		AlarmFlag=0;
		PomiarFlag=0;
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) RxBuf, RxBuf_SIZE);
	}

	/* in case wi-fi search change for loading screen for certain time */
	if(screen_change==1){
		_ui_screen_change(ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
		screen_change=3;
	}
	else if(screen_change==2){
		_ui_screen_change(ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
		screen_change=3;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
	//__HAL_RCC_PWR_CLK_ENABLE();
  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */



  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  // Check if device was woken up from Standby
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET) {
    // Check if wake up was caused by timer
    if(__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF)) {
      // Send "WUTF=1" through UART2 if flag is set
      HAL_UART_Transmit(&huart2, (uint8_t*) "WUTF=1\r\n", strlen("WUTF=1\r\n"), 500);
    } else {
      // Send "WUTF=0" through UART2 if flag is not set
      HAL_UART_Transmit(&huart2, (uint8_t*) "WUTF=0\r\n", strlen("WUTF=0\r\n"), 500);
    }

    // Check if wake up flag of the power interface is set
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU)) {
      // Send "WUF=1" through UART2 if flag is set
      HAL_UART_Transmit(&huart2, (uint8_t*) "WUF=1\r\n", strlen("WUF=1\r\n"), 500);
    } else {
      // Send "WUF=0" through UART2 if flag is not set
      HAL_UART_Transmit(&huart2, (uint8_t*) "WUF=0\r\n", strlen("WUF=0\r\n"), 500);
    }

    // Check if wake up was caused by alarm
    if(__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_ALRAF)) {
      // Send "ALRAF=1" through UART2 if flag is set
      HAL_UART_Transmit(&huart2, (uint8_t*) "ALRAF=1\r\n", strlen("ALRAF=1\r\n"), 500);
      /* If wake up was caused by measurement alarm turn
        on measurement flag and don't turn of backlight */
      PomiarFlag = 0;
      backlight = 0;
    } else {
      /* If wake up was not from alarm then it was from touching the screen,
        turn on backlight and don't turn on measurement */
      // Send "ALRAF=0" through UART2
      HAL_UART_Transmit(&huart2, (uint8_t*) "ALRAF=0\r\n", strlen("ALRAF=0\r\n"), 500);
      // Turn on the backlight
      HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, 1);
      // Set measurement flag and backlight
      PomiarFlag = 1;
      backlight = 1;
      // Enable alarm interrupt and set related configurations
      __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
      __HAL_RTC_ALARM_EXTI_ENABLE_IT();
      __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();
      __HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_ALRA);
      __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
    }

    // Clear StandBy flag of the power interface
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    // Read from backup data register (RTC_BKP_DR0)
    uint32_t dataco2_dust = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);

    // Split the read data into CO2 and Dust values
    uint32_t co2_raw = dataco2_dust >> 16;  // upper 16 bits for CO2
    uint32_t dust = dataco2_dust & 0x7FFF;           // lower 15 bits for dust

    // Convert data to strings
    sprintf(str_CO2, "%4u", co2_raw);
    sprintf(sensor_data_table[Dust_OUT-2].data_label, "%3u", dust);

    // Read data from other registers
    uint32_t dataw = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
    uint32_t datat = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
    uint32_t datap = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
    uint32_t datawt = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR18);

    // Split datawt into temperature and humidity
    measure_tab[Measure_IN].pomiar[Humi_IN].num = (uint8_t)(datawt >> 24);
    measure_tab[Measure_IN].pomiar[Humi_IN].tenths = (uint8_t)(datawt >> 16);
    measure_tab[Measure_IN].pomiar[Temp_IN].num = (uint8_t)(datawt >> 8);
    measure_tab[Measure_IN].pomiar[Temp_IN].tenths = (uint8_t)(datawt);

    // Read year, battery level and sensor battery level
    uint32_t batlvlrok = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR19);
    uint32_t rok = batlvlrok & 0xFFFF; // lower 16 bits for year
    uint32_t bat_lvl_pre = (batlvlrok >> 16) & 0xFF; // next 8 bits for battery level
    uint32_t bat_lvl_int = bat_lvl_pre;
    sensor_data_table[3].pomiar = (batlvlrok >> 24) & 0xFF; // highest 8 bits for sensor battery level
    // Convert data to strings
    sprintf(bat_lvl_lb, "%3u", bat_lvl_pre);
    sprintf(sensor_data_table[3].data_label, "%3u", sensor_data_table[3].pomiar);
    sprintf(str_temp_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[0].num, measure_tab[Measure_IN].pomiar[0].tenths);
    sprintf(str_hum_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[1].num, measure_tab[Measure_IN].pomiar[1].tenths);

    // Read SSID and password for saved wifi
    read_ssid();

    // Loop to assign data labels for Humidity, Temperature, and Pressure
    for (int i = 0; i < 4; i++) {
        sensor_data_table[Humi_OUT - 3].data_label[i] = (dataw >> (24 - 8 * i)) & 0xFF;
        sensor_data_table[Temp_OUT - 3].data_label[i] = (datat >> (24 - 8 * i)) & 0xFF;
        sensor_data_table[Pres_OUT - 3].data_label[i] = (datap >> (24 - 8 * i)) & 0xFF;
    }
    }

  // If device was turn on, not woken up from StandBy then just on backlight
  else{

	HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, 1);

	// read saved work mode and wi-fi credentials
	read_ssid();

	//__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	//__HAL_RTC_ALARM_DISABLE_IT(&hrtc, RTC_IT_ALRA);
	//__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);

	backlight=1;
  }

  return;
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 49999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 60000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 99;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 49999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 49999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 10000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 4999;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 20000;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 49999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 8000;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 49999;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 30000;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
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
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TCH_CS_Pin|BACKLIGHT_Pin|ESP_ENABLE_Pin|SPI2_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DC_Pin|LCD_RST_Pin|LCD_CS_Pin|ESP_RESET_Pin
                          |SD_RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_PC13_Pin */
  GPIO_InitStruct.Pin = LED_PC13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PC13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TCH_CS_Pin BACKLIGHT_Pin SPI2_CS_Pin */
  GPIO_InitStruct.Pin = TCH_CS_Pin|BACKLIGHT_Pin|SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin LCD_RST_Pin LCD_CS_Pin SD_RELAY_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_RST_Pin|LCD_CS_Pin|SD_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TCH_IRQ_Pin */
  GPIO_InitStruct.Pin = TCH_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TCH_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_RESET_Pin */
  GPIO_InitStruct.Pin = ESP_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_ENABLE_Pin */
  GPIO_InitStruct.Pin = ESP_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* LCD buffer DMA transfer done */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi1)
	{
		/* notify LVGL driver and disable LCD CS pin*/
		lcd_transfer_done();
	}
}

/* interrupt from display touch irq pin */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* if display was turned off enable backlight and varrible for LVGL diver enable in main loop */
	if(GPIO_Pin == TCH_IRQ_Pin){
		if(backlight == 0){

			/* if display was turned off turn on display and touch screen */
			backlight=1;
			HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(TCH_CS_GPIO_Port, TCH_CS_Pin, GPIO_PIN_SET);

			/* start backlight turn off timer again */
			__HAL_TIM_CLEAR_FLAG(&htim1, TIM_SR_UIF);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			HAL_TIM_Base_Start_IT(&htim1);

		}
		else
		{
			/* if display was touched when turned on reset turn off delay timer */
			touch_irq();
			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
	}
}

/* if after transfer to external USART2 buffer is full start from the begining */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2)
		{
		if(ogon==1000){
			ogon=0;
		}
		}
}

/* check for USART errors in case of debugging and reset USART1 if error occurred */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1)
		{
		__NOP();
		HAL_UART_DMAStop(&huart1);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) RxBuf, RxBuf_SIZE);
		}
	if (huart->Instance == USART2)
		{
		__NOP();
		}
}

/* handle timers interrupt */
/* tim1 timer screen backlight */
/* tim3 timer for us delay */
/* tim5 timer for */
/* tim9 timer for textbox color change */
/* tim10 timer for msg delay */        ///do usunięcia
/* tim11 timer for loading screen delay */

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim){


	/* tim1 timer for screen backlight turn screen off if delay with no touch elasped */
	if (htim->Instance == TIM1){
		HAL_TIM_Base_Stop_IT(&htim1);
		if(!wifi_set){
			backlight=0;
			HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, 0);
		}
	}

	/* timer for SCD41 communication delay after message sent */
	if (htim->Instance == TIM5){
		HAL_TIM_Base_Stop_IT(htim);
		uint16_t command = 0xec05;
		buf_co2_tr[0] = (uint8_t)(command >> 8);
		buf_co2_tr[1] = (uint8_t)command;
		one_shot_read=1;
		HAL_I2C_Master_Transmit_DMA(I2C_HANDLE, SCD40_ADDRESS, buf_co2_tr, sizeof(buf_co2_tr));
	}

	/* tim9 timer for textbox color change, switch textbox color back to normal*/
	if (htim->Instance == TIM9)
		{
	    if(init_lv){
		color = 0xFFFFFF;
		lv_obj_set_style_bg_color(ui_TextArea1, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui_TextArea2, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui_TextArea3, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui_TextArea4, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui_TextArea5, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
		HAL_TIM_Base_Stop_IT(htim);
	    }
		}

	/* timer 10 */
	if (htim->Instance == TIM10){
		HAL_TIM_Base_Stop_IT(htim);
	}

	/* tim11 timer for loading screen delay */
	if (htim->Instance == TIM11){
  	  	char label_print[264]={'\0'};
  	  	sprintf(wifi_list[indy-1], "**SWITCH TO HOTSPOT MODE");
  	  	for (int k = 0; k < indy; k++){
  	 	  	strcat(label_print, wifi_list[k]);
  	  	}
  	  	indy=0;
	    lv_dropdown_set_options(ui_Dropdown4, label_print);
	    screen_change=1;
		HAL_TIM_Base_Stop_IT(htim);
		nie_szuka_wifi=1;
		for(int i=0;i<33;i++){
			wifi_list[0][i]='\0';
		}
	}
}

/* interrupt from USART1 after line IDLE detect or buffor full
 * rewrite buffor to transmit MainBuf used for USART2 transfer */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
		{
		uint16_t remain = huart1.RxXferCount;
		int i;
		int temp;
		for(i=Size;i>0;i--){
				temp=RxMainBuf_SIZE-remain-i;
				MainBuf[temp]=RxBuf[temp];
			}
		glowa=RxMainBuf_SIZE-remain;
		}
}

/* interrupt from alarm event if device was awake, turn on sensor communication */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	AlarmFlag = 1;
}

/* Turn on message receive from SCD41 after demand for data sent */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(one_shot_read==1){
		HAL_I2C_Master_Receive_DMA(I2C_HANDLE, SCD40_ADDRESS, buf_co2, sizeof(buf_co2));
		one_shot_read=0;
	}
	__NOP();
}

/* I2C error handle for debugging */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	__NOP();
}

/* menage revieved measurement data from SCD41 sensor */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	  co2_raw = (buf_co2[0] << 8) | buf_co2[1];
	  uint16_t T_raw =(buf_co2[3] << 8) | buf_co2[4];
	  uint16_t H_raw = (buf_co2[6] << 8) | buf_co2[7];
	  float T_val = -45+175*(T_raw/65635.0);
	  float H_val = 100*(H_raw/65635.0);
	  measure_tab[Measure_IN].pomiar[0].num=floor(H_val);
	  measure_tab[Measure_IN].pomiar[0].tenths=(uint8_t)(modff(H_val, &H_val)*10);
	  measure_tab[Measure_IN].pomiar[1].num=(T_val);
	  measure_tab[Measure_IN].pomiar[1].tenths=(uint8_t)(modff(T_val, &T_val)*10);
	  measure_tab[Measure_IN].pomiar[2].num=(co2_raw)/100;
	  measure_tab[Measure_IN].pomiar[2].tenths=(co2_raw)%100;
	  sprintf(str_temp_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[0].num, measure_tab[Measure_IN].pomiar[0].tenths);
	  sprintf(str_hum_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[1].num, measure_tab[Measure_IN].pomiar[1].tenths);
	  sprintf(str_CO2, "%4u", co2_raw);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
