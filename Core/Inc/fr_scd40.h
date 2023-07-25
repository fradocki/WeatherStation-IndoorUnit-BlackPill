/*
 * scd40.h
 *
 *  Created on: 27 kwi 2023
 *      Author: Filip
 */

#ifndef SRC_SCD40_H_
#define SRC_SCD40_H_

#include <main.h>
#include <stdint.h>
#include "fr_global.h"

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xFF

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern uint8_t buf_co2_tr[2];
extern uint8_t buf_co2[9];
void SCD40_writecommand();
void SCD40_readdata(uint16_t command);
uint8_t sensirion_common_generate_crc(uint8_t* data, uint16_t count);
extern TIM_HandleTypeDef htim5;
extern uint8_t buf[2];
uint32_t get_air_quality_color(uint16_t ppm);

#endif /* SRC_SCD40_H_ */
