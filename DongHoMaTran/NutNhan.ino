
#define NO_PRESS  0
#define SHORT_PRESS  1
#define LONG_PRESS  2

byte buttonState;
byte pressCount;

void button_process(){
  if(digitalRead(buttonPin) == LOW){
    buttonState = SHORT_PRESS;
    pressCount++;
    if(pressCount>10){
      buttonState = LONG_PRESS;
    }
  }
  else {
    if(buttonState == SHORT_PRESS){
      buttonState = NO_PRESS;
      //wake up and show time
      if(runMode == 0)
        showState = (showState+1)%4;
      else if(runMode ==1)
        showState = (showState+1)%7;
    }
    else if(buttonState == LONG_PRESS){
      buttonState = NO_PRESS;
      //change mode for display
      runMode = (runMode+1)%2;
    }
    pressCount = 0;
  }
}

