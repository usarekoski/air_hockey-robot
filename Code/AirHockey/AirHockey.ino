#include <SPI.h>
#include "DEFINES.h"


unsigned long Time; // keep track of time (note: "time" is used by RTC)

void loop() {
  Time = millis();
  
  stepperTest();
  
}
