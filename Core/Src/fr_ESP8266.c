 /*
 * ESP8266.c
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */


#include "fr_ESP8266.h"
#include "ui.h"
#include <ctype.h>

uint32_t seconds = 0;
uint16_t buffer_index = 0, timeout = 0, messageHandlerFlag = 0;

char wiersze[15][45]={'\0'};
uint16_t indx, kopiuj_buf=0;
int ip_liczby[4];


void ESP_RESET()
{

	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

}

void ESP_Server_Init()
{

	HAL_Delay(200);
	ESP_RESET();
	HAL_Delay(500);

}

void transfer_bufor(){
	uint16_t i=0;
	  		  	while(((ogon+i)!=glowa)&&(ogon+i)<1000){

	  		  	for(int a=0;a<5;a++){
	  		  		if((MainBuf[ogon+i])==sensor_data_table[a].letter){
	  		  				if((MainBuf[ogon+i-1])==':'){
	  		  						szukaj(sensor_data_table+a, (ogon+i));
	  		  				}
	  		  	}
	  		  	}
  		  		if((MainBuf[ogon+i-2])=='D'){
  		  				if((MainBuf[ogon+i-1])==','){
  		  						channel=(MainBuf[ogon+i]-48);
  		  				}
  		  		}
	  		  		if(flaga_szukania==1){
	  		  			int a, b;
						if((MainBuf[ogon+i])==*(tab_polecenie[obecny.etap].rsp)){
								if(ogon+i==998){
									a=999;
									b=1;
								}
								else if(ogon+i==999){
									a=1;
									b=2;
								}
								else{
									a=ogon+i+1;
									b=ogon+i+2;
								}
								if((MainBuf[a])==*(tab_polecenie[obecny.etap].rsp+1)){
									if((MainBuf[b])==*(tab_polecenie[obecny.etap].rsp+2)){
										flaga_szukania=2;
									}
								}

						}
	  		  		}
		  		  	if((MainBuf[ogon+i]=='\n') || (znak>43)){
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
		  		  	if((!nie_szuka_wifi) || (tryb_wifi==WIFI_CHANGE)){
			  		  	if(kopiuj_buf){
			  		  		if((MainBuf[ogon+i]!='"')&&(indx<32)&&(indy<7)){
			  		  		wifi_list[indy][indx]=MainBuf[ogon+i];
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
		  		  		szukaj_wifi(i);
		  		  	}
	  		  		i++;
	  		  	}
	  		  	HAL_UART_Transmit_DMA(&huart2, (uint8_t*) (MainBuf+ogon), i);
				ogon+=i;

}
void szukaj(struct search_letter_table * sensor_data_table,uint16_t znak) {

	int inde = znak - 5;
	int indd = znak - 4;
	int indc = znak - 3;
	int indb = znak - 2;

	inde=(inde+1000)%1000;
	indd=(indd+1000)%1000;
	indc=(indc+1000)%1000;
	indb=(indb+1000)%1000;

	if ((MainBuf[inde] >= 48 && MainBuf[inde] <= 57)
		&&(MainBuf[indd] >= 48 && MainBuf[indd] <= 57)
		&& (MainBuf[indc] >= 46 && MainBuf[indc] <= 57)
		&& (MainBuf[indb] >= 48 && MainBuf[indb] <= 57)) {

		sensor_data_table->data_label[0] = MainBuf[inde];
		sensor_data_table->data_label[1]  = MainBuf[indd];
		sensor_data_table->data_label[2]  = MainBuf[indc];
		sensor_data_table->data_label[3]  = MainBuf[indb];

		if(sensor_data_table->data_label[2]==46)
			MainBuf[indc]=48;


		measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].num = (((uint8_t) (MainBuf[inde])
				- 48) * 10) + ((uint8_t) (MainBuf[indd] - 48));
		measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].tenths = (((uint8_t) (MainBuf[indc])
				- 48) * 10) + ((uint8_t) (MainBuf[indb] - 48));

		sensor_data_table->pomiar = measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].num*100 + measure_tab[Measure_OUT].pomiar[sensor_data_table->ind].tenths;

		if(sensor_data_table->letter=='D')
			gradient_dust = getColorGradient(sensor_data_table->pomiar / 5);

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
	else{
		sensor_data_table->data_label[0] = 0;
		sensor_data_table->data_label[1]  = 0;
		sensor_data_table->data_label[2]  = 0;
		sensor_data_table->data_label[3]  = 0;
	}
}

void szukaj_wifi(uint16_t i){

	uint16_t ind,indmin9,indmin10;
	ind=ogon+i;
	char lit4, lit5;
	if((ind-4)<0){
		indmin9=ind+1000-4;
	}
	else{
		indmin9=ind-4;
	}
	if((ind-5)<0){
		indmin10=ind+1000-5;
	}
	else{
		indmin10=ind-5;
	}
	if(tryb_wifi==WIFI_CHANGE){
		lit4='A';
		lit5='T';
	}
	else{
		lit4=':';
		lit5='P';
	}
	if((MainBuf[indmin10]==lit5)&&(MainBuf[indmin9]==lit4)&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}

}

void ChooseNextStep(){
	if(tryb_wifi==RESET_MODE_TO_HOTSPOT){
		tryb_wifi=HOTSPOT;
		obecny.etap=0;
		obecny.krok=0;
        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
	}
	else if(tryb_wifi==RESET_MODE_TO_ROUTER){
		tryb_wifi=ROUTER_MODE;
		obecny.etap=0;
		obecny.krok=0;
        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
	}
	else if(tryb_wifi==RESET_TO_WIFI_CHANGE){
		tryb_wifi=WIFI_CHANGE;
		obecny.etap=0;
		obecny.krok=0;
		wifi_change_bar=20;
	}
	else if(tryb_wifi==WIFI_CHANGE){
		sprintf(ip_set, wifi_list[0]);
		sprintf(wifi_ssid[0],wifi_read_ssid);
		sprintf(wifi_ssid[1],wifi_password);
		save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
		read_ssid();
		obecny.etap=0;
		obecny.krok=0;
		_ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		HAL_TIM_Base_Start_IT(&htim1);
		wifi_set=0;
		tryb_wifi=RESET_MODE_TO_ROUTER;
	}
	else{
		if(wifi_set){
			tryb_wifi=RESET_TO_WIFI_CHANGE;
			wifi_change_bar=10;
			ESP_RESET();
			obecny.etap=0;
			obecny.krok=0;
		}
		else{
			obecny.etap=0;
			PomiarFlag=1;
			__HAL_RCC_PWR_CLK_ENABLE();
			HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
			HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
		    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
		    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
		    pobierz_czas();
		    SetNextAlarm();
		}
	}
}

void send_msg(){

	switch(obecny.krok){

		case AT_COMMAND_IDLE_WAITING:
				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
				if(TIM4->CNT > tab_polecenie[obecny.etap].time_init){
					if(obecny.etap==0&&((tryb_wifi==ROUTER_MODE)||(tryb_wifi==HOTSPOT))){
						measure_battery();
					}
					if((obecny.etap==7)&&((tryb_wifi==ROUTER_MODE)||(tryb_wifi==HOTSPOT))){
						HAL_RTC_GetTime(&hrtc, &time_measure, RTC_FORMAT_BIN);
						if(done<4){
							writeDataAtPosition(measure_tab);
						}
						else if(done<9){
							writeDataAtPosition(measure_tab+1);
						}
						else{
							done=1;
						}
					}
					if(done==1){
						obecny.krok++;
						i1=0;
					}
				}
		break;

		case AT_COMMAND_SENDING:
				if((USART1->SR) & (USART_SR_TC))
				{
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
				else if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tab_polecenie[obecny.etap].msg, strlen(tab_polecenie[obecny.etap].msg))==HAL_OK){
					obecny.krok++;
					flaga_szukania=1;
				}
				else{
					obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
					obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
				}
				}
		break;

		case AT_COMMAND_FIND_RESPONSE:
 				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
				if(TIM4->CNT > tab_polecenie[obecny.etap].time_response){
					if(flaga_szukania==2){
						obecny.krok++;
					}
					else{
						if(++iter_wifi_msg>tab_polecenie[obecny.etap].iter){
							iter_wifi_msg=0;
							reset_iter++;
							if( (tryb_wifi==ROUTER_MODE) && (reset_iter>25) ){
								tryb_wifi=RESET_MODE_TO_HOTSPOT;
								obecny.etap=0;
								obecny.krok=0;
							}
							else if( (tryb_wifi==ROUTER_MODE) && (reset_iter>8) ){
								tryb_wifi=RESET_MODE_TO_ROUTER;
								obecny.etap=0;
								obecny.krok=0;
								reset_iter=0;
						        save_ssid(wifi_ssid[0], wifi_ssid[1], ip_set);
							}
							else if( (tryb_wifi==HOTSPOT) && (reset_iter>5) ){
								tryb_wifi=RESET_MODE_TO_HOTSPOT;
								obecny.etap=0;
								obecny.krok=0;
								reset_iter=0;
							}
							else{
								obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
								obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
							}
						}
						else{
							obecny.krok=tab_polecenie[obecny.krok].powrot.krok=0;
						}
					}
					flaga_szukania=0;
					i1=0;
				}
				if(flaga_szukania==2){
					obecny.krok++;
				}
		break;

		case AT_COMMAND_WAIT_AND_EXIT:
				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
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
				else if(TIM4->CNT > tab_polecenie[obecny.etap].time_exit){
					if(tab_polecenie[obecny.etap].terminate==0){
						obecny.krok=0;
						obecny.etap++;
						if(tryb_wifi==WIFI_CHANGE){
							wifi_change_bar=20+(obecny.etap)*5;
						}
					}
					else{
						ChooseNextStep();
					}
				}
		break;

		}

}

void save_ssid(char * ssid, char * pass, char * ip_set){

	uint8_t ssid_len= strlen(ssid);
	uint8_t pass_len= strlen(pass);
	uint8_t mem_ssid[ssid_len+1];
	uint8_t mem_pass[pass_len+1];
	memcpy(mem_ssid, ssid, ssid_len);
	memcpy(mem_pass, pass, pass_len);
	uint32_t store = (ssid_len<<24) | (pass_len<<16) | (tryb_wifi&0xff);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, store);
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = ((mem_ssid[4*i])<<24)|(mem_ssid[4*i+1]<<16)|(mem_ssid[4*i+2]<<8)|(mem_ssid[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x06U+(0x01U*i)), temp);
	}
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = (mem_pass[4*i]<<24)|(mem_pass[4*i+1]<<16)|(mem_pass[4*i+2]<<8)|(mem_pass[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x0CU+(0x01U*i)), temp);
	}
	extractNumbersFromIP(ip_set, ip_liczby);
	uint32_t store_ip=((ip_liczby[0]&0xff)<<24)|((ip_liczby[1]&0xff)<<16)|((ip_liczby[2]&0xff)<<8)|((ip_liczby[3]&0xff));
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, store_ip);

}

void read_ssid(){

	uint32_t store= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
	uint32_t ssid_len=store>>24;
	uint32_t pass_len=(store>>16)&0xff;
	tryb_wifi=(store&0xff);
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x06U+(0x01U*i)));
		wifi_ssid[0][4*i]=(temp>>24)&0xff;
		wifi_ssid[0][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[0][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[0][4*i+3]=temp&0xff;
	}
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x0CU+(0x01U*i)));
		wifi_ssid[1][4*i]=(temp>>24)&0xff;
		wifi_ssid[1][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[1][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[1][4*i+3]=temp&0xff;
	}
	for(int i=ssid_len;i<33;i++){
		wifi_ssid[0][i]='\0';
	}
	for(int i=pass_len;i<33;i++){
		wifi_ssid[1][i]='\0';
	}
	uint32_t store_ip= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
	int a=store_ip>>24;
	int b=(store_ip>>16)&0xff;
	int c=(store_ip>>8)&0xff;
	int d=store_ip&0xff;
    snprintf(wifi_ssid[2], 16, "%d.%d.%d.%d", a, b, c, d);
    snprintf(wifi_ssid[3], 16, "%d.%d.%d.%d", a, b, c, 1);

}

void extractNumbersFromIP(char* ipString, int* ipNumbers) {

    char* token = strtok(ipString, ".");
    int i = 0;
    while (token != NULL && i < 4) {
        if (isdigit(token[0])) {
            ipNumbers[i] = atoi(token);
            i++;
        }
        token = strtok(NULL, ".");
    }
}

uint32_t getColorGradient(int value){
    if(value < 0){
    	value=0;
    }
    if(value > 100){
    	value=100;
    }
    int red, green;

    if (value < 50) {
        red = (int)((value / 50.0) * 255);
        green = 255;
    } else {
        red = 255;
        green = (int)(((100 - value) / 50.0) * 255);
    }

    int blue = 0;

    int color = (red << 16) + (green << 8) + blue;

    return color;
}


