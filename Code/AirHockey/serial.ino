extern float targetX;
extern float targetY;

extern int reset_coordinates;
String coordinates;
int loc_x;
int loc_y;
char c;


float string_to_float(String str){
  char carray[str.length() + 1]; //determine size of the array
  str.toCharArray(carray, sizeof(carray)); //put str into an array
  return atof(carray);
}

void serial_update(){
  while (Serial1.available()) {
    c = Serial1.read();  //gets one byte from serial buffer
    if (c == ',') {
      loc_x = coordinates.indexOf("x");
      targetX = string_to_float(coordinates.substring(0, loc_x));
      
      loc_y = coordinates.indexOf("y");
      targetY = string_to_float(coordinates.substring(loc_x+1, loc_y));
      
      reset_coordinates = string_to_float(coordinates.substring(loc_y+1));
      Serial.println(coordinates);
      
      coordinates = ""; //clears variable for new input
      break;
    }
      
     else {
       coordinates += c; //makes the string coordinates
     }
    }
}
