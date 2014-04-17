  
unsigned long t0; //point in time when move started
float dX; //length of X change
float dY; //length of Y change
float sDist; //lenght of move
float kXY; //direction

float t1; //acceleration duration
float t2; //duration of constant speed
float t3; //decceleration duration

float s1; //lenghts of previous
float s2;
float s3;

float x;
float y;

double dt;
float sX;

extern float targetX;
extern float targetY;

extern byte moveXY;
  
void kinematics(float xn, float yn){
  Serial.println("Calculating Kinematics");
  t0 = millis();
  dX = targetX - xn;
  dY = targetY - yn;
  if(dX!=0)kXY = dY/dX;
  else kXY=999999999;
  Serial.print("kXY: ");
  Serial.print(kXY);
  
  sDist = sqrt(dX*dX+dY*dY); //mm
  Serial.print(" dist: ");
  Serial.println(sDist);
  
  //three parts: 1|accel, 2|const.speed and 3|deccel:
  t1 = V_MAX/A_MAX; //meters
  t3 = t1;
  s1 = 0.5*A_MAX*t1*t1;
  s3 = s1;
  //Serial.println(s3);
  s2 = (sDist/1000)-(s1+s3);
  t2 = s2/V_MAX;
  
  if(s2 <= 0){ // maximum speed is not reached during the move
    s1 = sDist/2000; //mm to meter / 2
    s2 = 0;
    s3 = s1;
    t1 = sqrt(2*s1/A_MAX);
    t2 = 0;
    t3 = t1;
  }
  Serial.print("t1: ");
  Serial.print(t1);
  Serial.print(" t2: ");
  Serial.print(t2);
  Serial.print(" t3: ");
  Serial.println(t3);
  Serial.print("s1: ");
  Serial.print(s1);
  Serial.print(" s2: ");
  Serial.print(s2);
  Serial.print(" s3: ");
  Serial.println(s3);
}

void getXY(){ //return true when still moving, false when done
  dt = double((millis()-t0))/1000.0; // time passed after the start in seconds
  
  if (dt <= t1){ //1
    sX = 0.5*A_MAX*dt*dt; // distance from start
  }
  else if(t1 < dt && dt < (t1+t2)){ //2
    sX = s1 + V_MAX*(dt-t1); // distance from start
  }
  else if(dt >= (t1+t2)){ //3
    sX = s1 + s2 + s3 - 0.5*A_MAX*(dt-(t1+t2+t3))*(dt-(t1+t2+t3)); // distance from start
  }
  if(kXY!=0){
  x = (sX/sqrt(pow(kXY,2)+1))*1000; // calculate x and y deimensions that are added to starting coordinates in mm
  y = (sX/sqrt(pow(kXY,-2)+1))*1000;
  }else{
    x = sX*1000.0;
    y = 0;
  }

  if(dt >= (t1+t2+t3)){ //ready
    moveXY = 0;
  }
}
