#include <SPI.h>
#include <CFC_LedMatrixSerial.h>
#include <Time.h>
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "SystemFont5x7.h"

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

CFC_LedMatrixSerial led(3);
long lastLongPeriod;
long lastShortPeriod;
byte runMode;
byte buttonPin = 8;
unsigned int countState;

void setup() {
  pinMode(buttonPin,INPUT_PULLUP);
  led.selectFont(System5x7);
  led.clearDisplay();
  led.clearBufferDisplay();
  Serial.begin(9600);
  
  if(digitalRead(buttonPin) == LOW){
    setSyncProvider( requestSync);  //set function to call when sync required
    delay(10000);
    processSyncMessage();
    //setTime(10,56,0,15, 12, 2014);
    //RTC.set(now());
  }
  else {
    setSyncProvider(RTC.get);   // the function to get the time from the RTC    
  }
  if(timeStatus()!= timeSet) 
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time"); 
  randomSeed(analogRead(A2));  //get random from random
}

void loop() {
  long currentTime = millis();
  if(currentTime - lastLongPeriod > 100){
    lastLongPeriod = currentTime;
    button_process();
    //command_process();
    run();
  }
  if(currentTime - lastShortPeriod > 2){
    lastShortPeriod = currentTime;
    led.spiScanLed();  
  }
}

void processSyncMessage() {
 //if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
      RTC.set(now());
    }  
  }
}

time_t requestSync()
{
  Serial.print(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}











