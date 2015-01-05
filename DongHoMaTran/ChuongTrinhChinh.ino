#include <stdio.h>
#include <string.h>
#define MODE_SHOW_TIME   0
#define MODE_TEXT        1

#define SHOW_TIME        0
#define SHOW_DATE        1
#define SHOW_DATE_WEEK   2
#define SHOW_YEAR        3

byte modeStateCount;
byte showState = 0;
byte lastShowState =100;
byte firstNumber;
byte secondNumber;
byte thirstNumber;
byte lastNumber;
unsigned int countShowState;
unsigned int tmp;
byte iRun;

void run(){
  switch(runMode){
  case MODE_SHOW_TIME:
    show_time();
    break;
  case MODE_TEXT:
    show_text();
    break;
  default: 
    break;
  }
}

void show_time(){
  switch(showState){
  case SHOW_TIME:
    firstNumber = hourFormat12();
    secondNumber = minute();
    if(secondNumber != lastNumber || showState != lastShowState){      
      if(hour()==0 && minute() == 0){
        setSyncProvider(RTC.get);  //đồng bộ thời gian lại với RTC
      }
//      Serial.print("time is: ");
//      Serial.print(firstNumber);
//      Serial.print(' ');
//      Serial.println(secondNumber);
      byte color = LED_RED;
      led.clearBufferDisplay();
      if(firstNumber<10){
        led.setChar(2,0,firstNumber + '0',LED_GREEN);
        led.setChar(7,0,':',LED_RED);        
        led.setChar(11,0,secondNumber/10 + '0',LED_GREEN);
        led.setChar(17,0,secondNumber%10 + '0',LED_GREEN);
      }
      else{
        led.setChar(0,0,firstNumber/10 + '0',LED_GREEN);  //jusi number 1
        led.shift(SHIFT_LEFT);
        led.setChar(4,0,firstNumber%10 + '0',LED_GREEN);
        led.setChar(9,0,':',LED_RED);
        led.setChar(13,0,secondNumber/10 + '0',LED_GREEN);
        led.setChar(19,0,secondNumber%10 + '0',LED_GREEN);
      }
      if(hour() >= 12) {  //pm
        led.setLed(10,7,LED_RED);
        led.setLed(11,7,LED_RED);
        led.setLed(8,7,LED_RED);
        led.setLed(9,7,LED_RED);
      }      
      led.pushBuffer();
    }
    else{
      thirstNumber = second();
      if(firstNumber<10){
        led.setChar(7,0,':',thirstNumber%4);
      }
      else{
        led.setChar(9,0,':',thirstNumber%4);
      }    
      led.pushBuffer();
    }
    lastNumber = secondNumber;
    break;
  case SHOW_DATE:
    firstNumber = day();
    secondNumber = month();
    if(firstNumber != lastNumber || showState != lastShowState){      
      Serial.print("day: ");
      Serial.print(firstNumber);
      Serial.print(" - ");
      Serial.println(secondNumber);
      byte color = LED_GREEN;
      led.clearBufferDisplay();
      if(secondNumber<10){
        if(firstNumber>=10) led.setChar(0,0,firstNumber/10 + '0',color);
        led.setChar(5,0,firstNumber%10 + '0',color);
        led.setChar(11,0,'-',LED_ORANGE);
        led.setChar(17,0,secondNumber + '0',color);
      }
      else {
        if(firstNumber>=10)led.setChar(0,0,firstNumber/10 + '0',color);
        led.setChar(6,0,firstNumber%10 + '0',color);
        led.setChar(11,0,'-',LED_ORANGE);
        led.setChar(15,0,secondNumber/10 + '0',color);
        led.setChar(19,0,secondNumber%10 + '0',color);
      }
      led.pushBuffer();
    }
    lastNumber = firstNumber;
    break;
  case SHOW_DATE_WEEK:    
    firstNumber = weekday();
    if(firstNumber != lastNumber || showState != lastShowState){
      Serial.println(firstNumber);
      led.clearBufferDisplay();
      switch(firstNumber){
      case 1:  //sunday
        led.setChar(3,1,'S',LED_RED);
        led.setChar(8,1,'u',LED_ORANGE);
        led.setChar(13,1,'n',LED_GREEN);
        break;      
      case 2:
        led.setChar(3,1,'M',LED_GREEN);
        led.setChar(8,1,'o',LED_GREEN);
        led.setChar(13,1,'n',LED_GREEN);
        break;
      case 3:
        led.setChar(3,1,'T',LED_GREEN);
        led.setChar(8,1,'u',LED_GREEN);
        led.setChar(13,1,'e',LED_GREEN);
        break;
      case 4:
        led.setChar(3,1,'W',LED_GREEN);
        led.setChar(8,1,'e',LED_GREEN);
        led.setChar(13,1,'d',LED_GREEN);
        break;
      case 5:
        led.setChar(3,1,'T',LED_GREEN);
        led.setChar(8,1,'h',LED_GREEN);
        led.setChar(13,1,'u',LED_GREEN);
        break;
      case 6:
        led.setChar(3,1,'F',LED_GREEN);
        led.setChar(8,1,'r',LED_GREEN);
        led.setChar(13,1,'i',LED_GREEN);
        break;
      case 7:
        led.setChar(3,1,'S',LED_GREEN);
        led.setChar(8,1,'a',LED_GREEN);
        led.setChar(13,1,'t',LED_GREEN);
        break;
      }
      led.pushBuffer();
    }
    lastNumber = firstNumber;
    break;
  case SHOW_YEAR:
    tmp = year();
    firstNumber = tmp%100;
    if(lastNumber != firstNumber || showState != lastShowState){
      led.clearBufferDisplay();
      led.setChar(0,0,tmp/1000 + '0',LED_GREEN);
      led.setChar(6,0,(tmp/100)%10 + '0',LED_GREEN);
      led.setChar(12,0,(tmp/10)%10 + '0',LED_GREEN);
      led.setChar(18,0,tmp%10 + '0',LED_GREEN);      
      led.pushBuffer();
    }
    lastNumber = firstNumber;
    break;
  default: 
    break;
  }
  lastShowState = showState;
}

void show_text(){
  byte randomColor = (byte) random(1,3);
  switch(showState){
  case 0:  //Hello blink the shift left
    if(lastShowState != showState){
      countShowState = 0;
      led.clearBufferDisplay();
      led.setChar(0,1,'H',randomColor);
      randomColor = (byte) random(1,4);
      led.setChar(5,1,'e',randomColor);
      randomColor = (byte) random(1,4);
      led.setChar(9,1,'l',randomColor);
      randomColor = (byte) random(1,4);
      led.setChar(12,1,'l',randomColor);
      randomColor = (byte) random(1,4);
      led.setChar(16,1,'o',randomColor);
      led.pushBuffer();
    }
    else if(countShowState <20){
      if(countShowState%4 <2){
        led.clearDisplay();
      }
      else led.pushBuffer();
    }
    else {    
      led.shift(SCROLL_LEFT);
      led.pushBuffer();
    }
    countShowState++;
    break;
  case 1:  //Happy green
    if(lastShowState != showState || countShowState == 0) {     //H2 D4 
      countShowState = 0;
      led.clearBufferDisplay(); 
      led.setChar(0,1,'H',LED_GREEN);
      randomColor = (byte) random(1,3);
      led.setChar(5,1,'a',LED_GREEN);
      randomColor = (byte) random(1,3);
      led.setChar(10,1,'p',LED_GREEN);
      randomColor = (byte) random(1,3);
      led.setChar(14,1,'p',LED_GREEN);
      randomColor = (byte) random(1,3);
      led.setChar(19,1,'y',LED_GREEN);
      led.pushBuffer();
    }
    else if(countShowState == 15){
      led.clearBufferDisplay();
      led.setChar(1,1,'B',LED_RED);
      led.setChar(5,1,'.',LED_ORANGE);
      led.setChar(8,1,'D',LED_GREEN);
      led.setChar(13,1,'a',LED_GREEN);
      led.setChar(18,1,'y',LED_GREEN);
      led.pushBuffer();
    }
    else if(countShowState == 30){
      led.clearBufferDisplay();
      led.setChar(0,1,'b',LED_GREEN);
      led.setChar(5,1,'e',LED_ORANGE);
//      led.setChar(8,1,'\'',LED_GREEN);
      led.setLed(10,0,LED_GREEN);
      led.setLed(9,1,LED_GREEN);
      led.setChar(12,0,'^',LED_ORANGE);
      led.setChar(19,0,'^',LED_ORANGE);
      led.setChar(16,1,'.',LED_ORANGE);
      led.pushBuffer();      
    }
    else if(countShowState >50){
      countShowState = -1;
    }

    countShowState ++;
    break;
  case 2:  //Random Rain
    if(lastShowState != showState ){
      countShowState = 0;
      led.clearBufferDisplay();
    }
    led.setLed((byte)random(0,24),(byte)random(0,8),(byte)random(0,4));
    led.pushBuffer();
    countShowState ++;
    break;
  case 3:  //Sinwave
    if(lastShowState != showState) {
      countShowState =0;
      led.clearBufferDisplay();
    }
    firstNumber = (byte)((sin(countShowState*3.14/7)+1)*7.0/2 )%8;
    //Serial.println(firstNumber);
    led.drawLine(countShowState%24,0,countShowState%24,7,LED_NONE);
    led.drawLine((countShowState+1)%24,0,(countShowState+1)%24,7,LED_NONE);
    led.setLed(countShowState%24, firstNumber,LED_ORANGE);
    led.pushBuffer();
    countShowState++;
    break;
  case 4:  //Blink Enjoy your Life
    if(lastShowState != showState || countShowState<15) {     //H2 D4 
      led.clearBufferDisplay();
      led.setChar(0,0,'E',(byte) random(1,4));
      led.setChar(5,0,'n',(byte) random(1,4));
      led.setChar(10,0,'j',(byte) random(1,4));
      led.setChar(14,0,'o',(byte) random(1,4));
      led.setChar(19,0,'y',(byte) random(1,4));
      led.pushBuffer();
    }
    else if(countShowState<30 ){
      led.clearBufferDisplay();
      led.setChar(0,0,'y',(byte) random(1,4));
      led.setChar(6,0,'o',(byte) random(1,4));
      led.setChar(12,0,'u',(byte) random(1,4));
      led.setChar(18,0,'r',(byte) random(1,4));
      led.pushBuffer();
    }
    else if(countShowState <45){
      led.clearBufferDisplay();
      led.setChar(3,0,'L',(byte) random(1,4));
      led.setChar(8,0,'i',(byte) random(1,4));
      led.setChar(13,0,'f',(byte) random(1,4));
      led.setChar(19,0,'e',(byte) random(1,4));
      led.pushBuffer();
    }
    else {
      countShowState = 0;
    }
    countShowState++;
    break;
  case 5:  //rain
    if(lastShowState != showState ) {     //H2 D4 
      led.clearBufferDisplay();      
      countShowState = 0;
    }
    else{
      tmp = random(0,12);
      led.shift(SHIFT_DOWN);
      for(iRun = 0;iRun < tmp;iRun++){
        led.setLed(random(0,24), 0,random(0,4));
      }
    }
    led.pushBuffer();  
    //    countShowState++;
    break;
  case 6:
    if(lastShowState != showState){
      led.clearBufferDisplay();
      countShowState = 0;            
    }
    
    if(countShowState %10 ==0){
      led.setLed(random(0,24), random(0,8),random(0,4));
    }
    else{
      led.shift(random(6,10));
    }
    led.pushBuffer();
    countShowState++;
    break;
  }
  lastShowState = showState;
}










