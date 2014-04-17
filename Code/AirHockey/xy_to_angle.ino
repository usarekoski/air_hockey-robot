// This function converts the desired cartesian coordinates to angles of the steppers

float theta;
float dist; //distance from origo

float xyToAngle(float x, float y, boolean motor){ // XY-coordinates in mm, motor 0 = left, 1 = right. Returns degrees
  if(motor) x = MOTORSPAN - x; // true -> right motor, invert x
  theta = radToDeg(atan2(y,x)); // angle from origo to XY
  dist = sqrt(x*x+y*y);
  theta += radToDeg(acos(dist/(2*ARMLENGHT))); // add angle from latter to first arm
  return (theta);
}

float radToDeg(float rad){ // converts redians to degrees
  return rad*180/PII;
}
