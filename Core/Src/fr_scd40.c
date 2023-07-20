/*
 * scd40.c
 *
 *  Created on: 27 kwi 2023
 *      Author: Filip
 */
#include <fr_scd40.h>



// Function to write command to SCD40 sensor
void SCD40_writecommand() {
    uint16_t command = 0x219d; // The command to be sent - measure single shot
    buf[0] = (uint8_t)(command >> 8); // Load the high byte into the buffer
    buf[1] = (uint8_t)command; // Load the low byte into the buffer

    // Transmit the command to the SCD40 sensor using DMA (Direct Memory Access)
    HAL_StatusTypeDef HAL1 = HAL_I2C_Master_Transmit_DMA(I2C_HANDLE, SCD40_ADDRESS, buf, 2);

    // Clear the Update Interrupt Flag of the timer
    __HAL_TIM_CLEAR_FLAG(&htim5, TIM_SR_UIF);

    // Start the timer in interrupt mode to wait 5000ms for data to be ready
    HAL_TIM_Base_Start_IT(&htim5);

}


// Function to generate a CRC-8 checksum for data integrity validation
uint8_t sensirion_common_generate_crc(uint8_t* data, uint16_t count) {
	uint16_t current_byte; // Byte counter
	uint8_t crc = CRC8_INIT; // CRC initialization
	uint8_t crc_bit; // Bit counter

	// Loop through each byte of data
	for (current_byte = 0; current_byte < count; ++current_byte) {
		// XOR current data byte with CRC
		crc ^= (data[current_byte]);

		// Loop through each bit in a byte
		for (crc_bit = 8; crc_bit > 0; --crc_bit) {
			// Check if leftmost bit is set
			if (crc & 0x80) {
				// Left shift CRC and XOR with polynomial
				crc = (crc << 1) ^ CRC8_POLYNOMIAL;
			} else {
				// Left shift CRC
				crc = (crc << 1);
			}
		}
	}

	// Return the computed CRC
	return crc;
}


// Function to get the color representation of air quality based on PPM value
uint32_t getAirQualityColor(int ppm) {
	unsigned int color1, color2; // Variables for color interpolation
	int range1, range2; // Variables for range interpolation

	// Map the given PPM value to the appropriate color and range
	if (ppm < 1000) {
		// PPM is less than 1000
		color1 = 0x00FF00;   // Green (RGB: 0, 255, 0)
		color2 = 0xFFFF00;   // Yellow (RGB: 255, 255, 0)
		range1 = 0;
		range2 = 50;
	} else if (ppm < 1500) {
		// PPM is between 1000 and 1500
		color1 = 0xFFFF00;   // Yellow (RGB: 255, 255, 0)
		color2 = 0xFFA500;   // Orange (RGB: 255, 165, 0)
		range1 = 50;
		range2 = 100;
	} else if (ppm < 2000) {
		// PPM is between 1500 and 2000
		color1 = 0xFFA500;   // Orange (RGB: 255, 165, 0)
		color2 = 0xFF0000;   // Red (RGB: 255, 0, 0)
		range1 = 100;
		range2 = 150;
	} else if (ppm < 2500) {
		// PPM is between 2000 and 2500
		color1 = 0xFF0000;   // Red (RGB: 255, 0, 0)
		color2 = 0x8B0000;   // Dark red (RGB: 139, 0, 0)
		range1 = 150;
		range2 = 200;
	} else {
		// PPM is more than 2500
		return 0x8B0000;   // Dark red (RGB: 139, 0, 0)
	}

	// Split color values into RGB components
	int r1 = (color1 >> 16) & 0xFF; // Red component of color1
	int g1 = (color1 >> 8) & 0xFF; // Green component of color1
	int b1 = color1 & 0xFF; // Blue component of color1
	int r2 = (color2 >> 16) & 0xFF; // Red component of color2
	int g2 = (color2 >> 8) & 0xFF; // Green component of color2
	int b2 = color2 & 0xFF; // Blue component of color2

	// Determine the proportion of the ppm value within its range
	int ppmInRange = ppm - range1;
	int totalRange = range2 - range1;

	// Interpolate color components
	int interpolatedR = r1 + ((r2 - r1) * ppmInRange) / totalRange; // Interpolated red component
	int interpolatedG = g1 + ((g2 - g1) * ppmInRange) / totalRange; // Interpolated green component
	int interpolatedB = b1 + ((b2 - b1) * ppmInRange) / totalRange; // Interpolated blue component

	// Combine interpolated color components into a single integer
	unsigned int interpolatedColor = (interpolatedR << 16) | (interpolatedG << 8) | interpolatedB;

	return interpolatedColor; // Return the interpolated color
}



