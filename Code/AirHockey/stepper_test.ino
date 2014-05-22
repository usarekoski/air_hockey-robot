// test steppers

byte dir = 0;//motor test
unsigned long stepperTime = 0;
int s = 0;

void stepperTest(){
  if((Time - stepperTime) > 3 && dir == 0){
    stepperTime = Time;
    s++;
    stepperDrive(1,0);
    //if(s == 100){
      
      //delay(150);
      //dir = 1;
      //s=0;
    //}
  }
  if((Time - stepperTime) > 5 && dir == 1){
    stepperTime = Time;
    s++;
    stepperDrive(-1,1);
    if(s == 100){
      
      delay(150);
      dir = 0;
      s=0;
    }
  }
  /*
  for(int x = 0; x < 920; x++){
    stepperDrive(1,-1);
    //delayMicroseconds(1);
  }
  delay(1000);
  for(int x = 0; x < 920; x++){
    stepperDrive(-1,1);
    //delayMicroseconds(1);
  }
  delay(1000);*/
}
