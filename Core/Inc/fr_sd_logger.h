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

void calculate_previous_day(RTC_DateTypeDef* current_date);
unsigned long calculate_3min_periods_elapsed(RTC_TimeTypeDef time_measure);
void measure_init();
void write_table_data();
void write_data_at_position(struct measure_type* measure_tab);
uint8_t read_data_from_position(int measure_ind, int data);

extern struct measure_type measure_tab[2];

#endif /* INC_SD_LOGGER_H_ */
