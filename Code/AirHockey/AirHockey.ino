// Main code
#include <math.h>
#include <SPI.h>
#include "DEFINES.h"
byte usd = 1;

unsigned long Time; // keep track of time (note: "time" is used by RTC)
unsigned long moveTime = 0;

unsigned long targTime = 0;
int asd = 0;
float incoming_coordinate;

extern float targetX;
extern float targetY;


void targetConst(){
 targetX = constrain(targetX, MIN_XPOS, MAX_XPOS);
 targetY = constrain(targetY, MIN_YPOS, MAX_YPOS);
}

void serial_update(){
  if (Serial1.available() >= 2) {
      incoming_coordinate = Serial1.parseFloat();
      Serial.println(incoming_coordinate);
      targetX = incoming_coordinate;
      targetY = 100;
      
  }
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

