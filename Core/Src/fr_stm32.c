/*
 * fr_stm32.c
 *
 *  Created on: 22 cze 2023
 *      Author: Filip
 */
#include "fr_stm32.h"

// Function to get current time and date from RTC
void get_time() {

    // Get current time from RTC
    HAL_RTC_GetTime(&hrtc, &time_get, RTC_FORMAT_BIN);

    // Get current date from RTC
    HAL_RTC_GetDate(&hrtc, &data_get, RTC_FORMAT_BIN);

    // Store time as string in "HH:MM:SS" format
    sprintf(czas_str, "%02u:%02u:%02u", time_get.Hours, time_get.Minutes,time_get.Seconds);

    // Array of weekday names
    char *weekDays[7] = {"Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek", "Sobota", "Niedziela"};

    // Copy the appropriate weekday name
    strcpy(dzien, weekDays[data_get.WeekDay - 1]);

    // Array of month names
    char *months[12] = {"Styczen", "Luty", "Marzec", "Kwiecien", "Maj", "Czerwiec", "Lipiec", "Sieprien", "Wrzesien", "Pazdziernik", "Listopad", "Grudzien"};

    // Copy the appropriate month name
    strcpy(miesiac, months[data_get.Month - 1]);

    // Store date as string in "dayName DD MonthName YYYY" format
    sprintf(data_str, "%s %02u %s %u", dzien, data_get.Date, miesiac, (data_get.Year + rok));
}


// Function to put the device into standby mode
void sleep(){

    // Set the LED to high
	HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, GPIO_PIN_SET);

	// Send command to LCD
	lcd_cmd(0x10);

	// Enable access to the backup domain
    HAL_PWR_EnableBkUpAccess();

    // Initialize variables for storing sensor data
    uint32_t datawt= 0;
    uint32_t dataco2_dust=0;
    /*
    // save information about router mode on 16th lower bit of dust value since it's max value is 720
    if(tryb_wifi==ROUTER_MODE){
        sensor_data_table[Dust_OUT-2].pomiar=((sensor_data_table[Dust_OUT-2].pomiar & 0x7FFF)) | 0x8000;
    }
    else{
        sensor_data_table[Dust_OUT-2].pomiar=sensor_data_table[Dust_OUT-2].pomiar & 0x7FFF;
    }
    */
	// Combine CO2 raw data and dust data into one variable
    dataco2_dust = (co2_raw << 16) | sensor_data_table[Dust_OUT-2].pomiar;

	// Write combined data to backup register
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, dataco2_dust);

	// Combine sensor measurements into one variable
    datawt |= ((uint32_t)measure_tab[Measure_IN].pomiar[0].num) << 24;
    datawt |= ((uint32_t)measure_tab[Measure_IN].pomiar[0].tenths) << 16;
    datawt |= ((uint32_t)measure_tab[Measure_IN].pomiar[1].num) << 8;
    datawt |= ((uint32_t)measure_tab[Measure_IN].pomiar[1].tenths);

    // Variables for humidity, pressure, and temperature data
    uint32_t dataw = 0;
	uint32_t datap = 0;
	uint32_t datat = 0;

	// Combine sensor data labels into respective variables
    for(int i=0;i<4;i++){
    	dataw |= sensor_data_table[Humi_OUT-3].data_label[i] << (24-8*i);
    	datat |= sensor_data_table[Temp_OUT-3].data_label[i] << (24-8*i);
    	datap |= sensor_data_table[Pres_OUT-3].data_label[i] << (24-8*i);
    }

	// Write humidity, temperature, and pressure data to backup registers
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, dataw);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, datat);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, datap);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR18, datawt);

    // Combine battery level, base year into one variable
    uint32_t bat_lvl_rok= ((sensor_data_table[3].pomiar) << 24) | (bat_lvl_int << 16) | (rok);

    // Write battery level data to backup register
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR19, bat_lvl_rok);

	// Save SSID information
	save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);

	// Clear RTC wakeup, power wakeup, and RTC alarm flags
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

	// Disable access to the backup domain
    HAL_PWR_DisableBkUpAccess();

	// Enable wakeup pin
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	// Enter standby mode
	HAL_PWR_EnterSTANDBYMode();
}


// Function to set the next alarm
void set_next_alarm(void)
{
    RTC_AlarmTypeDef sAlarm = {0}; // RTC alarm structure to store the next alarm setting

    // If the current time is 23:59, set the alarm hour to 0 (midnight)
    // Otherwise, set the alarm hour to the current hour plus 1
    if((time_get.Hours + ((time_get.Minutes + 1) / 60)) == 24){
        sAlarm.AlarmTime.Hours = 0;
    }
    else{
        sAlarm.AlarmTime.Hours = time_get.Hours + ((time_get.Minutes + 1) / 60);
    }

    // Set the alarm minute to the current minute plus 1, wrapping around to 0 if it exceeds 59
    sAlarm.AlarmTime.Minutes = (time_get.Minutes + 1) % 60;

    // Set the alarm seconds to the current seconds
    sAlarm.AlarmTime.Seconds = time_get.Seconds;

    // Set the sub-seconds, daylight saving, store operation, alarm mask, sub-second mask, weekday select, date, and alarm A
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = data_get.Date;
    sAlarm.Alarm = RTC_ALARM_A;

    // Set the alarm
    // If setting the alarm fails, call the error handler
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}


// Function to check if alarm will be within 5s to not go into standby mode
int check_alarm(){
    RTC_AlarmTypeDef sAlarm = {0}; // RTC alarm structure to store the current alarm setting
    HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN); // Get the current alarm setting

    HAL_RTC_GetTime(&hrtc, &time_get, RTC_FORMAT_BIN); // Get the current time

    // If current hour is the alarm hour
    if(sAlarm.AlarmTime.Hours == time_get.Hours){
        // If current time is within one minute ahead of alarm time and more than 5 seconds left
        // or if current time is within one minute behind of alarm time and less than 55 seconds left
        if((((sAlarm.AlarmTime.Minutes - time_get.Minutes) < 1) &&
            ((sAlarm.AlarmTime.Seconds - time_get.Seconds) > 5))
            ||
            (((sAlarm.AlarmTime.Minutes - time_get.Minutes) == 1) &&
            ((sAlarm.AlarmTime.Seconds - time_get.Seconds) < 55))){
            return 1; // Return 1 to indicate that alarm will be within 5s
        }
    }
    else if(((time_get.Minutes - sAlarm.AlarmTime.Minutes) == 58) &&
          ((time_get.Seconds - sAlarm.AlarmTime.Seconds) < 55)){
            return 1; // Return 1 to indicate that alarm will be within 5s
    }
    else{
        return 0; // Return 0 to indicate that it's not time to ring the alarm yet
    }
}


// Function to fetch alarm from RTC and display time remaining for the next alarm
void get_alarm(){
    // If Alarm A interrupt is enabled and measurement flag is set
    if(((RTC->CR)&RTC_CR_ALRAIE) && PomiarFlag){
        char alarm_left[10]; // Array to store the time left for the next alarm in string format
        RTC_AlarmTypeDef alarm_get; // RTC alarm structure to store the current alarm setting
        uint32_t sec_time; // Variable to store the current time in seconds
        uint32_t sec_alarm; // Variable to store the alarm time in seconds
        uint32_t sec_left; // Variable to store the time left for the next alarm in seconds

        // Get the current alarm setting
        HAL_RTC_GetAlarm(&hrtc, &alarm_get, RTC_ALARM_A, RTC_FORMAT_BIN);

        // Convert current time to seconds
        sec_time = time_get.Hours*3600 + time_get.Minutes*60 + time_get.Seconds;

        // Convert alarm time to seconds
        sec_alarm = alarm_get.AlarmTime.Hours*3600 + alarm_get.AlarmTime.Minutes*60 + alarm_get.AlarmTime.Seconds;

        // Calculate the time left for the next alarm
        if(sec_alarm < sec_time){
            sec_left = 86400 - sec_time + sec_alarm; // Alarm time is in the next day
        }
        else{
            sec_left = sec_alarm - sec_time; // Alarm time is later in the same day
        }

        // Convert the time left for the next alarm to string format
        sprintf(alarm_left, "%d:%d", sec_left/60, sec_left%60);

        // Display the time left for the next alarm on the UI
        lv_label_set_text(ui_Label7, alarm_left);
    }
    else{
        // If Alarm A interrupt is not enabled or measurement flag is not set, display "pomiar" on the UI
        lv_label_set_text(ui_Label7, "pomiar");
    }
}

// Function to update UI elements
void update_ui_data() {
    // Updating labels with sensor values
    lv_label_set_text(ui_Screen1_Label9, str_temp_in); // set indoor temperature label
    lv_label_set_text(ui_Screen1_Label10, str_hum_in); // set indoor humidity label
    lv_label_set_text(ui_Label10, str_CO2); // set CO2 label

    lv_label_set_text(ui_Screen1_Label6, sensor_data_table[Humi_OUT-3].data_label); // set outdoor humidity label
    lv_label_set_text(ui_Screen1_Label4, sensor_data_table[Temp_OUT-3].data_label); // set outdoor temperature label
    lv_label_set_text(ui_Screen1_Label5, sensor_data_table[Pres_OUT-3].data_label); // set pressure label
    lv_label_set_text(ui_Label15, sensor_data_table[Dust_OUT-2].data_label); // set dust level label

    // Updating labels with current time and date
    lv_label_set_text(ui_Screen1_Label13, czas_str); // set time label
    lv_label_set_text(ui_Screen1_Label14, data_str); // set date label

    // Updating progress bars with sensor values
    lv_bar_set_value(ui_Bar3, dust, LV_ANIM_OFF); // set dust level progress bar value
    lv_obj_set_style_bg_color(ui_Bar3, lv_color_hex(gradient_dust), LV_PART_INDICATOR | LV_STATE_DEFAULT); // set dust level progress bar color

    gradient_co2 = get_air_quality_color(co2_raw); // get CO2 color based on current CO2 level
    lv_bar_set_value(ui_Bar1, co2_raw, LV_ANIM_OFF); // set CO2 progress bar value
    lv_obj_set_style_bg_color(ui_Bar1, lv_color_hex(gradient_co2), LV_PART_INDICATOR | LV_STATE_DEFAULT); // set CO2 progress bar color

    lv_bar_set_value(ui_Bar2, bat_lvl_int, LV_ANIM_OFF); // set internal battery level bar value
    lv_label_set_text(ui_Label4, bat_lvl_lb); // set internal battery level label

    lv_bar_set_value(ui_Bar4, sensor_data_table[3].pomiar, LV_ANIM_OFF); // set sensor battery level bar value
    lv_label_set_text(ui_Label28, sensor_data_table[3].data_label); // set sensor battery level label

}

void init_lvgl()
{
    // Asign value to indicate that LVGL was initialized
    init_lv = 2;

    // Initialize the LCD display
    lcd_init();

    // Initialize LVGL library
    lv_init();

    // Initialize the display with id 0
    display_init(0);

    // Get current time to show on initial screen
    get_time();

    // Initialize the User Interface (UI)
    ui_init();

    // Initialize the input device driver structure
    lv_indev_drv_init(&indev_drv);

    // Set the type of the input device to pointer type
    indev_drv.type = LV_INDEV_TYPE_POINTER;

    // Assign a read function for the touch screen device driver
    indev_drv.read_cb = lv_xpt_2064_read;

    // Register the input device driver to the LVGL library
    lv_indev_drv_register(&indev_drv);
}

