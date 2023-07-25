/*
 *  eeprom.h
 *
 *  Created on: 17 sty 2023
 *      Author: Filip
 */

#ifndef INC_FR_EEPROM_H_
#define INC_FR_EEPROM_H_

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "fr_global.h"

uint8_t eeprom_write_char(uint16_t page, uint8_t *data);
uint8_t eeprom_read_char(uint16_t page, uint8_t *data, uint16_t size);


#endif /* INC_FR_EEPROM_H_ */
