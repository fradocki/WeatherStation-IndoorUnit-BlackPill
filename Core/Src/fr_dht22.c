/*
 * dht22.c
 *
 *  Created on: Oct 12, 2022
 *      Author: Filip
 */
#include "fr_dht22.h"

// Delays a given amount of us.
void delay(uint32_t time){
    // Set timer counter to zero
    __HAL_TIM_SET_COUNTER(&htim3,0);
    // Wait until timer counter reaches time
    while (TIM3->CNT < time){}
}

// Sets a specific GPIO pin as output
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

// Sets a specific GPIO pin as input
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

// Initializes DHT22 sensor
void DHT22_Start (void){
    Set_Pin_Output(DHT22_PORT, DHT22_PIN); // set the pin as output
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0); // pull the pin low
    delay(1200); // wait for more than 1ms
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1); // pull the pin high
    delay(40); // wait for 30us
    Set_Pin_Input(DHT22_PORT, DHT22_PIN); // set as input
}

// Checks the response of the DHT22 sensor
uint8_t DHT22_Check_Response (void){
    uint8_t response = 0;
    delay(40); // wait for 40us
    if (!HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) { // if the pin is low
        delay(80); // wait for 80us
        if (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) // if the pin is high, response is ok
            response = 1;
        else
            response = -1;
    }
    while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)); // wait for the pin to go low
    return response;
}

// Reads a byte of data from DHT22 sensor
uint8_t DHT22_Read (void){
    uint8_t i, j;
    for (j = 0; j < 8; j++){
        while (!HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)); // wait for the pin to go high
        delay(35); // wait for 40 us
        if (!HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) // if the pin is low
            i &= ~(1 << (7 - j)); // write 0
        else
            i |= (1 << (7 - j)); // if the pin is high, write 1
        while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)); // wait for the pin to go low
    }
    return i;
}

// Measures temperature and humidity with DHT22 sensor
void DHT22_Measure(void){
    uint8_t rh_byte1, rh_byte2, temp_byte1, temp_byte2;
    uint8_t presence = 0;
    long double sum, rh, temp;

    HAL_SuspendTick(); // Suspend tick increment
    DHT22_Start(); // Start DHT22 sensor
    presence = DHT22_Check_Response(); // Check sensor response
    rh_byte1 = DHT22_Read();
    rh_byte2 = DHT22_Read();
    temp_byte1 = DHT22_Read();
    temp_byte2 = DHT22_Read();
    sum = DHT22_Read(); // Read checksum
    HAL_ResumeTick(); // Resume tick increment

    temp = ((temp_byte1 << 8) | temp_byte2); // Merge temperature bytes
    rh = ((rh_byte1 << 8) | rh_byte2); // Merge humidity bytes
    // Convert the 16-bit raw values to temperature and humidity
    TemperatureIN = (float) (temp / 10.0);
    HumidityIN = (float) (rh / 10.0);

    measure_tab[Measure_IN].pomiar[0].num=floor(HumidityIN);
	measure_tab[Measure_IN].pomiar[0].tenths=(uint8_t)(modff(HumidityIN, &HumidityIN)*10);
	measure_tab[Measure_IN].pomiar[1].num=(TemperatureIN);
	measure_tab[Measure_IN].pomiar[1].tenths=(uint8_t)(modff(TemperatureIN, &HumidityIN)*10);
	sprintf(str_temp_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[0].num, measure_tab[Measure_IN].pomiar[0].tenths);
	sprintf(str_hum_in, "%2u.%1u", measure_tab[Measure_IN].pomiar[1].num, measure_tab[Measure_IN].pomiar[1].tenths);
	//strncpy(measure_tab[Measure_IN].pomiar[1],str_temp_in,4);
	//sprintf(print, "ind: %d, UART1:RxXferCount: %d\r\n\r\n", ind, (int) huart1.RxXferCount);
	HAL_RTC_GetTime(&hrtc, &time_measure, RTC_FORMAT_BIN);
	writeDataAtPosition(measure_tab);
	//lv_chart_series_t * ui_Chart2_series_1 = lv_chart_add_series(ui_Chart2, lv_color_hex(0x808080), LV_CHART_AXIS_SECONDARY_Y);
	//static lv_coord_t ui_Chart1_series_1_array[97];
	//lv_chart_set_ext_y_array(ui_Chart1, ui_Chart1_series_1, ui_Chart1_series_1_array);
}

