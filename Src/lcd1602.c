#include "lcd1602.h"
#include "main.h"

//Declare functions that are use only within the library
inline void command(uint8_t);
void write4bits(uint8_t);
void send(uint8_t, uint8_t);
void expanderWrite(uint8_t);
void pulseEnable(uint8_t);
void writeNibble(uint8_t);



inline void lcd_write(uint8_t value) {
	send(value, Rs);
}


void LCD_Init(I2C_HandleTypeDef *h, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows){
	lcd.hi2cp=h;
	lcd.Addr=lcd_Addr;
	lcd.cols=lcd_cols;
	lcd.rows=lcd_rows;
	lcd.backlightval=LCD_NOBACKLIGHT;
	lcd.displayfunction=LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (lcd.rows > 1) {
		lcd.displayfunction |= LCD_2LINE;
	}
	lcd.numlines=lcd.rows;

	HAL_Delay(30);

	//Now we pull both RS and R/W low to begin commands
	expanderWrite(lcd.backlightval);
	HAL_Delay(30);

	//put the LCD in 4 bit mode (we start in 8bit mode)
	write4bits(0x03<<4);
	DWT_Delay_us(4500);

	//second try
	write4bits(0x03<<4);
	DWT_Delay_us(4500);

	//third go
	write4bits(0x03<<4);
	DWT_Delay_us(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | lcd.displayfunction);

	// turn the display on with no cursor or blinking default
	lcd.displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD_display();

	// clear it off
	LCD_clear();

	// Initialize to default text direction (for roman languages)
	lcd.displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | lcd.displaymode);

	LCD_home();

}


/********** high level commands, for the user! */
void LCD_clear(){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	DWT_Delay_us(2000);  // this command takes a long time!
}

void LCD_home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	DWT_Delay_us(2000);  // this command takes a long time!
}

void LCD_setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > lcd.numlines ) {
		row = lcd.numlines-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LCD_noDisplay() {
	lcd.displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}
void LCD_display() {
	lcd.displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

// Turns the underline cursor on/off
void LCD_noCursor() {
	lcd.displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}
void LCD_cursor() {
	lcd.displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

// Turn on and off the blinking cursor
void LCD_noBlink() {
	lcd.displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}
void LCD_blink() {
	lcd.displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | lcd.displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCD_scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD_leftToRight(void) {
	lcd.displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | lcd.displaymode);
}

// This is for text that flows Right to Left
void LCD_rightToLeft(void) {
	lcd.displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | lcd.displaymode);
}

// This will 'right justify' text from the cursor
void LCD_autoscroll(void) {
	lcd.displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | lcd.displaymode);
}

// This will 'left justify' text from the cursor
void LCD_noAutoscroll(void) {
	lcd.displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | lcd.displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		lcd_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void LCD_noBacklight(void) {
	lcd.backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LCD_backlight(void) {
	lcd.backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}

/*********** mid level commands, for sending data/cmds */
void writeNibble(uint8_t ch){
	ch|=Rs;
	expanderWrite(ch&0xFB);
	pulseEnable(ch&0xFB);
	DWT_Delay_us(100);
}
inline void command(uint8_t value) {
	send(value, 0);
}


/************ low level data pushing commands **********/


// write either command or data
void send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
    write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void expanderWrite(uint8_t data){
	buf=data|lcd.backlightval;
	HAL_I2C_Master_Transmit(lcd.hi2cp,lcd.Addr<<1,&buf, 1, HAL_MAX_DELAY);
}

void pulseEnable(uint8_t _data){
	expanderWrite(_data | En);		// En high
	DWT_Delay_us(1U);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);		// En low
	DWT_Delay_us(50);		// commands need > 37us to settle
}

void write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}



/*********** Alias functions ****************/
void LCD_load_custom_character(uint8_t char_num, uint8_t *rows){
		LCD_createChar(char_num, rows);
}

void LCD_setBacklight(uint8_t new_val){
	if(new_val){
		LCD_backlight();		// turn backlight on
	}else{
		LCD_noBacklight();		// turn backlight off
	}
}

void LCD_printstr(const char c[]){
	//This function is not identical to the function used for "real" I2C displays
	//it's here so the user sketch doesn't have to be changed
	uint8_t i=0;
	while(c[i]!='\0'){
		LCD_printChar(c[i]);
		i++;
	}

}

void LCD_printChar(uint8_t c){
	writeNibble(c&0xF0);
	writeNibble((c<<4)&0xF0);
}
