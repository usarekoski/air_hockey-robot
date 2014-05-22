// Main code
#include <math.h>
#include <SPI.h>
#include "DEFINES.h"
byte usd = 1;

unsigned long Time; // keep track of time (note: "time" is used by RTC)
unsigned long moveTime = 0;

unsigned long targTime = 0;

extern float targetX;
extern float targetY;

void targetConst(){
 targetX = constrain(targetX, MIN_XPOS, MAX_XPOS);
 targetY = constrain(targetY, MIN_YPOS, MAX_YPOS);
}

void loop() {
  Time = millis();
  if((micros() - moveTime) > MOVEUPDATE){
    serial_update();
    targetConst();
    moveUpdate();
    moveTime = micros();
  }
    
 //stepperTest();
  
}

