float thetaL;
float thetaR;
float oldThetaL = 65;
float oldThetaR = 65;

float targetX;
float targetY;

byte moveFlag = 0;
byte moveXY = 1;
extern float x;
extern float y;

void moveUpdate(){ // this should be run every 1-2ms during moves
 if(xPos != targetX || yPos != targetY){
  if(moveFlag==0){
    kinematics(xPos,yPos); // calculate path
    moveFlag = 1;
    moveXY = 1;
    Serial.println(targetX);
    Serial.println(targetY);
  }
  if(moveXY==1){ //get new coordinates and update x and y variables
    getXY();
    if(targetX>=xPos) x += xPos;
    if(targetY>=yPos) y += yPos;
    if(targetX<xPos) x = xPos - x;
    if(targetY<yPos) y = yPos - y;
    thetaL = xyToAngle(x,y,0); //next step coordinates to angles 0=L, 1=R
    thetaR = xyToAngle(x,y,1);
    //Serial.print(thetaL);
    //Serial.print(" ");
    //Serial.println(thetaR);
    if((thetaR-oldThetaR)/DPS>=1){
      //Serial.print("MOVE");
      oldThetaR = thetaR;
      stepperDrive(1,0);
    }
    else if((thetaR-oldThetaR)/DPS<=-1){
      //Serial.print("MOVE");
      oldThetaR = thetaR;
      stepperDrive(-1,0);
    }
    if((thetaL-oldThetaL)/DPS>=1){
      oldThetaL = thetaL;
      stepperDrive(0,1);
    }
    else if((thetaL-oldThetaL)/DPS<=-1){
      oldThetaL = thetaL;
      stepperDrive(0,-1);
    }
  }else{ // getXY returns 0 -> target pos reached
    moveFlag = 0; // move done, rady for a new one
    xPos = targetX;
    yPos = targetY;
  }
 }
}
