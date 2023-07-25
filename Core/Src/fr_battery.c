/*
 * battery.c
 *
 *  Created on: 25 lut 2023
 *      Author: Filip
 */
#include <fr_battery.h>

extern ADC_HandleTypeDef hadc1;
extern double bat_lvl;
extern char bat_lvl_lb[7];
extern uint32_t bat_lvl_int;
extern uint32_t bat_lvl_pre;

// Measures the battery voltage and calculates the battery level
void measure_battery(){

    uint16_t AD_BAT;
    // The battery level data array
    uint16_t bat_lvl_tab[20];
    double voltage;
    // Measure battery level 20 times
    for(int i=0; i<20;i++){
        // Start ADC conversion
        HAL_ADC_Start(&hadc1);
        // Poll ADC for conversion
        HAL_ADC_PollForConversion(&hadc1, 20);
        // Get the converted value and add to total
        AD_BAT += HAL_ADC_GetValue(&hadc1);
        // Stop the ADC conversion
        HAL_ADC_Stop(&hadc1);
        // Store individual measurement to array
        bat_lvl_tab[i] = AD_BAT;
    }

    // Calculate the average of the 20 measurements
    AD_BAT /= 20;
    // Calculate the battery voltage based on the ADC value
    voltage = ((AD_BAT / 4096.0) * 3100) * 2;
    // Determine battery level based on voltage
    if(voltage > 4200){
        // If voltage is more than 4.2V, battery level is 100%
        bat_lvl = 100;
    }
    else if(voltage < 3300){
        // If voltage is less than 3.3V, battery level is 0%
        bat_lvl = 0;
    }
    else{
        // If voltage is between 3.3V and 4.2V, calculate battery level accordingly
        if(voltage > 3650){
            bat_lvl = ((((voltage - 3550.0) / (4200 - 3550)) * 80) / 1) + 20;
        }
        else{
            bat_lvl = ((((voltage - 3200.0) / (3650.0 - 3200)) * 20) / 1);
        }
    }
    // Convert battery level to integer
    bat_lvl_int = (uint32_t)bat_lvl;
    // If current battery level is higher than previous but the increase is less than 4 (and not zero), set it to previous level
    if((bat_lvl_int > bat_lvl_pre) && (bat_lvl_int - bat_lvl_pre < 4) && (bat_lvl_pre != 0)){
        bat_lvl_int = bat_lvl_pre;
    }
    // Format battery level to string
    sprintf(bat_lvl_lb, "%3u", bat_lvl_int);
}

