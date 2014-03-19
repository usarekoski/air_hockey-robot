// These functions take the direction of motors as parameters and update steppers accordingly, max one step at a time

byte motor1Phase = 1; // 1 to 4
byte motor2Phase = 1;
byte motorByte = 0; // byte that is sent to shift register and controls H-bridges


void stepperDrive(int motor1Dir, int motor2Dir){
  //motor1(LEFT):
  if(motor1Dir != 0){
    motor1Phase += motor1Dir;
    if(motor1Phase == 5)motor1Phase = 1;
    if(motor1Phase == 0)motor1Phase = 4;
    switch (motor1Phase){ //
      case 1: //A+ & B+ 1010****
        motorByte = motorByte << 4; //clear first 4 bits by shifting left
        motorByte = motorByte >> 4; //shift back
        motorByte += 160; //add new bits
        break;
      case 2: //A- & B+ 0110****
        motorByte = motorByte << 4; //clear first 4 bits by shifting left
        motorByte = motorByte >> 4; //shift back
        motorByte += 96; //add new bits
        break;
      case 3: //A- & B- 0101****
        motorByte = motorByte << 4; //clear first 4 bits by shifting left
        motorByte = motorByte >> 4; //shift back
        motorByte += 80; //add new bits
        break;
      case 4: //A+ & B- 1001****
        motorByte = motorByte << 4; //clear first 4 bits by shifting left
        motorByte = motorByte >> 4; //shift back
        motorByte += 144; //add new bits
        break;
    }
  }
  //motor2(RIGHT):
  if(motor2Dir != 0){
    motor2Phase += motor2Dir;
    if(motor2Phase == 5)motor2Phase = 1;
    if(motor2Phase == 0)motor2Phase = 4;
    switch (motor2Phase){
      case 1: //A+ & B+ ****1010
        motorByte = motorByte >> 4; //clear last 4 bits by shifting right
        motorByte = motorByte <<  4; //shift back
        motorByte += 10; //add new bits
        break;
      case 2: //A- & B+ ****0110
        motorByte = motorByte >> 4; //clear last 4 bits by shifting right
        motorByte = motorByte << 4; //shift back
        motorByte += 6; //add new bits
        break;
      case 3: //A- & B- ****0101
        motorByte = motorByte >> 4; //clear last 4 bits by shifting right
        motorByte = motorByte << 4; //shift back
        motorByte += 5; //add new bits
        break;
      case 4: //A+ & B- ****1001
        motorByte = motorByte >> 4; //clear last 4 bits by shifting right
        motorByte = motorByte << 4; //shift back
        motorByte += 9; //add new bits
        break;
    }
  }
  if(motor1Dir != 0 || motor2Dir !=0)sendByte(motorByte); //update shift register if needed
}

void sendByte(byte mByte){ //send updated state to stepper drivers
  register int x = 1;
  fastGpioDigitalWrite(LATCHPIN, x);
 x =!x;
  SPI.transfer(mByte);
  fastGpioDigitalWrite(LATCHPIN, x);
}