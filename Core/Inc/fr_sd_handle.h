/*
 * sd_handle.h
 *
 *  Created on: 24 lut 2023
 *      Author: Filip
 */

#ifndef INC_SD_HANDLE_H_
#define INC_SD_HANDLE_H_

#define BUFFER_SIZE 128
#include "fatfs.h"
#include "fr_global.h"

void sd_mount(void);
void sd_free_space(void);
void sd_write(void);
void sd_read(void);
void send_uart (char *);
int mount_sd_card();
uint8_t processData(int measure_ind, struct sensor_table * sensor_data_read_temp);

#endif /* INC_SD_HANDLE_H_ */
