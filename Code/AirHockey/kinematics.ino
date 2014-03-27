  
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

float dt;
float sX;

extern float targetX;
extern float targetY;
  
void kinematics(float xn, float yn){
  t0 = micros();
  dX = targetX - xn;
  dY = targetY - yn;
  kXY = dX/dY;
  
  sDist = (sqrt(dX*dX+dY*dY))/1000;
  //three parts: 1|accel, 2|const.speed and 3|deccel:
  t1 = V_MAX/A_MAX;
  t3 = t1;
  s1 = 0.5*A_MAX*t1*t1;
  s3 = s1;
  s2 = sDist-(s1+s3);
  t2 = s2/V_MAX;
  
  if(s2 <= 0){ // maximum speed is not reached during the move
    s1 = sDist/2;
    s2 = 0;
    s3 = s1;
    t1 = sqrt(2*s1/A_MAX);
    t2 = 0;
    t3 = t1;
  }
}

boolean getXY(){ //return true when still moving, false when done
  dt = (micros()-t0)/1000000; // time passed after the start
  
  if (dt <= t1){ //1
    sX = 0.5*A_MAX*dt*dt; // distance from start
  }
  else if(t1 < dt && dt < (t1+t2)){ //2
    sX = s1 + V_MAX*(dt-t1); // distance from start
  }
  else if(dt >= (t1+t2)){ //3
    sX = s1 + s2 + s3 - 0.5*A_MAX*(dt-(t1+t2))*(dt-(t1+t2)); // distance from start
  }
  x = sX*kXY; // calculate x and y deimensions that are added to starting coordinates
  y = sX/kXY;
    if(dt >= (t1+t2+t3))return 0;
  return 1;
}
