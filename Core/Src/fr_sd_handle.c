/*
 * sd_handle.c
 *
 *  Created on: 25 lut 2023
 *      Author: Filip
 */

#include "fr_sd_handle.h"

extern FATFS fs;  // file system
extern FIL fil; // File
extern FILINFO fno;
extern FRESULT fresult;  // result
extern UINT br, bw;  // File read/write count

/**** capacity related *****/
extern FATFS *pfs;
extern DWORD fre_clust;
extern uint32_t total, free_space;
extern char buffer_sd[BUFFER_SIZE];  // to store strings..
int i=0;
extern UART_HandleTypeDef huart2;

int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer (void)
{
	for (int i=0; i<BUFFER_SIZE; i++) buffer_sd[i] = '\0';
}

void send_uart (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart2, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}

void sd_mount(void){
	  HAL_Delay(1000);
	  fresult = f_mount(&fs, "/", 1);
	  	if (fresult != FR_OK) send_uart ("ERROR!!! in mounting SD CARD...\n\n");
	  	else send_uart("SD CARD mounted successfully...\n\n");
}

void sd_free_space(void){
  	f_getfree("", &fre_clust, &pfs);
  	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  	sprintf (buffer_sd, "SD CARD Total Size: \t%lu\n",total);
  	send_uart(buffer_sd);
  	clear_buffer();
  	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
  	sprintf (buffer_sd, "SD CARD Free Space: \t%lu\n\n",free_space);
  	send_uart(buffer_sd);
  	clear_buffer();
}
void sd_write(void){
	/* Open file to write/ create a file if it doesn't exist */
      fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

  	/* Writing text */
  	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

  	/* Close file */
  	fresult = f_close(&fil);

  	if (fresult == FR_OK)send_uart ("File1.txt created and the data is written \n");
}
void sd_read(void){
  	/* Open file to read */
  	fresult = f_open(&fil, "file1.txt", FA_READ);

  	/* Read string from the file */
  	f_gets(buffer_sd, f_size(&fil), &fil);

  	send_uart("File1.txt is opened and it contains the data as shown below\n");
  	send_uart(buffer_sd);
  	send_uart("\n\n");

  	/* Close file */
  	f_close(&fil);

  	clear_buffer();
}



