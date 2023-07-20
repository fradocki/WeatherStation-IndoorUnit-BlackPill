/*
 * lcd.c
 *
 *  Created on: Sep 14, 2022
 *      Author: Filip
 */

#include "fr_lcd_lvgl_touch.h"
#include "fr_lcd.h"
#include "stdint.h"

extern lv_disp_drv_t disp_drv;

// Function to send command to the LCD
void lcd_cmd(uint8_t cmd)
{
    // Set the command mode by resetting DC pin
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    // Select the device by resetting CS pin
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    // Transmit the command using SPI
    HAL_StatusTypeDef status;
    status = HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);

    // Deselect the device by setting CS pin
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

    // Delay to allow the LCD to process the command
    HAL_Delay(1);
}

// Function to send data to the LCD
static void lcd_data(uint8_t data)
{
    // Set the data mode by setting DC pin
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    // Select the device by resetting CS pin
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    // Transmit the data using SPI
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);

    // Deselect the device by setting CS pin
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}


static void lcd_send(uint16_t value)
{
	if (value & 0x100) {
		lcd_cmd(value);
	} else {
		lcd_data(value);
	}
}

// Defines a macro CMD(x) for LCD command encoding.
// This operation sets the highest bit in the 9-bit value
#define CMD(x)			((x) | 0x100)


// Array of initialization commands and data for the LCD screen
static const uint16_t init_table[] = {

    // Each entry in this array consists of a command followed by associated data
    CMD(0xCB), 0x39, 0x2C, 0x00, 0x34, 0x02, // Command 0xCB with data
    CMD(0xCF), 0x00, 0xC1, 0x30,               // Command 0xCF with data
    CMD(0xE8), 0x85, 0x00, 0x78,               // Command 0xE8 with data
    CMD(0xEA), 0x00, 0x00,                     // Command 0xEA with data
    CMD(0xED), 0x64, 0x03, 0x12, 0x81,         // Command 0xED with data
    CMD(0xF7), 0x20,                           // Command 0xF7 with data
    CMD(0xC0), 0x23,                           // Command 0xC0 with data
    CMD(0xC1), 0x10,                           // Command 0xC1 with data
    CMD(0xC5), 0x3E, 0x28,                     // Command 0xC5 with data
    CMD(0xC7), 0x86,                           // Command 0xC7 with data
    CMD(0x36), 0x48,                           // Command 0x36 with data
    CMD(0x3A), 0x55,                           // Command 0x3A with data
    CMD(0xB1), 0x00, 0x18,                     // Command 0xB1 with data
    CMD(0xB6), 0x08, 0x82, 0x27,               // Command 0xB6 with data
    CMD(0xF2), 0x00,                           // Command 0xF2 with data
    CMD(0x26), 0x01,                           // Command 0x26 with data
    CMD(0xE0), 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,  // Command 0xE0 with data
    CMD(0xE1), 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,  // Command 0xE1 with data
    CMD(0x11)  // Command 0x11

};

// Function to initialize the LCD display
void lcd_init(void) {
    // Define an integer for loop iterations
    int i;

    // Reset the LCD display by toggling the reset pin
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);   // Enable
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET); // Disable
    HAL_Delay(10); // Delay for 10 milliseconds

    // Initiate communication with the LCD by toggling the chip select pin
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);   // Select
    HAL_Delay(10); // Delay for 10 milliseconds
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);   // Enable
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);     // Deselect

    // Software reset the LCD
    lcd_cmd(0x01);
    HAL_Delay(10); // Delay for 10 milliseconds

    // Send the initial configuration data to the LCD
    for (i = 0; i < sizeof(init_table) / sizeof(uint16_t); i++) {
        lcd_send(init_table[i]);
    }

    // Turn on the display (exit sleep mode)
    lcd_cmd(0x29);
    HAL_Delay(100); // Delay for 100 milliseconds

    // Set orientation
    lcd_cmd(0x36); // Memory Access Control command
    HAL_Delay(1);  // Delay for 1 millisecond
    lcd_data(0x20 | 0x08); // Set bits for orientation
}

// Function to send 16-bit data to the LCD
static void lcd_data16(uint16_t value) {
    // Send the high byte
    lcd_data(value >> 8);
    // Send the low byte
    lcd_data(value);
}

// Helper function to send x or y coordinates to the LCD
static void send_coords_to_lcd(uint8_t cmd, int coord1, int coord2) {
    uint8_t buffer[4];

    // Convert coordinates to a byte array
    buffer[0] = coord1 >> 8;
    buffer[1] = coord1;
    buffer[2] = coord2 >> 8;
    buffer[3] = coord2;

    // Send the command to the LCD
    lcd_cmd(cmd);

    // Send each byte of the buffer to the LCD
    for(int i = 0; i < 4; i++) {
        lcd_data(buffer[i]);
    }
}

// Set the drawing window of the LCD
static void lcd_set_window(int x1, int y1, int x2, int y2) {
    // Set the x coordinates of the drawing window
    send_coords_to_lcd(0x2A, x1, x2);

    // Set the y coordinates of the drawing window
    send_coords_to_lcd(0x2B, y1, y2);

    // Start writing to the GRAM (Graphic RAM)
    lcd_cmd(0x2C);
}

// Function to copy a frame buffer onto the LCD screen
void lcd_copy(int x1, int x2, int y1, int y2, uint16_t * DISP_fb) {
    // Calculate the width and height of the area to copy
    int width = x2 - x1 + 1;
    int height = y2 - y1 + 1;

    // Set the drawing window on the LCD screen
    lcd_set_window(x1, y1, x2, y2);

    // Set the data/command line to data mode
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);

    // Pull the chip select line low to start communication
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    // Transmit the frame buffer data to the LCD screen using DMA
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*) DISP_fb, 2 * width * height);
}

// Function to mark the end of a LCD data transfer operation
void lcd_transfer_done(void)
{
    // Notify the LittlevGL library that the flush operation is complete
    lv_disp_flush_ready(&disp_drv);

    // Set the chip select pin to High to end the SPI communication
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

// Function to check if the LCD is busy with an ongoing transfer operation
bool lcd_is_busy(void)
{
    // Return true if SPI communication is ongoing; otherwise return false
	if (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY)
		return true;
	else
		return false;
}


