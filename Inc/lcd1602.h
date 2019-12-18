#ifndef LCD1602_H
#define LCD1602_H

#include "stm32f3xx_hal.h"
#include "dwt_stm32_delay.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit


struct LiquidCrystal_I2C{
	I2C_HandleTypeDef* hi2cp;
	uint8_t Addr,
	displayfunction,
	displaycontrol,
	displaymode,
	numlines,
	cols,
	rows;
	uint8_t backlightval;

}lcd;

uint8_t buf;

void LCD_Init(I2C_HandleTypeDef *h, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
void LCD_clear();
void LCD_home();
void LCD_noDisplay();
void LCD_display();
void LCD_noBlink();
void LCD_blink();
void LCD_noCursor();
void LCD_cursor();
void LCD_scrollDisplayLeft();
void LCD_scrollDisplayRight();
void LCD_leftToRight();
void LCD_rightToLeft();
void LCD_noBacklight();
void LCD_backlight();
void LCD_autoscroll();
void LCD_noAutoscroll();
void LCD_createChar(uint8_t, uint8_t[]);
void LCD_setCursor(uint8_t, uint8_t);
void LCD_printChar(uint8_t);
void LCD_printstr(const char c[]);
void LCD_load_custom_character(uint8_t char_num, uint8_t *rows);
void LCD_setBacklight(uint8_t);


#endif
