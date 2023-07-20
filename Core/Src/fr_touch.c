/*
 * touch.c
 *
 *  Created on: Sep 28, 2022
 *      Author: Filip
 */

#include <fr_touch.h>

// Function to read from xpt2046 touch controller
void xpt2046_read(SPI_HandleTypeDef *_hspi, volatile int i)
{
	// Reset the touch controller chip select
	HAL_GPIO_WritePin(TCH_CS_GPIO_Port, TCH_CS_Pin, GPIO_PIN_RESET);

	uint8_t raw[2] = { 0 };
	uint8_t zero[2] = { 0 };
	uint8_t cmd[1] = { 0 };
	int x = 0;
	int y = 0;
	int mem = 0;

	// These are used to hold the return status of SPI transmit operations
	HAL_StatusTypeDef stat, stat1, stat2, stat3;

	// Loop over the number of samples
	for (uint8_t i = 0; i < XPT2046_SAMPLE; i++) {

		// Prepare command to read x-coordinate
		cmd[0] = RAMKA_X;

		// Send command and receive raw x-coordinate
		stat = HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		stat1 = HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		x += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);

		// Prepare command to read y-coordinate
		cmd[0] = RAMKA_Y;

		// Send command and receive raw y-coordinate
		stat2 = HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		stat3 = HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		y += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);
	}

	// Set the touch controller chip select
	HAL_GPIO_WritePin(TCH_CS_GPIO_Port, TCH_CS_Pin, GPIO_PIN_SET);

	// Calculate the average of the samples
	x /= XPT2046_SAMPLE;
	y /= XPT2046_SAMPLE;

	// Adjust the coordinates based on the display rotation
	switch (ROTACJA_XPT) {
	case 0:
	default:
		break;
	case 1:
		x = XPT_MAX  - x;
		y = XPT_MAX  - y;
		break;
	case 2:
		y = XPT_MAX  - y;
		break;
	case 3:
		x = XPT_MAX  - x;
		break;
	}

	// Calibration adjustment for x and y
	mem=x;
	x=(x*(WA)+(y*WB)+(WC))/(WDEN);
	y=(mem*(WD)+(y*WE)+(WF))/(WDEN);

	// Limiting the x and y values to display resolution
	if(x<0){
		x=0;
	}
	else if(x>320){
		x=320;
	}
	else if(y<0){
		y=0;
	}
	else if(y>280){
		y=280;
	}

	// Store the calibrated coordinates
	touchtable[i].x= x;
	touchtable[i].y= y;
}

// Function to handle touch event states
void dotyk(SPI_HandleTypeDef *hspi){
	switch(TouchState){
		case XPT2046_IDLE:
			// If the touch state is idle, do nothing
			break;

		case XPT2046_PRESAMPLING:
			// If the touch state is in pre-sampling, sample the touch coordinates
			if(touchsample < MAXSAMPLE){
				xpt2046_read(hspi, touchsample);
				touchsample++;
			}

			// After sampling MAXSAMPLE times, transition to the touched state
			if(touchsample == MAXSAMPLE){
				TouchState = XPT2046_TOUCHED;
				touchsample = 0;
				touchsample1 = 0;
			}

			// If the touch has been released, transition to the released state
			if(GPIO_PIN_SET == HAL_GPIO_ReadPin(TCH_IRQ_GPIO_Port, TCH_IRQ_Pin)){
				TouchState = XPT2046_RELEASED;
			}
			break;

		case XPT2046_TOUCHED:
			// If the touch state is in touched, sample the touch coordinates again until MAXSAMPLE is reached
			if(touchsample1 < MAXSAMPLE){
				xpt2046_read(hspi, touchsample1);
				touchsample1++;
			}

			// If the touch has been released, transition to the released state
			if(GPIO_PIN_SET == HAL_GPIO_ReadPin(TCH_IRQ_GPIO_Port, TCH_IRQ_Pin)){
				TouchState = XPT2046_RELEASED;
				touchsample1 = 0;
			}
			break;

		case XPT2046_RELEASED:
			// If the touch state is in released, reset all the samples and transition to idle state
			touchsample = 0;
			touchsample1 = 0;
			TouchState = XPT2046_IDLE;

			// Clear all interrupt flags
			while(HAL_NVIC_GetPendingIRQ(EXTI2_IRQn)){
				__HAL_GPIO_EXTI_CLEAR_IT(TCH_IRQ_Pin);
				HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
			}

			// Enable the interrupt for the next touch event
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);
			break;
	}
}

// function fired by touch screen interrupt
void touch_irq(void)
{
	// Disable IRQ for avoid false IRQs(Datasheet)
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	// Jump to PRESAMPLING state
	TouchState = XPT2046_PRESAMPLING;
	// start sampling from the beginging
	touchsample = 0;
	touchsample1 = 0;
}

// Function to calibrate the touch screen
void kalibracja(SPI_HandleTypeDef *_hspi)
{
	touch wynik;
	wsp wsp;

	// Predefined screen points for calibration
	int xa=20, xb=22;
	int ya=20, yb=22;

	calib P1={.x=21,.y=21}; // Calibration point P1

	// Buffers for LCD data
	uint16_t buff[9]={0};

	touch T1={.x=0,.y=0}; // Touch point corresponding to calibration point P1

	lcd_copy(xa,xb,ya,yb,buff); // Copy LCD data to buffer
	printf("Wcis"); // Print a message
	HAL_Delay(4000); // Wait for 4 seconds

	// Read touch coordinates if screen is touched
	if(HAL_GPIO_ReadPin(TCH_IRQ_GPIO_Port, TCH_IRQ_Pin)==0)
	{
		// Read touch coordinates 3 times
		for(int i=0;i<3;i++)
		{
			xpt2046_read(_hspi, 1);
			printf("odczytano pozycje: %d, %d\n\r", wynik.x, wynik.y);
			T1.x+=wynik.x;
			T1.y+=wynik.y;
			HAL_Delay(300);
		}

		// Calculate average touch coordinates for point T1
		T1.x/=3;
		T1.y/=3;
	}

	// Repeat the process for calibration points P2 and P3
	xa=300, xb=302;
	ya=220, yb=222;
	calib P2={.x=301,.y=221}; // Calibration point P2
	touch T2={.x=0,.y=0}; // Touch point corresponding to calibration point P2

	lcd_copy(xa,xb,ya,yb,buff);
	printf("Wcis");
	HAL_Delay(4000);
	if(HAL_GPIO_ReadPin(TCH_IRQ_GPIO_Port, TCH_IRQ_Pin)==0)
	{
		for(int i=0;i<3;i++)
		{
			xpt2046_read(_hspi, &wynik);
			printf("odczytano pozycje: %d, %d\n\r", wynik.x, wynik.y);
			T2.x+=wynik.x;
			T2.y+=wynik.y;
			HAL_Delay(300);
		}
		T2.x/=3;
		T2.y/=3;
	}

	xa=310, xb=312;
	ya=20, yb=22;
	calib P3={.x=311,.y=21}; // Calibration point P3
	touch T3={.x=0,.y=0}; // Touch point corresponding to calibration point P3

	lcd_copy(xa,xb,ya,yb,buff);
	printf("Wcis");
	HAL_Delay(4000);
	if(HAL_GPIO_ReadPin(TCH_IRQ_GPIO_Port, TCH_IRQ_Pin)==0)
	{
		for(int i=0;i<3;i++)
		{
			xpt2046_read(_hspi, 1);
			printf("odczytano pozycje: %d, %d\n\r", wynik.x, wynik.y);
			T3.x+=wynik.x;
			T3.y+=wynik.y;
			HAL_Delay(300);
		}
		T3.x/=3;
		T3.y/=3;
	}

	// Compute calibration parameters based on the touched and calibration points
	// Following formulae are used to convert the touch coordinates into screen coordinates
	// Using a 2D affine transformation
	wsp.DEN = ((T1.x - T3.x) * (T2.y - T3.y))-((T2.x - T3.x) * (T1.y - T3.y));
	wsp.A = ((P1.x - P3.x) * (T2.y - T3.y))-((P2.x - P3.x) * (T1.y - T3.y));
	wsp.B = ((T1.x - T3.x) * (P2.x - P3.x))-((P1.x - P3.x) * (T2.x - T3.x));
	wsp.C= (T3.x * P2.x - T2.x * P3.x) * T1.y+(T1.x * P3.x - T3.x * P1.x) * T2.y +
					(T2.x * P1.x - T1.x * P2.x) * T3.y;
	wsp.D = ((P1.y - P3.y) * (T2.y - T3.y))-((P2.y - P3.y) * (T1.y - T3.y));
	wsp.E = ((T1.x - T3.x) * (P2.y - P3.y))-((P1.y - P3.y) * (T2.x - T3.x));
	wsp.F = (T3.x * P2.y - T2.x * P3.y) * T1.y+(T1.x * P3.y - T3.x * P1.y) * T2.y +
					(T2.x * P1.y - T1.x * P2.y) * T3.y;

	// Scaling down calibration parameters
	wsp.A/=10;
	wsp.B/=10;
	wsp.C/=10;
	wsp.D/=10;
	wsp.E/=10;
	wsp.F/=10;
	wsp.DEN/=10;

	// Print calibration parameters
	printf("Wynik DEN: %d\n\r", wsp.DEN);
	printf("Wynik A: %d\n\r", wsp.A);
	printf("Wynik B: %d\n\r", wsp.B);
	printf("Wynik C: %d\n\r", wsp.C);
	printf("Wynik D: %d\n\r", wsp.D);
	printf("Wynik E: %d\n\r", wsp.E);
	printf("Wynik F: %d\n\r", wsp.F);
	printf("Wynik x: %d\n\r", wynik.x);
	printf("Wynik y: %d\n\r", wynik.y);
}

