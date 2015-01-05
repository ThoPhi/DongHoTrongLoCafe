/*
 *    CFC_LedMatrixSerial.cpp - A library for controling Leds with a MAX7219/MAX7221
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
 

#include "CFC_LedMatrixSerial.h"

CFC_LedMatrixSerial::CFC_LedMatrixSerial(byte numDevices) {
	maxDisplayDevices = numDevices;
    displayCol = numDevices*LED_MATRIX_SIZE;
	displayRow = LED_MATRIX_SIZE;
	
	// bufferMatrix = (byte **) malloc(displayCol);
		// for(byte i=0;i<displayCol;i++)	
			// bufferMatrix[i] = (byte *) malloc(displayRow);
			
	// scanLedBufferGreen = (byte *) malloc(displayCol);
	// scanLedBufferRed = (byte *) malloc(displayCol);
	
		
	// for(byte i=0;i<displayCol;i++)
		// for(byte j=0;j<displayRow;j++)
			// bufferMatrix[i][j] = 0x01;
	// array = malloc(rows * sizeof(int *));
// for (i = 0; i < rows; i++)
  // array[i] = malloc(cols * sizeof(int));

	
	pinMode(SPI_CS_PIN,OUTPUT);
	digitalWrite(SPI_CS_PIN,LOW);
	SPI.begin();
}

byte CFC_LedMatrixSerial::getDeviceCount() {
    return maxDisplayDevices;
}

void CFC_LedMatrixSerial::clearDisplay() {
    for(byte i=0;i<displayCol;i++){
		scanLedBufferGreen[i] = 0x00;
		scanLedBufferRed[i] = 0x00;
	}
}

void CFC_LedMatrixSerial::clearBufferDisplay(){
	for(byte i=0;i<displayCol;i++)
		for(byte j=0;j<displayRow;j++){
			bufferMatrix[i][j] = 0x00;
			// scanLedBufferGreen[i] = 0x00;
			// scanLedBufferRed[i] = 0x00;
		}
}

void CFC_LedMatrixSerial::setLed(byte col, byte row, byte stateLed) {
	if((col>=0 && col<displayCol) && (row>=0 && row<displayRow)){
		bufferMatrix[col][row]  = stateLed;
	}
	
}	

void CFC_LedMatrixSerial::selectFont(const uint8_t * font){
    this->Font = font;
}

uint8_t CFC_LedMatrixSerial::charWidth(const char letter){	
    char c = letter;
    // Space is often not included in font so use width of 'n'
    if (c == ' ') return 1;//c = 'n';
    uint8_t width = 0;

    uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
    uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);

    uint16_t index = 0;

    if (c < firstChar || c >= (firstChar + charCount)) {
	    return 0;
    }
    c -= firstChar;

    if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0) {
	    // zero length is flag indicating fixed width font (array does not contain width data entries)
	    width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
    } else {
	    // variable width font, read width data
	    width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
    }
    return width;
}

byte CFC_LedMatrixSerial::setChar(byte col, byte row, char character, byte state){ 
	if(col >=displayCol || row >= displayRow) return 0;
	char c = character;
	byte height = pgm_read_byte(this->Font + FONT_HEIGHT);
	if(c == ' '){
		byte charWide = 1;//charWidth(' ');
		//this->drawFilledBox(col,row,col + charWide, row+height, LED_NONE);
		return charWide;
	}
    uint8_t width = 0;
    uint8_t bytes = 1;//(height + 7) / 8;
    uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
    uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);
    uint16_t index = 0;
    if (c < firstChar || c >= (firstChar + charCount)) return 0;
    c -= firstChar;
    if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0) {
	    width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
	    index = c * bytes * width + FONT_WIDTH_TABLE;
    } else {
	    for (uint8_t i = 0; i < c; i++) {
	        index += pgm_read_byte(this->Font + FONT_WIDTH_TABLE + i);
	    }
	    index = index * bytes + charCount + FONT_WIDTH_TABLE;
	    width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
    }
	for (byte i=0;i < width;i++){
		byte charCol = pgm_read_byte(this->Font  + index + i);
		for(byte j=0;j<8;j++){
			if((charCol %2)>0)	{
				setLed(col+i,row+j,state);			
			}
			charCol = charCol>>1;
		}
	}
    return width;
}

void CFC_LedMatrixSerial::drawString(int bX, int bY, const char *bChars, byte length, byte color) //writing
{
	if(bX >= displayCol || bY >= displayRow) return;
	uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);
	if (bY+height >= displayRow) return;
	int strWidth = 0;
	for(byte i=0;i<length;i++){
		byte charWide = this->setChar(bX+strWidth, bY, bChars[i], color);
		if(charWide > 0){
			strWidth += charWide;
			strWidth++;
		}
		else if(charWide < 0) return;
		if((bX+strWidth) >= displayCol || (bY >= displayRow)) return;
	}   
}

void CFC_LedMatrixSerial::drawLine(byte x1, byte y1, byte x2, byte y2, byte color){
    signed char  dy = y2 - y1;	//for limited led matrix so value from -128 to 127
    signed char dx = x2 - x1;
    signed char stepx, stepy;
    if (dy < 0) {
	    dy = -dy;
	    stepy = -1;
    } else {
	    stepy = 1;
    }
    if (dx < 0) {
	    dx = -dx;
	    stepx = -1;
    } else {
	    stepx = 1;
    }/////////change from here
    dy <<= 1;			// dy is now 2*dy ???
    dx <<= 1;			// dx is now 2*dx ???

	setLed(x1,y1,color);
	
    //writePixel(x1, y1, bGraphicsMode, true);
    if (dx > dy) {
	    int fraction = dy - (dx >> 1);	// same as 2*dy - dx
	    while (x1 != x2) {
	        if (fraction >= 0) {
		        y1 += stepy;
		        fraction -= dx;	// same as fraction -= 2*dx
	        }
	        x1 += stepx;
	        fraction += dy;	// same as fraction -= 2*dy
			setLed(x1,y1,color);
	        //writePixel(x1, y1, bGraphicsMode, true);
	    }
    } else {
	    int fraction = dx - (dy >> 1);
	    while (y1 != y2) {
	        if (fraction >= 0) {
		        x1 += stepx;
		        fraction -= dy;
	        }
	        y1 += stepy;
	        fraction += dx;
			setLed(x1,y1,color);
	       // writePixel(x1, y1, bGraphicsMode, true);
	    }
    }
}

void CFC_LedMatrixSerial::drawRectangle(byte x1, byte y1, byte x2, byte y2, byte color){
	drawLine(x1, y1, x2, y1, color);
    drawLine(x2, y1, x2, y2, color);
    drawLine(x2, y2, x1, y2, color);
    drawLine(x1, y2, x1, y1, color);
}

void CFC_LedMatrixSerial::drawRectangleFill(byte x1, byte y1, byte x2, byte y2, byte color){
	for (int b = x1; b <= x2; b++) {
	    drawLine(b, y1, b, y2, color);
    }
}

void CFC_LedMatrixSerial::shift(byte dir){
	
	switch(dir){
		case SCROLL_LEFT:
		case SHIFT_LEFT:
				for(byte iRow=0;iRow<displayRow;iRow++){
					tmp = bufferMatrix[0][iRow];
					for(byte iCol=0;iCol<displayCol-1;iCol++){
						bufferMatrix[iCol][iRow] = bufferMatrix[iCol+1][iRow];
					}
					if(dir == SCROLL_LEFT) bufferMatrix[displayCol-1][iRow] = tmp;
					else bufferMatrix[displayCol-1][iRow] = 0x00;
				}
		break;
		case SCROLL_RIGHT:
		case SHIFT_RIGHT:				
			for(byte iRow=0;iRow<displayRow;iRow++){
				tmp = bufferMatrix[displayCol-1][iRow];
				for(byte iCol=displayCol-1;iCol>0;iCol--){
					bufferMatrix[iCol][iRow] = bufferMatrix[iCol-1][iRow];
				}
				if(SCROLL_RIGHT == dir) bufferMatrix[0][iRow] = tmp;
				else bufferMatrix[0][iRow] = 0;
			}
		break;
		case SCROLL_UP:
		case SHIFT_UP:				
				for(byte iCol=0;iCol<displayCol;iCol++){
					tmp = bufferMatrix[iCol][0];
					for(byte iRow=0;iRow<displayRow-1;iRow++)
						bufferMatrix[iCol][iRow] = bufferMatrix[iCol][iRow+1];
					if(SCROLL_UP == dir) bufferMatrix[iCol][displayRow-1] = tmp;
					else bufferMatrix[iCol][displayRow-1] = 0;
				}
		break;
		case SCROLL_DOWN:
		case SHIFT_DOWN:				
			for(byte iCol=0;iCol<displayCol;iCol++){
				tmp = bufferMatrix[iCol][displayRow-1];
				for(byte iRow=displayRow-1;iRow>0;iRow--)
					bufferMatrix[iCol][iRow] = bufferMatrix[iCol][iRow-1];
				if(dir == SCROLL_DOWN) bufferMatrix[iCol][0] = tmp;
				else bufferMatrix[iCol][0] = 0;
			}
		break;
	}
}

void CFC_LedMatrixSerial::printBuffer(){  //print for test
	Serial.println(" print Buffer ");
	for(byte i=0;i<displayCol;i++){
		for(byte j=0;j<displayRow;j++){
			Serial.print(bufferMatrix[displayCol - i -1][j]);	
			Serial.print('_');
		}
		Serial.println();
	}
}

void CFC_LedMatrixSerial::pushBuffer(){	//pushBuffer to display, need more function for vertical
	byte offsetCol,tmpRed,tmpGreen;
	for(byte iDeviceAddr=0 ; iDeviceAddr<maxDisplayDevices ; iDeviceAddr++){
		offsetCol = iDeviceAddr*LED_MATRIX_SIZE;
		for( byte iLedRow=0 ; iLedRow < LED_MATRIX_SIZE; iLedRow++){
			tmpGreen=0;
			tmpRed = 0;
			for( byte iBufferCol= 0 ; iBufferCol <LED_MATRIX_SIZE; iBufferCol++){
				if(iBufferCol != 0 ){
					tmpGreen = tmpGreen>>1;
					tmpRed = tmpRed>>1;
				}
				switch (bufferMatrix[offsetCol + iBufferCol][iLedRow]){
					case LED_NONE:
					break;
					case LED_GREEN:
						tmpGreen |= 0x80;
					break;
					case LED_RED:
						tmpRed |= 0x80;
					break;
					case LED_ORANGE:
						tmpGreen |= 0x80;
						tmpRed |= 0x80;
					break;
				}
			}
			tmpGreen = ((tmpGreen&0x0f)<<4) | ((tmpGreen&0xf0)>>4);
			tmpRed = ((tmpRed&0x0f)<<4) | ((tmpRed&0xf0)>>4);			
			scanLedBufferGreen[offsetCol + iLedRow] = tmpGreen;
			scanLedBufferRed[ offsetCol + iLedRow] = tmpRed;			
		}
	}	
}
	
void CFC_LedMatrixSerial::spiScanLed() {	//spi push to 595, physic, ok
	byte offset;
	//pull latch pin to low level
   digitalWrite(SPI_CS_PIN,LOW);
   for(byte i=0;i<maxDisplayDevices;i++){
   //for(byte i=0;i<1;i++){
	//set select index for ledmatrix
	SPI.transfer(scanLedIndexBuffer[scanLedIndex]);
	offset = i * LED_MATRIX_SIZE;
	//transfer green data coresponding with index
	SPI.transfer(scanLedBufferGreen[offset + scanLedIndex ]);
	//transfer red data base on index
	SPI.transfer(scanLedBufferRed[offset + scanLedIndex]);
   }
   scanLedIndex = (scanLedIndex + 1)%8;
   //latch data of 595 shift register to enable data out
   digitalWrite(SPI_CS_PIN,HIGH);
}    
