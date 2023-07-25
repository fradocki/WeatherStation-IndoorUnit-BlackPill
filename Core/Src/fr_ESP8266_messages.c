/*
 * fr_ESP8266_messages.c
 *
 *  Created on: 23 cze 2023
 *      Author: Filip
 */

/*
Device can work in to basic modes in direct communication to central device AP
or thought corresponding AP(wi-fi network)
-load_msg- mode used in indirect communication
-load_msg- mode used in direct communication

There are also two additional modes
-wifi_reset- to reset ESP8266 device, used between changes of working mode and in case of communication failure
-chabge_wifi- to change from direct mode to indirect and to switch between AP's in direct mode
*/


#include "fr_ESP8266_messages.h"

//communication thought local wi-fi network
void load_msg(){

	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+CWMODE=1\r\n");
	tab_polecenie[0].time_response = 4000;
	sprintf(tab_polecenie[0].rsp,"Ok\r");
	tab_polecenie[0].powrot.etap =3;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =0;
	tab_polecenie[0].iter =4;
	tab_polecenie[0].terminate =0;

	tab_polecenie[1].time_init=0;
	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
	tab_polecenie[1].time_response = 4000;
	sprintf(tab_polecenie[1].rsp,"OK\r");
	tab_polecenie[1].powrot.etap = 3;
	tab_polecenie[1].powrot.krok = 0;
	tab_polecenie[1].time_exit = 0;
	tab_polecenie[1].iter =4;
	tab_polecenie[1].terminate =0;

	tab_polecenie[2].time_init=0;
	sprintf(tab_polecenie[2].msg,"AT+CIPSERVER=1,80\r\n");
	tab_polecenie[2].time_response = 4000;
	sprintf(tab_polecenie[2].rsp,"OK\r");
	tab_polecenie[2].powrot.etap = 3;
	tab_polecenie[2].powrot.krok = 0;
	tab_polecenie[2].time_exit = 0;
	tab_polecenie[2].iter =4;
	tab_polecenie[2].terminate =0;


	tab_polecenie[3].time_init=0;
	sprintf(tab_polecenie[3].msg,"AT+CIPSTA_CUR=\"%s\",\"%s\",\"255.255.255.0\"\r\n", wifi_ssid[2], wifi_ssid[3]);
	tab_polecenie[3].time_response =10000;
	sprintf(tab_polecenie[3].rsp,"OK\r");
	tab_polecenie[3].powrot.etap =3;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =6;
	tab_polecenie[3].terminate =0;

	tab_polecenie[4].time_init=0;
	sprintf(tab_polecenie[4].msg,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifi_ssid[0], wifi_ssid[1]);
	tab_polecenie[4].time_response = 30000;
	sprintf(tab_polecenie[4].rsp,"OK\r");
	tab_polecenie[4].powrot.etap =4;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =10;
	tab_polecenie[4].iter =6;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=0;
	sprintf(tab_polecenie[5].msg,"AT\r\n");
	tab_polecenie[5].time_response = 40000;
	sprintf(tab_polecenie[5].rsp,":D#");
	tab_polecenie[5].powrot.etap =4;
	tab_polecenie[5].powrot.krok =0;
	tab_polecenie[5].time_exit =0;
	tab_polecenie[5].iter =5;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	sprintf(tab_polecenie[6].msg,"AT+CIPSEND=%d,3\r\n", channel);
	tab_polecenie[6].time_response =15000;
	sprintf(tab_polecenie[6].rsp,"OK\r");
	tab_polecenie[6].powrot.etap =3;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =0;
	tab_polecenie[6].iter =3;
	tab_polecenie[6].terminate =0;

	tab_polecenie[7].time_init=0;
	sprintf(tab_polecenie[7].msg,"GIT");
	tab_polecenie[7].time_response =30000;
	sprintf(tab_polecenie[7].rsp,"OK\r");
	tab_polecenie[7].powrot.etap =3;
	tab_polecenie[7].powrot.krok =0;
	tab_polecenie[7].time_exit =0;
	tab_polecenie[7].iter =3;
	tab_polecenie[7].terminate =0;

	tab_polecenie[8].time_init=500;
	sprintf(tab_polecenie[8].msg,"AT+GSLP=60000\r\n");
	tab_polecenie[8].time_response = 6000;
	sprintf(tab_polecenie[8].rsp,"OK\r");
	tab_polecenie[8].powrot.etap =0;
	tab_polecenie[8].powrot.krok =0;
	tab_polecenie[8].time_exit =200;
	tab_polecenie[8].iter =2;
	tab_polecenie[8].terminate =1;

	if(wifi_set){
		sprintf(tab_polecenie[7].msg,"#WF");
		sprintf(tab_polecenie[7].rsp,"#GT");
	}

}

//direct communication to control unit AP

void load_hotspot(){

	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+CWMODE=3\r\n");
	tab_polecenie[0].time_response = 6000;
	sprintf(tab_polecenie[0].rsp,"OK\r");
	tab_polecenie[0].powrot.etap =0;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =100;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =0;

	tab_polecenie[1].time_init=0;
	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
	tab_polecenie[1].time_response = 4000;
	sprintf(tab_polecenie[1].rsp,"OK\r");
	tab_polecenie[1].powrot.etap = 1;
	tab_polecenie[1].powrot.krok = 0;
	tab_polecenie[1].time_exit = 0;
	tab_polecenie[1].iter =2;
	tab_polecenie[1].terminate =0;

	tab_polecenie[2].time_init=0;
	sprintf(tab_polecenie[2].msg,"AT+CIPSERVER=1,80\r\n");
	tab_polecenie[2].time_response = 4000;
	sprintf(tab_polecenie[2].rsp,"OK\r");
	tab_polecenie[2].powrot.etap = 2;
	tab_polecenie[2].powrot.krok = 0;
	tab_polecenie[2].time_exit = 0;
	tab_polecenie[2].iter =2;
	tab_polecenie[2].terminate =0;


	tab_polecenie[3].time_init=0;
	sprintf(tab_polecenie[3].msg,"AT+CWSAP_CUR=\"ESP82661\",\"1234567890\",5,3\r\n");
	tab_polecenie[3].time_response =20000;
	sprintf(tab_polecenie[3].rsp,"OK\r");
	tab_polecenie[3].powrot.etap =3;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =2;
	tab_polecenie[3].terminate =0;

	tab_polecenie[4].time_init=0;
	sprintf(tab_polecenie[4].msg,"AT\r\n");
	tab_polecenie[4].time_response =20000;
	sprintf(tab_polecenie[4].rsp,":D#");
	tab_polecenie[4].powrot.etap =3;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =0;
	tab_polecenie[4].iter =8;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=100;
	sprintf(tab_polecenie[5].msg,"AT+CIPSEND=%d,3\r\n", channel);
	tab_polecenie[5].time_response =15000;
	sprintf(tab_polecenie[5].rsp,"OK\r");
	tab_polecenie[5].powrot.etap =3;
	tab_polecenie[5].powrot.krok =0;
	tab_polecenie[5].time_exit =0;
	tab_polecenie[5].iter =3;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	sprintf(tab_polecenie[6].msg,"GIT");
	tab_polecenie[6].time_response =30000;
	sprintf(tab_polecenie[6].rsp,"OK\r");
	tab_polecenie[6].powrot.etap =3;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =6000;
	tab_polecenie[6].iter =3;
	tab_polecenie[6].terminate =0;

	tab_polecenie[7].time_init=500;
	sprintf(tab_polecenie[7].msg,"AT+GSLP=60000\r\n");
	tab_polecenie[7].time_response = 6000;
	sprintf(tab_polecenie[7].rsp,"OK\r");
	tab_polecenie[7].powrot.etap =0;
	tab_polecenie[7].powrot.krok =0;
	tab_polecenie[7].time_exit =200;
	tab_polecenie[7].iter =2;
	tab_polecenie[7].terminate =1;

	if(wifi_set){
		sprintf(tab_polecenie[6].msg,"#WF");
		sprintf(tab_polecenie[6].rsp,"#GT");
	}
}

//used to reset ESP8266 device

void wifi_reset(){

	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+RST\r\n");
	tab_polecenie[0].time_response = 20000;
	sprintf(tab_polecenie[0].rsp,"rea");
	tab_polecenie[0].powrot.etap =0;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =2000;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =1;

}

//communication used to change intermediary AP

void change_wifi(){

	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+CWMODE=3\r\n");
	tab_polecenie[0].time_response = 6000;
	sprintf(tab_polecenie[0].rsp,"OK\r");
	tab_polecenie[0].powrot.etap =3;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =0;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =0;

	tab_polecenie[1].time_init=0;
	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
	tab_polecenie[1].time_response = 4000;
	sprintf(tab_polecenie[1].rsp,"OK\r");
	tab_polecenie[1].powrot.etap = 3;
	tab_polecenie[1].powrot.krok = 0;
	tab_polecenie[1].time_exit = 0;
	tab_polecenie[1].iter =2;
	tab_polecenie[1].terminate =0;

	tab_polecenie[2].time_init=0;
	sprintf(tab_polecenie[2].msg,"AT+CIPSERVER=1,80\r\n");
	tab_polecenie[2].time_response = 4000;
	sprintf(tab_polecenie[2].rsp,"OK\r");
	tab_polecenie[2].powrot.etap = 3;
	tab_polecenie[2].powrot.krok = 0;
	tab_polecenie[2].time_exit = 0;
	tab_polecenie[2].iter =2;
	tab_polecenie[2].terminate =0;

	tab_polecenie[3].time_init=0;
	sprintf(tab_polecenie[3].msg,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifi_read_ssid, wifi_password);
	tab_polecenie[3].time_response =20000;
	sprintf(tab_polecenie[3].rsp,"OK\r");
	tab_polecenie[3].powrot.etap =3;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =2;
	tab_polecenie[3].terminate =0;


	tab_polecenie[4].time_init=0;
	sprintf(tab_polecenie[4].msg,"AT+CWSAP_CUR=\"ESP8266_CHANGE\",\"1234567890\",5,3\r\n");
	tab_polecenie[4].time_response =20000;
	sprintf(tab_polecenie[4].rsp,"OK\r");
	tab_polecenie[4].powrot.etap =4;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =0;
	tab_polecenie[4].iter =2;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=0;
	sprintf(tab_polecenie[5].msg,"AT+CIFSR\r\n");
	tab_polecenie[5].time_response =40000;
	sprintf(tab_polecenie[5].rsp,"RDY");
	tab_polecenie[5].powrot.etap =4;
	tab_polecenie[5].powrot.krok =0;
	tab_polecenie[5].time_exit =0;
	tab_polecenie[5].iter =2;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	sprintf(tab_polecenie[6].msg,"AT+CIPSEND=0,3\r\n");
	tab_polecenie[6].time_response =6000;
	sprintf(tab_polecenie[6].rsp,"OK\r");
	tab_polecenie[6].powrot.etap =4;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =0;
	tab_polecenie[6].iter =2;
	tab_polecenie[6].terminate =0;

	tab_polecenie[7].time_init=0;
	sprintf(tab_polecenie[7].msg,"RDY");
	tab_polecenie[7].time_response =6000;
	sprintf(tab_polecenie[7].rsp,"OK\r");
	tab_polecenie[7].powrot.etap =4;
	tab_polecenie[7].powrot.krok =0;
	tab_polecenie[7].time_exit =0;
	tab_polecenie[7].iter =2;
	tab_polecenie[7].terminate =0;

	tab_polecenie[8].time_init=0;
	sprintf(tab_polecenie[9].msg,"SSID=:\"%s\",SSID=:\"%s\"\r\n", wifi_read_ssid, wifi_password);
	sprintf(tab_polecenie[8].msg,"AT+CIPSEND=0,%d\r\n", strlen(tab_polecenie[9].msg));
	tab_polecenie[8].time_response =20000;
	sprintf(tab_polecenie[8].rsp,"OK\r");
	tab_polecenie[8].powrot.etap =4;
	tab_polecenie[8].powrot.krok =0;
	tab_polecenie[8].time_exit =0;
	tab_polecenie[8].iter =2;
	tab_polecenie[8].terminate =0;

	tab_polecenie[9].time_init=0;
	tab_polecenie[9].time_response =20000;
	sprintf(tab_polecenie[9].rsp,"OK\r");
	tab_polecenie[9].powrot.etap =4;
	tab_polecenie[9].powrot.krok =0;
	tab_polecenie[9].time_exit =0;
	tab_polecenie[9].iter =3;
	tab_polecenie[9].terminate=0;

	tab_polecenie[10].time_init=0;
	sprintf(tab_polecenie[11].msg,"IPIP=:\"%s\"", wifi_list[0]);
	sprintf(tab_polecenie[10].msg,"AT+CIPSEND=0,%d\r\n", strlen(tab_polecenie[11].msg));
	tab_polecenie[10].time_response =20000;
	sprintf(tab_polecenie[10].rsp,"OK\r");
	tab_polecenie[10].powrot.etap =10;
	tab_polecenie[10].powrot.krok =0;
	tab_polecenie[10].time_exit =0;
	tab_polecenie[10].iter =3;
	tab_polecenie[10].terminate =0;

	tab_polecenie[11].time_init=0;
	tab_polecenie[11].time_response =20000;
	sprintf(tab_polecenie[11].rsp,"Rec");
	tab_polecenie[11].powrot.etap =10;
	tab_polecenie[11].powrot.krok =0;
	tab_polecenie[11].time_exit =0;
	tab_polecenie[11].iter =2;
	tab_polecenie[11].terminate=0;

	tab_polecenie[12].time_init=0;
	sprintf(tab_polecenie[12].msg,"AT\r\n");
	tab_polecenie[12].time_response =40000;
	sprintf(tab_polecenie[12].rsp,"##G");
	tab_polecenie[12].powrot.etap =10;
	tab_polecenie[12].powrot.krok =0;
	tab_polecenie[12].time_exit =500;
	tab_polecenie[12].iter =2;
	tab_polecenie[12].terminate =1;
}
