// All setup is done here to clear the main function
uint32_t latchValue;

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println("Setup");
  //Stepper driver:
  //pinMode(LATCHPIN, OUTPUT_FAST); //latchpin set to fast mode
  latchValue = fastGpioDigitalLatch();
  pinMode(3, OUTPUT_FAST);
  SPI.begin(); //initialize SPI
  pinMode(STEPPERENABLEPIN, OUTPUT); //Enable motors
  digitalWrite(STEPPERENABLEPIN, HIGH); //motor enable 
  sendByte(170);
  delay(7000);
}
