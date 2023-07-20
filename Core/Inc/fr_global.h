/*
 * fr_global.h
 *
 *  Created on: 22 cze 2023
 *      Author: Filip
 */
#ifndef INC_FR_GLOBAL_H_
#define INC_FR_GLOBAL_H_

#include <main.h>
#include "fatfs.h"
#include "fr_sd_handle.h"
#include "fr_touch.h"
#include "ui.h"

extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern RTC_HandleTypeDef hrtc;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi1_tx;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;



extern uint8_t buf_co2_tr[2];
extern uint16_t co2_raw;
extern uint8_t buf_co2[9];
extern uint8_t iter_wifi_msg;
extern uint32_t tryb_wifi;
extern uint32_t gradient_dust;
extern uint32_t gradient_co2;
extern volatile uint8_t RxBuf[RxBuf_SIZE];
extern volatile uint8_t MainBuf[RxMainBuf_SIZE];
extern uint8_t flaga;
extern uint16_t ogon;
extern uint16_t glowa;
extern uint16_t znak;
extern touch touchtable[MAXSAMPLE];
extern volatile int touchsample;
extern volatile int touchsample1;
extern volatile XPT2046_State TouchState;
extern char str_temp[12];
extern char str_cis[12];
extern char str_wil[12];
extern char str_dust[6];
extern char str_CO2[6];
extern char str_temp_in[12];
extern char str_hum_in[12];
extern char czas_str[20];
extern char data_str[40];
extern char dzien[13];
extern char miesiac[12];
extern char bat_lvl_lb[7];
extern char bat_sen_lvl_lb[7];
extern uint16_t bat_sen_lvl;
extern char wifi_read_ssid[33];
extern char wifi_password[33];
extern char wifi_list[8][33];
extern char wifi_ssid[4][33];
extern char ip_set[20];
extern uint16_t indy;
extern uint8_t iter_sd_card;
extern uint8_t done;

extern RTC_TimeTypeDef time_set;
extern RTC_DateTypeDef data_set;
extern RTC_TimeTypeDef time_get;
extern RTC_DateTypeDef data_get;
extern RTC_DateTypeDef data_measure;
extern RTC_TimeTypeDef time_measure;

extern uint8_t backlight;
extern uint8_t AlarmFlag, PomiarFlag, BackLightFlag;
extern uint8_t screen_change;

extern long double TemperatureIN;
extern long double HumidityIN;

extern uint16_t i1, flaga_szukania, rok;
extern uint32_t color;
extern uint8_t init_lv;
extern uint8_t set_lv;
extern uint8_t wifi_set;
extern uint8_t wifi_list_send;
extern uint32_t dataw;
extern uint32_t datat;
extern uint32_t datap;
extern uint32_t datad;
extern uint16_t dust;
extern uint16_t AD_BAT;
extern uint32_t bat_lvl_pre, bat_lvl_pre_sensor;;
extern uint8_t nie_szuka_wifi;
extern double bat_lvl;
extern uint32_t bat_lvl_int;
extern int16_t sensor_data_read[480];
extern uint8_t buf[2];
extern uint8_t one_shot_read;
extern uint8_t channel;
extern uint8_t reset_iter;
extern int32_t wifi_change_bar;
extern char wifi_change_bar_label[6];

extern struct measure_type measure_tab[2];
extern struct polecenie tab_polecenie[14];
extern struct kolejka obecny;
extern struct search_letter_table sensor_data_table[5];


////////////////////////////////////SD CARD VARIBLES/////////////////////////////////////////////
extern FATFS fs;  // file system
extern FIL file; // File
extern FILINFO fno;
extern FRESULT fresult;  // result
extern UINT br, bw;  // file read/write count

/**** capacity related *****/
extern FATFS *pfs;
extern DWORD fre_clust;
extern uint32_t total, free_space;

extern char buffer_sd[BUFFER_SIZE];  // to store strings..

extern lv_indev_drv_t indev_drv;

extern const char* measure_to_file_name[];

#endif /* INC_FR_GLOBAL_H_ */

