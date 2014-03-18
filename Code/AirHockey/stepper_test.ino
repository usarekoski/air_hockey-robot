// test steppers

byte dir = 0;//motor test
unsigned long stepperTime = 0;
byte s = 0;

void stepperTest(){
  if((Time - stepperTime) > 10 && dir == 0){
    stepperTime = Time;
    s++;
    stepperDrive(1,1);
    if(s == 51){
      delay(100);
      dir = 1;
      s=0;
    }
  }
  if((Time - stepperTime) > 10 && dir == 1){
    stepperTime = Time;
    s++;
    stepperDrive(-1,-1);
    if(s == 51){
      delay(100);
      dir = 0;
      s=0;
    }
  }
  /*for(byte x = 0; x < 201; x++){
    stepperDrive(0,-1);
    delay(5);
  }
  delay(1000);
  for(byte x = 0; x < 201; x++){
    stepperDrive(0,1);
    delay(5);
  }
  delay(1000);*/
}
