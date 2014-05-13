// Main code
#include <math.h>
#include <SPI.h>
#include "DEFINES.h"
byte usd = 1;

unsigned long Time; // keep track of time (note: "time" is used by RTC)
unsigned long moveTime = 0;

unsigned long targTime = 0;
int asd = 0;

extern float targetX;
extern float targetY;

void loop() {
  Time = millis();
  
  if((millis() - targTime) > 2000 || usd ==1){
    usd = 0;
    targTime = millis();
    asd++;
    if(asd == 5) asd = 1;
    switch(asd){
      case 1:
        targetX = 0;
        targetY = 0;
        break;
      case 2:
        targetX = 0;
        targetY = 140;
        break;
      case 3:
        targetX = 225;
        targetY = 140;
        break;
      case 4:
        targetX = 225;
        targetY = 0;
        break;
    }
    targetConst();   
    //Serial.print(targetX);
    //Serial.println("m");
  }
  if((micros() - moveTime) > MOVEUPDATE){
    moveTime = micros();
    moveUpdate();
  }
    
  
 //stepperTest();
  
}

void targetConst(){
 targetX = constrain(targetX, MIN_XPOS, MAX_XPOS);
 targetY = constrain(targetY, MIN_YPOS, MAX_YPOS);
}
