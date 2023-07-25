/*
 * eeprom.c
 *
 *  Created on: 17 sty 2023
 *      Author: Filip
 */

#include <fr_eeprom.h>
#include "string.h"
#include "math.h"

extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1
#define EEPROM_ADDR 0xA0

// This function writes data to the EEPROM
// Returns an error code - 0 means no errors, anything above 0 means there were issues.
uint8_t eeprom_write_char(uint16_t page, uint8_t *data) {
    uint16_t dataSize = strlen(data);   // Calculate the length of the data
    uint16_t startPage = page;           // Define the starting page
    uint16_t endPage = page + (dataSize / 17);   // Calculate the end page
    uint16_t numberOfPages = endPage - startPage + 1;  // Calculate the number of pages needed
    uint8_t errorCount = 0;              // Error counter
    HAL_StatusTypeDef status;            // Status variable for I2C operations

    if (endPage > 64) {                  // If end page exceeds EEPROM size, raise an error
        errorCount = 65;
    } else {
        for(int i = 0; i < numberOfPages; i++){
            uint16_t memAddress = (startPage + i) * 16;
            uint16_t eeprom = EEPROM_ADDR | (((memAddress) >> 3) & (0b00000110));
            // Write to the EEPROM. If unsuccessful, increment the error count.
            if((status = HAL_I2C_Mem_Write(EEPROM_I2C, eeprom, memAddress, 1, &data[i * 16], 16, 1000)) != HAL_OK){
                errorCount++;
            }
            HAL_Delay(5);  // delay after operation
        }
    }

    return errorCount;
}

// This function reads data from the EEPROM
// Returns an error code - 0 means no errors, anything above 0 means there were issues.
uint8_t eeprom_read_char(uint16_t page, uint8_t *data, uint16_t dataSize) {
    uint16_t startPage = page;           // Define the starting page
    uint16_t endPage = page + (dataSize / 17);   // Calculate the end page
    uint16_t numberOfPages = endPage - startPage + 1;  // Calculate the number of pages needed
    uint8_t errorCount = 0;              // Error counter
    HAL_StatusTypeDef status;            // Status variable for I2C operations

    if (endPage > 64) {                  // If end page exceeds EEPROM size, raise an error
        errorCount = 65;
    } else {
        for(int i = 0; i < numberOfPages; i++){
            uint16_t memAddress = (startPage + i) * 16;
            uint16_t eeprom = EEPROM_ADDR | (((memAddress) >> 3) & (0b00000110));
            // Read from the EEPROM. If unsuccessful, increment the error count.
            if((status = HAL_I2C_Mem_Read(EEPROM_I2C, eeprom, memAddress, 1, &data[i * 16], 16, 1000)) != HAL_OK){
                errorCount++;
            }
            HAL_Delay(5);  // delay after operation
        }
    }

    return errorCount;
}

