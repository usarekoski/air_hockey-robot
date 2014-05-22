// All setup is done here to clear the main function
uint32_t latchValue;

extern float xPos = MOTORSPAN/2; // current coordinates before planned move, these are updated after target is reached
extern float yPos = 120;
extern float targetX;
extern float targetY;
extern int reset_coordinates = 0;

void reset_pusher() {
  xPos = 0;
  yPos = 0;
  while (1) {
    serial_update();
    if (reset_coordinates == 1) {
      xPos = targetX;
      yPos = targetY;
      break;
    }
    moveUpdate();
  }
}

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
  //reset_pusher();
}
