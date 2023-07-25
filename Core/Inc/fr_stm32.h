/*
 * fr_stm32.h
 *
 *  Created on: 22 cze 2023
 *      Author: Filip
 */

#ifndef INC_FR_STM32_H_
#define INC_FR_STM32_H_

#include <main.h>
#include "fr_global.h"
#include "ui.h"
#include "fr_ESP8266.h"
#include "fr_lv_drv_touch.h"

void get_time();
void sleep();
void set_next_alarm(void);
int check_alarm();
void get_alarm();
void update_ui_data();
void init_lvgl();

#endif /* INC_FR_STM32_H_ */
