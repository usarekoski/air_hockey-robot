float xPos = 10; // current coordinates before planned move, these are updated after target is reached
float yPos = 30;

float thetaL;
float thetaR;
float oldThetaL;
float oldThetaR;

float targetX;
float targetY;

byte moveFlag = 0;

void moveUpdate(){ // this should be run every 1-2ms during moves
 if(xPos != targetX || yPos != targetY){
  if(moveFlag==0){
    kinematics(xPos,yPos); // calculate path
    moveFlag = 1;
  }
  if(getXY()){ //get new coordinates abd update x and y variables
    if(targetX>x) x += xPos;
    if(targetY>y) y += yPos;
    if(targetX<x) x = xPos - x;
    if(targetY<y) y = yPos - y;
    thetaL = xyToAngle(x,y,0); //next step coordinates to angles 0=L, 1=R
    thetaR = xyToAngle(x,y,1);
    stepperDrive(((thetaR-oldThetaR)/DPS),((thetaL-oldThetaL)/DPS));
    oldThetaL = thetaL;
    oldThetaR = thetaR;
  }else{ // getXY returns 0 -> target pos reached
    moveFlag = 0; // move done, rady for a new one
    xPos = targetX;
    yPos = targetY;
  }
 }
}
