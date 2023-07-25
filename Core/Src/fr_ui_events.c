// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.0
// LVGL VERSION: 8.3.3
// PROJECT: SquareLine_Project_POPRAWKI

#include "ui.h"

// Function to load text from user interface and process it
void laduj_tekst(lv_event_t * e)
{
    RTC_TimeTypeDef time_set= {0};
    RTC_DateTypeDef data_set= {0};
    // Get date and time from text areas in user interface
    data_set.Date = (uint8_t) atoi(lv_textarea_get_text(ui_TextArea1));
    data_set.Month = (uint8_t) atoi(lv_textarea_get_text(ui_TextArea2));
    uint16_t rok_temp = atoi(lv_textarea_get_text(ui_TextArea3));
    time_set.Hours = (uint8_t) atoi(lv_textarea_get_text(ui_TextArea4));
    time_set.Minutes = (uint8_t) atoi(lv_textarea_get_text(ui_TextArea5));
    data_set.Year = rok_temp-rok;

    // Define month days
    uint8_t dni[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if(data_set.Year % 400 == 0 || (data_set.Year % 100 != 0 && data_set.Year % 4 == 0)){
        dni[1] = 29;
    }

    // Check if the date and time values are legitimate
    uint8_t legit = isDateTimeLegitimate(data_set, time_set, dni, rok_temp);

    if(legit){
        // Calculate weekday and set RTC (Real Time Clock)
        data_set.WeekDay = calculateWeekDay(data_set.Date, data_set.Month, data_set.Year);
        HAL_RTC_SetTime(&hrtc, &time_set, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &data_set, RTC_FORMAT_BIN);

        // Handle PomiarFlag
        if(PomiarFlag){
            get_time();
            set_next_alarm();
        }

        // Update UI with green color indicating success
        updateTextColor(0x00FF00); // Green color
        // Start timer
        __HAL_TIM_CLEAR_FLAG(&htim9, TIM_SR_UIF);
        HAL_TIM_Base_Start_IT(&htim9);
    }
    else{
        // Update UI with red color indicating failure
        updateTextColor(0xFF0000); // Red color
        // Start timer
        __HAL_TIM_CLEAR_FLAG(&htim9, TIM_SR_UIF);
        HAL_TIM_Base_Start_IT(&htim9);
    }
}

// Check if the date and time values are legitimate
uint8_t isDateTimeLegitimate(RTC_DateTypeDef data_set, RTC_TimeTypeDef time_set, uint8_t * dni, uint16_t rok_temp){
    if(data_set.Month < 13 && data_set.Date <= dni[data_set.Month-1] && time_set.Minutes < 60 && time_set.Hours < 24 && rok_temp > 1999){
        return 1;
    }
    else{
        return 0;
    }
}

// Calculate weekday
uint8_t calculateWeekDay(uint8_t d, uint8_t m, uint16_t y) {
    return ( (d+=(m<3?y--:(y-2))), (23*m/9+d+4+y/4-y/100+y/400) ) % 7;
}

// Update text color
void updateTextColor(uint32_t color){
    lv_obj_set_style_bg_color(ui_TextArea1, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TextArea2, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TextArea3, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TextArea4, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TextArea5, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Calculate and return the min and max of the sensor readings
void calculateMinMax(int* min, int* max, int* min2) {
    *min = 10000;
    *max = 0;
    *min2 = 10001;
    for (int i=0; i<96; i++){
        if(sensor_data_read[i]>*max) {
            *max=sensor_data_read[i];
        }
        if(sensor_data_read[i]<*min) {
            *min2= *min;
            *min=sensor_data_read[i];
        }
        else if((sensor_data_read[i]<*min2)&&(sensor_data_read[i]!=*min)) {
            *min2=sensor_data_read[i];
        }
    }
    if(*min==*max) {
        *min2=*min;
    }
    else if(((*min2-*min)>5)||(*min2-*min)<-5){
        *min=*min2;
    }
}

// This function calculates range parameters based on input min, max and val1, val2
void calculateRangeParameters(int* rangemin, int* rangemax, uint16_t* major_cnt, int min, int max, int val1, int val2){
    *rangemin = ((min-val1)/val1)*val1; // Calculate rangemin
    *rangemax = ((max+val1)/val1)*val1; // Calculate rangemax
    int range = *rangemax - *rangemin;  // Calculate range
    *major_cnt = (range/val2)+1;        // Calculate major count
}

// This function calculates range parameters specifically for CO2_IN
void calculateRangeParametersForCO2(int* rangemin, int* rangemax, uint16_t* major_cnt, int min, int max){
    int range = max-min; // Calculate range
    // Determine parameters based on range
    if(range<500){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 50, 50);
    }
    else if(range<1000){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 100, 100);
    }
    else if(range<2000){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 200, 200);
    }
    else{
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 500, 500);
    }
}

// This function calculates range parameters specifically for Dust_OUT
void calculateRangeParametersForDustOut(int* rangemin, int* rangemax, uint16_t* major_cnt, int min, int max){
    int range = max-min; // Calculate range
    // Determine parameters based on range
    if(range<100){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 5, 5);
    }
    else if(range<500){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 50, 50);
    }
    else{
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 100, 100);
    }
}

// Set the chart's range and ticks for y-axis based on calculated values
void setChartRange(uint16_t pomiar, int rangemin, int rangemax, uint16_t minor_cnt, uint16_t major_cnt, uint16_t divison_lines) {
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, rangemin, rangemax);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, major_cnt, minor_cnt, true, 50);
    lv_chart_set_div_line_count(ui_Chart1, divison_lines, 24);
}

// Hide or display the series in the chart based on the 'rysuj' variable
void manageChartSeries(uint8_t rysuj) {
    for(int i=0; i<96; i++) {
        if(rysuj) {
            lv_chart_hide_series(ui_Chart1, ui_Chart1_series_1, 0);
            ui_Chart1_series_1_array[i]=sensor_data_read[i];
        }
        else {
            lv_chart_hide_series(ui_Chart1, ui_Chart1_series_1, 1);
        }
    }
}

// This function determines the range, minor count, major count, and division lines based on the value of 'pomiar'
void determineParameters(uint16_t pomiar, int min, int max, int* rangemin, int* rangemax, uint16_t* minor_cnt, uint16_t* major_cnt, uint16_t* divison_lines) {
    // Determine parameters based on pomiar
    uint8_t decimalFactor=1;
    if(pomiar==Pres_OUT){
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 5, 5);
        *minor_cnt=5;
        decimalFactor=1;
    }
    else if(pomiar==CO2_IN){
        calculateRangeParametersForCO2(rangemin, rangemax, major_cnt, min, max);
        *minor_cnt=2;
    }
    else if(pomiar==Dust_OUT){
        calculateRangeParametersForDustOut(rangemin, rangemax, major_cnt, min, max);
        *minor_cnt=2;
    }
    else{
        calculateRangeParameters(rangemin, rangemax, major_cnt, min, max, 50, 50);
        *minor_cnt= (*major_cnt > 10) ? 5 : 2;
        decimalFactor=10;
    }
    *divison_lines = (*major_cnt * *minor_cnt) - 1; // Calculate division lines
    // Set range and division lines for the chart
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, (*rangemin)/decimalFactor, (*rangemax)/decimalFactor );
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, *rangemin, *rangemax );
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, *major_cnt, *minor_cnt, true, 50);
    lv_chart_set_div_line_count(ui_Chart1, *divison_lines, 24);
}

// Function to set chart based on measurement data
void rysuj_wielkosc(lv_event_t * e) {
    uint16_t pomiar = lv_dropdown_get_selected(ui_Dropdown2);
    uint16_t data = lv_dropdown_get_selected(ui_Dropdown1);
    uint8_t rysuj = read_data_from_position(pomiar, data);

    int min, max, min2;
    calculateMinMax(&min, &max, &min2);

    int rangemin, rangemax;
    uint16_t minor_cnt, major_cnt, divison_lines;
    determineParameters(pomiar, min, max, &rangemin, &rangemax, &minor_cnt, &major_cnt, &divison_lines);

    setChartRange(pomiar, rangemin, rangemax, minor_cnt, major_cnt, divison_lines);
    manageChartSeries(rysuj);
}

void laduj_wifi(lv_event_t * e)
{
	sprintf(wifi_read_ssid, lv_textarea_get_text(ui_Screen3_TextArea2));
	sprintf(wifi_password, lv_textarea_get_text(ui_Screen3_TextArea1));
	// Check if option selected was last one
	if( (lv_dropdown_get_option_cnt(ui_Dropdown4)) == (lv_dropdown_get_selected(ui_Dropdown4)+1) ){
        tryb_wifi=RESET_MODE_TO_HOTSPOT;
        obecny.etap=0;
        obecny.krok=0;
        wifi_set=0;
        iter_wifi_msg=0;
        reset_iter=0;
        wifi_change_bar=0;
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
        return;
	}
	if(((strlen(wifi_read_ssid))>1) && ((strlen(wifi_password)>7))){
		wifi_set=1;
		screen_change=2;
	}
}

void skanuj_wifi(lv_event_t * e){
	__HAL_TIM_CLEAR_FLAG(&htim11, TIM_SR_UIF);
	__HAL_TIM_CLEAR_FLAG(&htim10, TIM_SR_UIF);
	HAL_TIM_Base_Start_IT(&htim10); //send msg delay
	HAL_TIM_Base_Start_IT(&htim11); //end scan delay
	wifi_list_send=1;
	nie_szuka_wifi=0;
}

void wifi_type(lv_event_t * e)
{
	lv_dropdown_get_selected_str(ui_Dropdown4, wifi_read_ssid, 33);
	lv_textarea_set_text(ui_Screen3_TextArea2, wifi_read_ssid);
}
void stop_wifi_change(lv_event_t * e)
{
	tryb_wifi=RESET_MODE_TO_HOTSPOT;
	obecny.etap=0;
	obecny.krok=0;
    wifi_set=0;
    iter_wifi_msg=0;
    reset_iter=0;
    wifi_change_bar=0;
}
