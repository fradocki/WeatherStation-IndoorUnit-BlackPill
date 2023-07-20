/*
 * sd_logger.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Filip
 */

#ifndef INC_SD_LOGGER_H_
#define INC_SD_LOGGER_H_

#include "main.h"
#include "fatfs.h"
#include "fr_sd_handle.h"
#include "string.h"
#include "fr_global.h"

extern RTC_TimeTypeDef time_measure;
extern FATFS fs;
extern FIL file;
extern FILINFO fno;
extern FRESULT fresult;
extern UINT br, bw;
extern FATFS* pfs;
extern DWORD fre_clust;
extern uint32_t total, free_space;
extern int16_t sensor_data_read[480];
extern uint16_t rok;
extern RTC_DateTypeDef data_measure;
extern RTC_HandleTypeDef hrtc;
extern lv_coord_t ui_Chart1_series_1_array[96];

void calculate_previous_day(RTC_DateTypeDef* current_date);
unsigned long calculate3MinPeriodsElapsed(RTC_TimeTypeDef time_measure);
void measure_init();
void writeTableData();
void writeDataAtPosition(struct measure_type* measure_tab);
uint8_t readDataFromPosition(int measure_ind, int data);

extern struct measure_type measure_tab[2];

#endif /* INC_SD_LOGGER_H_ */
