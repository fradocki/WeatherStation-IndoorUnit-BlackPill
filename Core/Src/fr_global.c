/*
 * fr_global.c
 *
 *  Created on: 22 cze 2023
 *      Author: Filip
 */
#include "fr_global.h"

uint8_t buf_co2_tr[2]={0};
uint16_t co2_raw=0;
uint8_t buf_co2[9]={0};
uint8_t iter_wifi_msg=0;
uint32_t tryb_wifi=0;
uint32_t gradient_dust;
uint32_t gradient_co2;
volatile uint8_t RxBuf[RxBuf_SIZE]="\0";
volatile uint8_t MainBuf[RxMainBuf_SIZE];
uint8_t flaga=1;
uint16_t ogon=0;
uint16_t glowa=0;
uint16_t znak=0;
touch touchtable[MAXSAMPLE];
volatile int touchsample=0;
volatile int touchsample1=0;
volatile XPT2046_State TouchState=0;
char str_temp[12];
char str_cis[12];
char str_wil[12];
char str_dust[6];
char str_CO2[6];
char str_temp_in[12];
char str_hum_in[12];
char czas_str[20];
char data_str[40];
char dzien[13];
char miesiac[12];
char bat_lvl_lb[7];
char bat_sen_lvl_lb[7];
uint16_t bat_sen_lvl;
char wifi_read_ssid[33]={'\0'};
char wifi_password[33];
char wifi_list[8][33]={'\0'};
char wifi_ssid[4][33]={'\0'};
char ip_set[20];
uint16_t indy;
uint8_t iter_sd_card=0;
uint8_t done=1;

RTC_TimeTypeDef time_set= {0};
RTC_DateTypeDef data_set= {0};
RTC_TimeTypeDef time_get= {0};
RTC_DateTypeDef data_get= {0};
RTC_DateTypeDef data_measure= {0};
RTC_TimeTypeDef time_measure= {0};

uint8_t backlight=0;
uint8_t AlarmFlag, PomiarFlag=0, BackLightFlag;
uint8_t screen_change=0;

long double TemperatureIN = 0;;
long double HumidityIN = 0;

uint16_t i1=0, flaga_szukania=0, rok=2000;
uint32_t color = 0xFFFFFF;
uint8_t init_lv=0;
uint8_t set_lv=0;
uint8_t wifi_set=0;
uint8_t wifi_list_send;
uint32_t dataw=0;
uint32_t datat=0;
uint32_t datap=0;
uint32_t datad;
uint16_t dust;
uint16_t AD_BAT;
uint32_t bat_lvl_pre, bat_lvl_pre_sensor;
uint8_t nie_szuka_wifi=1;
double bat_lvl;
uint32_t bat_lvl_int;
int16_t sensor_data_read[480]={0};
uint8_t buf[2];
uint8_t one_shot_read=0;
uint8_t channel;
uint8_t reset_iter=0;
int32_t wifi_change_bar=0;
char wifi_change_bar_label[6];

struct measure_type measure_tab[2];
struct polecenie tab_polecenie[14];
struct kolejka obecny={0,0};
struct search_letter_table sensor_data_table[5];


////////////////////////////////////SD CARD VARIBLES/////////////////////////////////////////////
FATFS fs;  // file system
FIL file; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // file read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char buffer_sd[BUFFER_SIZE];  // to store strings..

lv_indev_drv_t indev_drv;

// Mapping from measure_ind to file name
const char* measure_to_file_name[] = {
    [Humi_IN] = "hum_in",
    [Temp_IN] = "temp_in",
    [CO2_IN] = "co2_in",
    [Humi_OUT] = "hum_out",
    [Temp_OUT] = "temp_out",
    [Pres_OUT] = "pre_out",
    [Dust_OUT] = "dust_out"
};
