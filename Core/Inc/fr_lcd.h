#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "fr_global.h"


#define LCD_WIDTH	320
#define LCD_HEIGHT	240

//Definicje kolorów z odwróconą kolejnością bajtów
#define BLACK			0x0000
#define RED				0x00f8
#define GREEN			0xe007
#define BLUE			0x1f00
#define YELLOW			0xe0ff
#define MAGENTA			0x1ff8
#define CYAN			0xff07
#define WHITE			0xffff

void lcd_init(void); // Inicjalizacja wyświetlacza
void lcd_put_pixel(int x, int y, uint16_t color);
void lcd_copy(int x1, int x2, int y1, int y2, uint16_t * bufor); // Przesłanie zawartości bufora
void lcd_transfer_done(void);
bool lcd_is_busy(void);
void lcd_cmd(uint8_t);

extern SPI_HandleTypeDef hspi1;
