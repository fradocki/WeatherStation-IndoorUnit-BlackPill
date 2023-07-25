/*
 * sd_logger.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Filip
 */
#include "fr_sd_logger.h"

// Function to write sensor data at a particular position in the file on the SD card.
void write_data_at_position(struct measure_type * measure_tab){
	unsigned int bytesWritten;
	char temp_file_name[28]="\0";
	int result=-1;
    RTC_DateTypeDef data_measure= {0};
    RTC_TimeTypeDef time_measure= {0};

	HAL_RTC_GetTime(&hrtc, &time_measure, RTC_FORMAT_BIN);
	unsigned long position = calculate_3min_periods_elapsed(time_measure);

	HAL_RTC_GetDate(&hrtc, &data_measure, RTC_FORMAT_BIN);

	// Mount the SD card
 	if (!mount_sd_card()){
		return 0;
	}
    // Iterate over the measurement data one time to not block other parts of program
    if(iter_sd_card < (measure_tab->iter)){
        strcpy(temp_file_name, measure_to_file_name[measure_tab->ind[iter_sd_card]]);
        sprintf(temp_file_name, "%s_%02u_%02u_%04u.txt", temp_file_name, data_measure.Date, data_measure.Month, (data_measure.Year+rok));

        //Open the data file in read-write mode
        int file_open_result = f_open(&file, temp_file_name, FA_OPEN_EXISTING | FA_READ | FA_WRITE);

        // File handling operations depending on the file open result
        handleFileOperations(file_open_result, measure_tab, iter_sd_card, position, temp_file_name);

        iter_sd_card++;
    }
    else{
    	iter_sd_card=0;
    }
	done++;
}

// Function to read sensor data from a particular position in the file on the SD card.
uint8_t read_data_from_position(int measure_ind, int data) {
	struct sensor_table sensor_data_read_temp[480] = {0};
	char temp_file_name[28] = "\0";
	uint8_t rysuj = 0;
	int result = -1;
    RTC_DateTypeDef data_measure= {0};

	// Get the current date
	HAL_RTC_GetDate(&hrtc, &data_measure, RTC_FORMAT_BIN);
	unsigned int bytesRead;

	// Based on the type of measurement, choose the correct filename
	strcpy(temp_file_name,measure_to_file_name[measure_ind]);


	// Choose the correct file based on the date required
	switch(data){
	case dzisiaj:
		sprintf(temp_file_name, "%s_%02u_%02u_%04u.txt",temp_file_name,data_measure.Date,data_measure.Month,(data_measure.Year+rok));
		break;
	case wczoraj:
		calculate_previous_day(&data_measure);
		sprintf(temp_file_name, "%s_%02u_%02u_%04u.txt",temp_file_name,data_measure.Date,data_measure.Month,(data_measure.Year+rok));
		break;
	case przedwczoraj:
		calculate_previous_day(&data_measure);
		calculate_previous_day(&data_measure);
		sprintf(temp_file_name, "%s_%02u_%02u_%04u.txt",temp_file_name,data_measure.Date,data_measure.Month,(data_measure.Year+rok));
		break;
	}

	// Mount the SD card
	if (!mount_sd_card()){
		return 0;
	}

	// Open the file and read the data
	if (f_open(&file, temp_file_name, FA_OPEN_EXISTING | FA_READ | FA_WRITE) != FR_OK){
		send_uart("Failed to open file.");
		return rysuj=0;
	}
	;
	if (f_read(&file, sensor_data_read_temp, sizeof(sensor_data_read_temp), &bytesRead) != FR_OK){
		send_uart("Failed to write data to file.");
		return rysuj=0;
	}

	// Based on the type of measurement, process the data accordingly
	rysuj = processData(measure_ind, sensor_data_read_temp);


	// Close the file after reading the data
	f_sync(&file);
	f_close(&file);
	return rysuj;
}

// Function to mount the SD card
int mount_sd_card(){
	int result = -1;
	if((result = (f_mount(&fs, "", 1))) != FR_OK){
		char str_temp[50]={'\0'};
		sprintf(str_temp, "ERROR!!! in mounting SD CARD... %d\n\n", result);
		send_uart(str_temp);
		return 0;
	}
	else{
		send_uart("SD CARD mounted successfully...\n\n");
		return 1;
	}
}

// Function to  open - create and open file if no existed
void handleFileOperations(int file_open_result, struct measure_type * measure_tab, int i, unsigned long position, char *temp_file_name, char *msg, char *msg_help) {
    // File opened successfully
    if (file_open_result == FR_OK){
        processFile(measure_tab, i, position, msg, msg_help);
    }
    // File does not exist, create and write to it
    else if(file_open_result == FR_NO_FILE){
        createAndWriteFile(measure_tab, i, position, temp_file_name);
    }
    // File opening failed
    else{
        send_uart("Failed to open file.");
        return;
    }
    // Close the file after all operations are done
    f_close(&file);
}

// Function to wirte single data block and given positon in a file
void processFile(struct measure_type * measure_tab, int i, unsigned long position){
	char msg[165]="\0"; // Variable to hold the success message.
	char msg_help[55]="\0"; // Helper variable for creating the success message.

    // Move the file pointer to the specified position
    if (f_lseek(&file, 2*position) != FR_OK) {
        send_uart("Failed to move file pointer.");
        f_close(&file);
        return;
    }

    // Write the data to the file
    if (f_write(&file, measure_tab->pomiar+i, 2, &measure_tab->bytes_writen[i]) != FR_OK){
        send_uart("Failed to write to file.");
        f_close(&file);
        measure_tab->bytes_writen[i]=0;
        return;
    }
    else{
        if(measure_tab->bytes_writen[i]){
            sprintf(msg_help, "Fine to copy %d to file %s\r\n", (int)measure_tab->bytes_writen[i], measure_to_file_name[measure_tab->ind[i]]);
            strcat(msg, msg_help);
        }
    }

    //print result of the function to external USART
    send_uart(msg);
}

// This function is used to create a file and initialize it with data.
void createAndWriteFile(struct measure_type * measure_tab, int i, unsigned long position, char *temp_file_name){
    unsigned int bytesWritten;
	char msg[186]="\0"; // Variable to hold the success message.
	char msg_help[62]="\0"; // Helper variable for creating the success message.

    // Try to open the file in read-write mode. If the file does not exist, create it.
    if(f_open(&file, temp_file_name, FA_OPEN_ALWAYS | FA_READ | FA_WRITE)==FR_OK){
        struct sensor_table sensor_data_write[480]={0}; // Create an array to store sensor data.

        // Store the measurement data in the appropriate position.
        sensor_data_write[position].num=measure_tab->pomiar[i].num;
        sensor_data_write[position].tenths=measure_tab->pomiar[i].tenths;

        // Attempt to write the sensor data to the file.
        if(f_write(&file, sensor_data_write, sizeof(sensor_data_write), &bytesWritten)!= FR_OK){
            // If writing fails, send an error message, close the file, and return.
            send_uart("Failed to write to file.");
            f_close(&file);
            measure_tab->bytes_writen[i]=0;
            return;
        }
        else{
            //If the write operation was successful, create a success message.
            if(measure_tab->bytes_writen[i]){
                sprintf(msg_help, "Fine to create and copy %d to file %s\r\n", (int)measure_tab->bytes_writen[i], measure_to_file_name[measure_tab->ind[i]]);
                strcat(msg, msg_help);
            }
        }
    }
    else{
        //If opening or creating the file fails, send an error message and return.
        send_uart("Failed to open file.");
        return;
    }

    //print result of the function to external USART
    send_uart(msg);
}

// Function to process data read from file depending on a data type read
uint8_t processData(int measure_ind, struct sensor_table * sensor_data_read_temp){
    uint8_t rysuj = 0;
    uint8_t factor=10;

    if((measure_ind == CO2_IN) || (measure_ind == Dust_OUT) || (measure_ind == Pres_OUT)) {
        factor=100;
    }

    for(int i = 0; i < 96; i++){
        sensor_data_read[i] = (sensor_data_read_temp[i * 5].num) * factor + sensor_data_read_temp[i * 5].tenths;
        rysuj = 1;
    }

    return rysuj;
}

// Function to calculate number of 3-minute periods elapsed from midnight till the given time.
unsigned long calculate_3min_periods_elapsed(RTC_TimeTypeDef time_measure) {
  int hours = time_measure.Hours;
  int minutes = time_measure.Minutes;
  int totalMinutes = hours * 60 + minutes; // convert hours to minutes and add to minutes
  unsigned long periodsElapsed = totalMinutes / 3; // calculate how many 3-minute periods have elapsed
  return periodsElapsed;
}

// This function initializes the measure_tab and sensor_data_table arrays.
void measure_init() {
    /* Initializing the first element of measure_tab array */
    measure_tab[0].iter = 3;  // Number of measurements for the first element
    // assign index values corresponding to measure_ind enum
    measure_tab[0].ind[0]=Humi_IN;
    measure_tab[0].ind[1]=Temp_IN;
    measure_tab[0].ind[2]=CO2_IN;
    measure_tab[0].bytes_writen[0] = 0; // Bytes written for humidity inside set to 0
    measure_tab[0].bytes_writen[1] = 0; // Bytes written for temperature inside set to 0
    measure_tab[0].bytes_writen[2] = 0; // Bytes written for CO2 inside set to 0

    /* Initializing the second element of measure_tab array */
    measure_tab[1].iter = 4;  // Number of measurements for the second element
    // assign index values corresponding to measure_ind enum
    measure_tab[1].ind[0]=Humi_OUT;
    measure_tab[1].ind[1]=Temp_OUT;
    measure_tab[1].ind[2]=Pres_OUT;
    measure_tab[1].ind[3]=Dust_OUT;
    measure_tab[1].bytes_writen[0] = 0; // Bytes written for humidity outside set to 0
    measure_tab[1].bytes_writen[1] = 0; // Bytes written for temperature outside set to 0
    measure_tab[1].bytes_writen[2] = 0; // Bytes written for pressure outside set to 0
    measure_tab[1].bytes_writen[3] = 0; // Bytes written for dust outside set to 0

    /* Initializing sensor_data_table array */
    sensor_data_table[0].letter = 'W'; // Letter representation for first type of sensor data
    sensor_data_table[1].letter = 'T'; // Letter representation for second type of sensor data
    sensor_data_table[2].letter = 'P'; // Letter representation for third type of sensor data
    sensor_data_table[3].letter = 'B'; // Letter representation for fourth type of sensor data
    sensor_data_table[4].letter = 'D'; // Letter representation for fifth type of sensor data
    sensor_data_table[0].ind = 0; // Index for first type of sensor data
    sensor_data_table[1].ind = 1; // Index for second type of sensor data
    sensor_data_table[2].ind = 2; // Index for third type of sensor data
    sensor_data_table[3].ind = 3; // Index for fourth type of sensor data
    sensor_data_table[4].ind = 4; // Index for fifth type of sensor data
}

// Calculates the date for the previous day given a current date
void calculate_previous_day(RTC_DateTypeDef *current_date) {
    // Get the current year
    uint16_t year = current_date->Year + rok;

    // If the current day of the month is greater than 1
    if (current_date->Date > 1) {
        // Just subtract one day
        current_date->Date -= 1;
    }
    else {
        // If the current month is March and it is a leap year
        if (current_date->Month == 3 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
            // Leap year - February has 29 days
            current_date->Month = 2;
            current_date->Date = 29;
        }
        // If the current month is March and it is not a leap year
        else if (current_date->Month == 3) {
            // Not a leap year - February has 28 days
            current_date->Month = 2;
            current_date->Date = 28;
        }
        // If the current month is May, July, October, or December
        else if (current_date->Month == 5 || current_date->Month == 7 || current_date->Month == 10 || current_date->Month == 12) {
            // These months have 31 days
            current_date->Month -= 1;
            current_date->Date = 31;
        }
        // If the current month is January
        else if (current_date->Month == 1) {
            // It's January 1st - need to go back to December 31st of the previous year
            if(year > 0) {
                year -= 1;
                current_date->Month = 12;
                current_date->Date = 31;
            }
        }
        else {
            // All other months have 30 days
            current_date->Month -= 1;
            current_date->Date = 30;
        }
    }

    // Adjust the year in the date structure
    current_date->Year = year - rok;
}
