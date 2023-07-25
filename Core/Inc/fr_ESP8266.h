/*
 * ESP8266.h
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */

#ifndef INC_FR_ESP8266_H_
#define INC_FR_ESP8266_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fr_global.h"
#include "fr_scd40.h"
#include "fr_sd_logger.h"
#include "ui_helpers.h"
#include "fr_stm32.h"

void esp_reset();
void esp_server_init();
void esp_clear_buffer();
void esp_send(char *, int);
void transfer_bufor();
void search_msg_response(struct search_letter_table * sensor_data_table,uint16_t znak, int indb,int indc);
void send_msg();
void search_wifi(uint16_t i);
void extract_numbers_from_ip(char* ip_set, int* ip_liczby);
uint32_t get_color_gradient(int value);
void save_ssid(char * ssid, char * pass, char * ip_set);
void read_ssid();

#endif /* INC_FR_ESP8266_H_ */
