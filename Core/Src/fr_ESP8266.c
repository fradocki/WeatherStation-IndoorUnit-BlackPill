 /*
 * ESP8266.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Filip
 */


#include "fr_ESP8266.h"
#include "ui.h"
#include <ctype.h>

uint32_t seconds = 0;
uint16_t buffer_index = 0, timeout = 0, messageHandlerFlag = 0;

char wiersze[15][45]={'\0'};
uint16_t indx, kopiuj_buf=0;
int ip_liczby[4];

// function to hard reset ESP8266 with GPIO pins
void esp_reset()
{

	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(esp_reset_GPIO_Port, esp_reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(esp_reset_GPIO_Port, esp_reset_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

}

// function used to initialize ESP8266 through hard reset
void esp_server_init()
{

	HAL_Delay(200);
	esp_reset();
	HAL_Delay(500);

}

/* function used to process and transfer data through MainBuf(Buffer with data copied from RxBuf-
   which was received from ESP8266), buffer is transfered to external UASRT2(PC) */
void transfer_bufor(){
	uint16_t i=0;

    while(((ogon+i)!=glowa)&&(ogon+i)<1000){

        //variables used to search trough MainBuf buffer
        int ind  = ogon+i;
        int inda = ((ogon+i-1)+1000)%1000;
        int indb = ((ogon+i-2)+1000)%1000;
        int indc = ((ogon+i-3)+1000)%1000;

        //loop used to search for measurement values in messages from ESP8266
        for(int a=0;a<5;a++){
            if((MainBuf[ind])==sensor_data_table[a].letter){
                    if((MainBuf[inda])==':'){
                        search_msg_response(sensor_data_table+a, ind, inda, indb);
                    }
        }
        }

        //condition used to check channel value on which ESP8266 was connected
        if((MainBuf[indc])=='P'){
            if((MainBuf[indb])=='D'){
                if((MainBuf[inda])==','){
                    channel=(MainBuf[ind]-48);
                }
            }
        }

        //conditions used to check for positive confirmation of receive of the command
        if(search_flag==1){
            if((MainBuf[ind])==*(tab_polecenie[obecny.etap].rsp+2)){
                if((MainBuf[inda])==*(tab_polecenie[obecny.etap].rsp+1)){
                    if((MainBuf[indb])==*(tab_polecenie[obecny.etap].rsp)){
                        search_flag=2;
                    }
                }

            }
        }

        /*code used to copy messages form ESP8266 to label-
           to show it on terminal screen */
        if((MainBuf[ind]=='\n') || (znak>43)){
          wiersze[14][znak] = MainBuf[ogon+i];
          for (int k = 0; k < 14; k++) {
              sprintf(wiersze[k], wiersze[k+1]);
          }

          for(int k=0;k<45;k++){
            wiersze[14][k]='\0';
          }
          znak=0;
          if(init_lv){
                char label_print[675]={'\0'};
                for (int k = 0; k < 14; k++){
                    strcat(label_print, wiersze[k]);
                }
                lv_label_set_text(ui_Screen2_Label2,label_print);
          }
        }
        else{
            wiersze[14][znak] = MainBuf[ogon+i];
            znak++;
        }

        // code used to find wifi SSID's or IP adress in response from ESP8266
        if((!nie_szuka_wifi) || (tryb_wifi==WIFI_CHANGE)){
            if(kopiuj_buf){
                if((MainBuf[ind]!='"')&&(indx<32)&&(indy<7)){
                wifi_list[indy][indx]=MainBuf[ind];
                indx++;
                }
                else{
                    kopiuj_buf=0;
                    if(indx>1){
                        if(!nie_szuka_wifi){
                            wifi_list[indy][indx]='\n';
                        }
                        indy++;
                    }
                    indx=0;
                }
            }
            search_wifi(i);
        }

        // iterate to next buffer index
        i++;
    }

    // if lenght of new data was measured, send it to UART2(PC)
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*) (MainBuf+ogon), i);
    // update tail position
    ogon+=i;

}


void search_msg_response(struct search_letter_table * sensor_data_table,uint16_t znak,int indb,int indc) {
    // calculate index for previous characters
	int inde = znak - 5;
	int indd = znak - 4;

	// calculate indexes for buffer to circulate loop
	inde=(inde+1000)%1000;
	indd=(indd+1000)%1000;

	//check if given characters are numbers(also dot for second char)
	if ((MainBuf[inde] >= 48 && MainBuf[inde] <= 57)
		&&(MainBuf[indd] >= 48 && MainBuf[indd] <= 57)
		&& (MainBuf[indc] >= 46 && MainBuf[indc] <= 57)
		&& (MainBuf[indb] >= 48 && MainBuf[indb] <= 57)) {

	    // assign characters to sensor_data_table label
		sensor_data_table->data_label[0] = MainBuf[inde];
		sensor_data_table->data_label[1]  = MainBuf[indd];
		sensor_data_table->data_label[2]  = MainBuf[indc];
		sensor_data_table->data_label[3]  = MainBuf[indb];

		// change dot for 0
		if(sensor_data_table->data_label[2]==46)
			MainBuf[indc]=48;

		// convert and store read values in measure_tab(in format to handle SD card storage)
		measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].num = (((uint8_t) (MainBuf[inde])
				- 48) * 10) + ((uint8_t) (MainBuf[indd] - 48));
		measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].tenths = (((uint8_t) (MainBuf[indc])
				- 48) * 10) + ((uint8_t) (MainBuf[indb] - 48));

		// store measurements as integer value
		sensor_data_table->pomiar = measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].num*100 + measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].tenths;

		// calculate air quality bar color value for dust measurements
		if(sensor_data_table->letter=='D')
			gradient_dust = get_color_gradient(sensor_data_table->pomiar / 5);

		// get rid of leading zeros in labels
		if (sensor_data_table->data_label[0] == '0') {
			sensor_data_table->data_label[0] = ' ';
			if(sensor_data_table->letter=='D' || sensor_data_table->letter=='B'){
				if (sensor_data_table->data_label[1] == '0') {
					sensor_data_table->data_label[1] = ' ';
					if (sensor_data_table->data_label[2] == '0') {
						sensor_data_table->data_label[1] = ' ';
						if (sensor_data_table->data_label[3] == '0') {
							sensor_data_table->data_label[1] = '\0';
							sensor_data_table->data_label[0] = '0';
						}
					}
				}
			}
		}
	}
	// if measurement was not correct display zeros
	else{
		sensor_data_table->data_label[0] = 0;
		sensor_data_table->data_label[1] = 0;
		sensor_data_table->data_label[2] = 0;
		sensor_data_table->data_label[3] = 0;
	}
}

/* function to search wifi SSID's in response message from ESP8266
   or to search for own IP address in router mode*/

void search_wifi(uint16_t i){

	uint16_t ind,indmin4,indmin5;
	ind=ogon+i;
	char lit4, lit5;

	// calculate previous character position in circular buffer
	indmin4=((ind-4)+1000)%1000;
	indmin5=((ind-5)+1000)%1000;

	// search for specific characters- part of ESP8266 response
	if(tryb_wifi==WIFI_CHANGE){
		lit4='A';
		lit5='T';
	}
	else{
		lit4=':';
		lit5='P';
	}

	// if condition is satisfied- start to copy characters
	if((MainBuf[indmin5]==lit5)&&(MainBuf[indmin4]==lit4)&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}

}

/* The ChooseNextStep function is responsible for managing WiFi working mode.
   At the end of communication sequence, depending on system state variables it will
   maintain same state or switch into other. */

void ChooseNextStep(){

    /* If the system is in the state to reset WiFi to hotspot mode.
       Reset sequence is ended and system switch to hotspot mode. */

    if(tryb_wifi==RESET_MODE_TO_HOTSPOT){
        // update system mode to hotspot.
        tryb_wifi=HOTSPOT;

        // reset the current step and stage in the process.
        obecny.etap=0;
        obecny.krok=0;

        // save the current WiFi settings.
        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
    }

    /* If the system is in the state to reset WiFi to router mode.
       Reset sequence is ended and system switch to router mode. */
    else if(tryb_wifi==RESET_MODE_TO_ROUTER){
        // update system mode to router.
        tryb_wifi=ROUTER_MODE;

        // reset the current step and stage in the process
        obecny.etap=0;
        obecny.krok=0;

        // save the current WiFi settings
        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
    }

    /* If the system is in the state to reset WiFi to WiFi change.
       Reset sequence is ended and system start sequence in order to change WiFi. */

    else if(tryb_wifi==RESET_TO_WIFI_CHANGE){

        // update system mode to change WiFi settings
        tryb_wifi=WIFI_CHANGE;

        // reset the current step and stage in the process
        obecny.etap=0;
        obecny.krok=0;

        // update the status bar for WiFi changes
        wifi_change_bar=20;
    }

    /* If the system is in the state of changing WiFi settings. It means it ended
        WiFi change process and will start communication in router mode switching thought
        reset to router change mode */

    else if(tryb_wifi==WIFI_CHANGE){
        // update the IP settings, SSID, and password from the read values
        sprintf(ip_set, wifi_list[0]);
        sprintf(wifi_ssid[0],wifi_read_ssid);
        sprintf(wifi_ssid[1],wifi_password);

        // save the new WiFi settings
        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);

        // read the new WiFi settings
        read_ssid();

        // reset the current step and stage in the process
        obecny.etap=0;
        obecny.krok=0;

        // change the UI to the main screen
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);

        // start a timer for display backlight turn off delay
        __HAL_TIM_SET_COUNTER(&htim1, 0);
        HAL_TIM_Base_Start_IT(&htim1);

        // reset the WiFi set flag
        wifi_set=0;

        // set the system mode to reset to router mode
        tryb_wifi=RESET_MODE_TO_ROUTER;
    }
    else{
        // if the wifi_set flag is set device switch to WiFi change mode through
        if(wifi_set){
            // set the system mode to reset and change wifi settings.
            tryb_wifi=RESET_TO_WIFI_CHANGE;

            // update the status bar for WiFi changes.
            wifi_change_bar=10;

            // reset the ESP module.
            esp_reset();

            // reset the current step and stage in the process.
            obecny.etap=0;
            obecny.krok=0;
        }

        /* If device mode is hotspot or router mode and it's not changing.
           Communication ended successfully end communication sequence and set
           alarm timer for next to start */
        else{
            // reset the current stage
            obecny.etap=0;

            // set flag for measuring
            PomiarFlag=1;

            // enable the Power Clock
            __HAL_RCC_PWR_CLK_ENABLE();

            // disable the WakeUp Pin
            HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

            // deactivate the WakeUp Timer and Alarm
            HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
            HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

            // clear the WakeUp Timer and Alarm flags
            __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
            __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

            // get the current time
            get_time();

            // set the next alarm
            set_next_alarm();
        }
    }
}

   /* Function to handle communication in all modes using obecny.krok for stepping through sequence for
   individual message and obecny.etap to step through messages(stage) in sequence of each mode, modes and
   sequences are described in fr_ESP8266_messages.c file*/

void send_msg(){

    // switch between steps in individual message handle
	switch(obecny.krok){

	    //step for initial delay given amount of time
		case AT_COMMAND_IDLE_WAITING:
            if(i1==0){
                __HAL_TIM_SET_COUNTER(&htim4,0);
                i1=1;
            }

            /* in this step measurements and data saving functions are
               used not to halt processor during communication sequence */

            if(TIM4->CNT > tab_polecenie[obecny.etap].time_init){

                //at first stage measure battery level
                if(obecny.etap==0&&((tryb_wifi==ROUTER_MODE)||(tryb_wifi==HOTSPOT))){
                    measure_battery();
                }

                //at eighth stage save measured data to sd card
                if((obecny.etap==7)&&((tryb_wifi==ROUTER_MODE)||(tryb_wifi==HOTSPOT))){
                    if(done<4){
                        //write data to SD card one measurement at the time for indoor measure
                        write_data_at_position(measure_tab);
                    }
                    else if(done<9){
                        //write data to SD card one measurement at the time for outdoor measure
                        write_data_at_position(measure_tab+1);
                    }
                    else{
                        done=1;
                    }
                }

                // if previous task were made switch to next step
                if(done==1){
                    obecny.krok++;
                    i1=0;
                }

            }
		break;

		// step to send message to ESP8266 in DMA mode and check for response
		case AT_COMMAND_SENDING:

            // check if USART1 is not busy
            if((USART1->SR) & (USART_SR_TC))
            {

            /* if screen was changed to wifi panel, search for wireless networks,
               interrupting main sequence for a while */
            if(wifi_list_send){
                if(i1==0){
                    HAL_UART_Transmit_DMA(&huart1,(uint8_t*)"AT+CWLAP\r\n",strlen("AT+CWLAP\r\n"));
                    i1=1;
                    __HAL_TIM_SET_COUNTER(&htim4,0);
                }
                else{
                    if(TIM4->CNT > 16000){
                        wifi_list_send=0;
                    }
                }
            }

            /* send message from current communication stage and start searching
                 for response setting search_flag, switch to next step if communication
                 was successful */
            else if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tab_polecenie[obecny.etap].msg, strlen(tab_polecenie[obecny.etap].msg))==HAL_OK){
                obecny.krok++;
                search_flag=1;
            }

            // if communication was not successful back to specified step and stage
            else{
                obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
                obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
            }
            }
		break;

		// stage to find response to current message
		case AT_COMMAND_FIND_RESPONSE:

            //start timer to measure maximum time for response
            if(i1==0){
                __HAL_TIM_SET_COUNTER(&htim4,0);
                i1=1;
            }

            // check if maximum time for response elapsed
            if(TIM4->CNT > tab_polecenie[obecny.etap].time_response){

                // if message was found switch to next step
                if(search_flag==2){
                    obecny.krok++;
                }

                // if message was not found iterate current message specified amount of times
                else{
                    if(++iter_wifi_msg>tab_polecenie[obecny.etap].iter){

                        //reset current message iteration
                        iter_wifi_msg=0;
                        //count reset iteration after
                        reset_iter++;

                        /* if device is in router mode and reset sequence repeat
                           given amount of times switch to hotspot mode */
                        if( (tryb_wifi==ROUTER_MODE) && (reset_iter>25) ){
                            tryb_wifi=RESET_MODE_TO_HOTSPOT;
                            obecny.etap=0;
                            obecny.krok=0;
                        }

                        /* if device is in router mode and reset sequence repeat
                           given amount of times reset to the same mode */
                        else if( (tryb_wifi==ROUTER_MODE) && (reset_iter>8) ){
                            tryb_wifi=RESET_MODE_TO_ROUTER;
                            obecny.etap=0;
                            obecny.krok=0;
                            reset_iter=0;
                            save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
                        }

                        /* if device is in hotspot mode and reset sequence repeat
                           given amount of times reset to the same mode */
                        else if( (tryb_wifi==HOTSPOT) && (reset_iter>5) ){
                            tryb_wifi=RESET_MODE_TO_HOTSPOT;
                            obecny.etap=0;
                            obecny.krok=0;
                            reset_iter=0;
                        }

                        // if reset conditions was not met go back to specified stage of current sequence
                        else{
                            obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
                            obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
                        }
                        }

                    /* if iteration condition is not meet repeat current message again by
                       going back to first step */
                    else{
                        obecny.krok=tab_polecenie[obecny.krok].powrot.krok=0;
                    }
                }
                //reset search flag
                search_flag=0;
                //reset initial timer start flag
                i1=0;
            }

            /* check if response was found before time for response elapsed
               if so switch to next step */
            if(search_flag==2){
                obecny.krok++;
            }
		break;

		/* last step for each message, delay specified amount of time and do one of below
		   -switch to next stage
		   -end communication if it was last message
		   -change working mode if conditions are meet
		   */
		case AT_COMMAND_WAIT_AND_EXIT:

		    //start timer to measure exit delay time
            if(i1==0){
                __HAL_TIM_SET_COUNTER(&htim4,0);
                i1=1;
            }

            /* if screen was changed to wifi panel, search for wireless networks,
               interrupting main sequence for a while */
            if(wifi_list_send){
                if(TIM4->CNT > 2000){
                    if(i1==1){
                        HAL_UART_Transmit_DMA(&huart1, (uint8_t*) "AT+CWLAP\r\n",strlen("AT+CWLAP\r\n"));
                        i1=2;
                        __HAL_TIM_SET_COUNTER(&htim4,0);
                    }
                    __HAL_TIM_SET_COUNTER(&htim4,0);
                    if(++i1>4){
                        wifi_list_send=0;
                    }
                }

            }

            //if time elapsed check for next conditions
            else if(TIM4->CNT > tab_polecenie[obecny.etap].time_exit){

                //if it's not last message, switch to next stage
                if(tab_polecenie[obecny.etap].terminate==0){
                    obecny.krok=0;
                    obecny.etap++;
                    if(tryb_wifi==WIFI_CHANGE){
                        wifi_change_bar=20+(obecny.etap)*5;
                    }
                }

                // if it's last message choose next step with dedicated function
                else{
                    ChooseNextStep();
                }
            }
		break;

		}

}

/*
 * This function writes WiFi credentials and connection setting
 * to a backup domain in the RTC peripheral.
 */
void save_ssid(char * ssid, char * pass, char * ip_set){

    // Determine the lengths of the SSID and password
    uint8_t ssid_len= strlen(ssid);
    uint8_t pass_len= strlen(pass);

    // Create memory buffers to hold the SSID and password
    uint8_t mem_ssid[ssid_len+1];
    uint8_t mem_pass[pass_len+1];

    // Copy the SSID and password into the memory buffers
    memcpy(mem_ssid, ssid, ssid_len);
    memcpy(mem_pass, pass, pass_len);

    // Create a 32-bit value to hold the lengths of the SSID and password, and the WiFi mode.
    // This value will be stored in the backup register RTC_BKP_DR5
    uint32_t store = (ssid_len<<24) | (pass_len<<16) | (tryb_wifi&0xff);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, store);

    // Store the SSID in the backup registers, each character is 8 bit value
    for(int i=0; i<((ssid_len+3)/4); i++){
        uint32_t temp = ((mem_ssid[4*i])<<24)|(mem_ssid[4*i+1]<<16)|(mem_ssid[4*i+2]<<8)|(mem_ssid[4*i+3]);
        HAL_RTCEx_BKUPWrite(&hrtc, (0x06U+(0x01U*i)), temp);
    }

    // Store the password in the backup registers, in the same way as the SSID
    for(int i=0; i<((pass_len+3)/4); i++){
        uint32_t temp = (mem_pass[4*i]<<24)|(mem_pass[4*i+1]<<16)|(mem_pass[4*i+2]<<8)|(mem_pass[4*i+3]);
        HAL_RTCEx_BKUPWrite(&hrtc, (0x0CU+(0x01U*i)), temp);
    }

    // Use the extract_numbers_from_ip function to convert the IP address string into an array of numbers
    extract_numbers_from_ip(ip_set, ip_liczby);

    // Convert the array of IP address numbers into a 32-bit value. Store it the backup register RTC_BKP_DR4.
    uint32_t store_ip=((ip_liczby[0]&0xff)<<24)|((ip_liczby[1]&0xff)<<16)|((ip_liczby[2]&0xff)<<8)|((ip_liczby[3]&0xff));
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, store_ip);
}

/*
 * This function reads WiFi credentials and connection setting
 * from a backup domain in the RTC peripheral.
 */
void read_ssid(){

    /* Read the value from the backup register RTC_BKP_DR5.
       This value contains the lengths of the SSID and the password, and the WiFi mode. */
    uint32_t store= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);

    // Extract the length of the SSID, the password, and the WiFi mode from the stored value
    uint32_t ssid_len=store>>24;
    uint32_t pass_len=(store>>16)&0xff;
    tryb_wifi=(store&0xff);

    // Read the SSID from the backup registers, each character is 8 bit value
    for(int i=0; i<((ssid_len+3)/4); i++){
        uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x06U+(0x01U*i)));
        wifi_ssid[0][4*i]=(temp>>24)&0xff;
        wifi_ssid[0][4*i+1]=(temp>>16)&0xff;
        wifi_ssid[0][4*i+2]=(temp>>8)&0xff;
        wifi_ssid[0][4*i+3]=temp&0xff;
    }

    // Read the password from the backup registers, stored in the same way as the SSID
    for(int i=0; i<((pass_len+3)/4); i++){
        uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x0CU+(0x01U*i)));
        wifi_ssid[1][4*i]=(temp>>24)&0xff;
        wifi_ssid[1][4*i+1]=(temp>>16)&0xff;
        wifi_ssid[1][4*i+2]=(temp>>8)&0xff;
        wifi_ssid[1][4*i+3]=temp&0xff;
    }

    // Fill in any unused parts of the SSID and password with null characters, to ensure proper string termination
    for(int i=ssid_len;i<33;i++){
        wifi_ssid[0][i]='\0';
    }
    for(int i=pass_len;i<33;i++){
        wifi_ssid[1][i]='\0';
    }

    /* Read the IP address from the backup register RTC_BKP_DR4.
       The IP address is stored as a 4-byte value,
       with each byte representing one part of the IP address. */
    uint32_t store_ip= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
    int a=store_ip>>24;
    int b=(store_ip>>16)&0xff;
    int c=(store_ip>>8)&0xff;
    int d=store_ip&0xff;

    // Store the IP address and a gate address
    snprintf(wifi_ssid[2], 16, "%d.%d.%d.%d", a, b, c, d);
    snprintf(wifi_ssid[3], 16, "%d.%d.%d.%d", a, b, c, 1);
}


/*
 * This function extracts the numbers from an IP address string and stores them in an integer array.
 *
 * @param ipString: This is a pointer to a string which contains the IP address.
 *                  The IP address is expected to be in the usual dotted decimal notation.
 *
 * @param ipNumbers: This is a pointer to an array of integers. The function will store the
 *                   individual numbers of the IP address in this array.
 */
void extract_numbers_from_ip(char* ipString, int* ipNumbers) {

    // 'strtok' function is used to split the IP address string into tokens.
    // A token in this context is a part of the string bounded by the delimiter "."
    char* token = strtok(ipString, ".");

    // Counter for the number of tokens (IP address numbers) processed
    int i = 0;

    // We loop while we are able to get tokens from the string and we have not yet processed 4 numbers.
    // The check for i<4 prevents buffer overflow in case the string contains more than 4 numbers.
    while (token != NULL && i < 4) {

        // We only process tokens that start with a digit. This filters out empty tokens and tokens that
        // start with a non-digit character.
        if (isdigit(token[0])) {

            // The 'atoi' function converts the token (a string) to an integer and stores it in the ipNumbers array.
            ipNumbers[i] = atoi(token);

            // Increase the token counter.
            i++;
        }

        // Get the next token from the IP address string
        token = strtok(NULL, ".");
    }
}


uint32_t get_color_gradient(int value){
    // If the input value is less than 0, reset it to 0
    if(value < 0){
        value=0;
    }

    // If the input value is greater than 100, reset it to 100
    if(value > 100){
        value=100;
    }

    // Initialize variables to hold the calculated red and green color values
    int red, green;

    // If the input value is less than 50
    if (value < 50) {
        // Calculate the red value based on the input
        red = (int)((value / 50.0) * 255);
        // The green value is set to its maximum value
        green = 255;

    } else {
        // If the input value is 50 or greater, the red value is set to its maximum value
        red = 255;
        // Calculate the green value based on the input
        green = (int)(((100 - value) / 50.0) * 255);
    }

    // The blue value is always 0 in this color gradient
    int blue = 0;

    // Create the final color value
    int color = (red << 16) + (green << 8) + blue;

    // Return the final color value
    return color;
}

