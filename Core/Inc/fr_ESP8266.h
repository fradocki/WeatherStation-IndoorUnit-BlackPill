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

#define WiFi_Credentials	"AT+CWJAP=\"NORMAN121\",\"uiyuiy121\"\r\n"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern uint32_t seconds;
extern uint8_t buffer[1000];
extern uint16_t buffer_index, timeout, messageHandlerFlag;


void ESP_RESET();
void ESP_Server_Init();
void ESP_Clear_Buffer();
void ESP_Send(char *, int);
void transfer_bufor();
void szukaj(struct search_letter_table * sensor_data_table,uint16_t znak);
void send_msg();
void szukaj_wifi(uint16_t i);
void extractNumbersFromIP(char* ip_set, int* ip_liczby);
uint32_t getColorGradient(int value);
void save_ssid(char * ssid, char * pass, char * ip_set);
void read_ssid();

#endif /* INC_FR_ESP8266_H_ */
