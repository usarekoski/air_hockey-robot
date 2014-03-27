// Main code

#include <SPI.h>
#include "DEFINES.h"

unsigned long Time; // keep track of time (note: "time" is used by RTC)
unsigned long moveTime = 0;

void loop() {
  
  if((micros() - moveTime) > MOVEUPDATE){
    moveTime = micros();
    moveUpdate();
  }
  //stepperTest();
}
