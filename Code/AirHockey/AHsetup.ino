// All setup is done here to clear the main function

void setup(){
  //Stepper driver:
  pinMode(LATCHPIN, OUTPUT_FAST); //latchpin set to fast mode
  SPI.begin(); //initialize SPI
  pinMode(STEPPERENABLEPIN, OUTPUT); //Enable motors
  digitalWrite(STEPPERENABLEPIN, HIGH); //motor enable 
  
  
}
