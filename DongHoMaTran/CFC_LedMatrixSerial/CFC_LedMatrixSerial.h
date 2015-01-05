/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _CFC_LED_MATRIX_SERIAL_H_
#define _CFC_LED_MATRIX_SERIAL_H_

#define LED_VERTICAL

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pins_arduino.h"
#include <avr/pgmspace.h>
#include <SPI.h>
//color led defination
#define LED_NONE	0
#define LED_GREEN	1 
#define LED_RED		2 
#define LED_ORANGE	3
//hardware defination
#define SPI_CS_PIN	10 
#define LED_MATRIX_SIZE	8
#define LED_MATRIX_ROW 8
#define LED_MATRIX_COL 24
//font defination
#define FONT_LENGTH             0
#define FONT_FIXED_WIDTH        2
#define FONT_HEIGHT             3
#define FONT_FIRST_CHAR         4
#define FONT_CHAR_COUNT         5
#define FONT_WIDTH_TABLE        6

#define NONE 0
#define BLINK 1
#define SHIFT_LEFT	2  
#define SHIFT_RIGHT 3  
#define SHIFT_UP	4 
#define SHIFT_DOWN 5
#define SCROLL_LEFT	6 
#define SCROLL_RIGHT 7
#define SCROLL_UP 8
#define SCROLL_DOWN	9
#define TEST_MATRIX	10  

static byte scanLedIndexBuffer[8] = {0x10,0x20,0x40,0x80,0x01,0x02,0x04,0x08};

class CFC_LedMatrixSerial {
 public:
    /* 
     * Create a new controler 
     * Params :
     * //dataPin		pin on the Arduino where data gets shifted out
     * //clockPin		pin for the clock
     * //csPin		pin for selecting the device 
     * numDevices	maximum number of devices that can be controled
     */
    CFC_LedMatrixSerial(byte numDevices=3);

    /*
     * Gets the number of devices attached to this LedControl.
     * Returns :
     * int	the number of devices on this LedControl
     */
    byte getDeviceCount();

    /* 
     * Switch all Leds on the display off. 
     * Params:
     * addr	address of the display to control
     */
    void clearDisplay();
	void clearBufferDisplay();

    /* 
     * Set the status of a single Led.
     * Params :
     * //addr	address of the display 
     * row	the row of the Led (0..7)
     * col	the column of the Led (0..7)
     * state	If true the led is switched on, 
     *		if false it is switched off
     */
    void setLed(byte col, byte row, byte state);
	
	/*
	*	setFont for matrix panel
	*	Params:
	*	font: array of font data
	*/
	void selectFont(const uint8_t* font);
	
	/*
	*	Set character on led matrix
	*	Params:
	*	col & row : position for begin text 
	* 	character: which character want draw 
	* 	state: color 
	*/
	byte setChar(byte col, byte row, char character, byte state);
	uint8_t charWidth(const char letter);
	void drawString(int bX, int bY, const char *bChars, byte length, byte color) ;
	
	void drawLine(byte x1, byte y1, byte x2, byte y2, byte color);
	void drawRectangle(byte x1, byte y1, byte x2, byte y2, byte color);
	void drawRectangleFill(byte x1, byte y1, byte x2, byte y2, byte color);

	void shift(byte dir);
	/*
	*	Scan Led Matrix by send spi data
	* 	Should call in main for fine view
	*/
	void spiScanLed();
	
	/*
	*	Push buffermatrix to buffer scan
	*	Call each time updated
	*/	
	void pushBuffer();
	
	/*
	*	printBuffer for test
	*/
	void printBuffer();
	
 private :
	//byte **bufferMatrix;
	byte bufferMatrix[LED_MATRIX_COL][LED_MATRIX_ROW];	
	
	//byte *scanLedBufferGreen;//[displayCol][displayRow];
	//byte *scanLedBufferRed;//[displayCol][displayRow];
	byte scanLedBufferGreen[LED_MATRIX_COL];
	byte scanLedBufferRed[LED_MATRIX_COL];

	
 byte maxDisplayDevices;
	byte displayCol;
	byte displayRow;
	byte tmp;
	byte scanLedIndex ;
	const uint8_t* Font;
	
   //   /* The array for shifting the data to the devices */
    // byte spidata[16];
    // /* Send out a single command to the device */
    // void spiTransfer(int addr, byte opcode, byte data);

    // /* We keep track of the led-status for all 8 devices in this array */
    // byte status[64];
    // /* Data is shifted out of this pin*/
    // int SPI_MOSI;
    // /* The clock is signaled on this pin */
    // int SPI_CLK;
    // /* This one is driven LOW for chip selectzion */
    // int SPI_CS;
    // /* The maximum number of devices we use */
    // int maxDevices;     
};

#endif	
