// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myServoX;  // create servo object to control a servo 
Servo myServoY;

#define MIN_DEGREES_X 1500
#define MAX_DEGREES_X 2600
#define MIN_DEGREES_Y 1100
#define MAX_DEGREES_Y 1900
#define MIDDLE_POS_X (MAX_DEGREES_X + MIN_DEGREES_X)/2;
#define MIDDLE_POS_Y (MAX_DEGREES_Y + MIN_DEGREES_Y)/2;


int joystickValueX;
int joystickValueY;
int posX;
int posY;

int valuesFromSerialPort[4];
int counter = 0;
boolean debug = false;

double delayTime = 0;

String inString = "";    // string to hold input
 
void setup() 
{
  Serial.begin(9600);

  myServoX.attach(9);
  posX = MIDDLE_POS_X;
  myServoX.writeMicroseconds(posX);
  
  myServoY.attach(10);
  posY = MIDDLE_POS_Y;
  myServoY.writeMicroseconds(posY);

  delay(5000);
} 


//void loop() 
//{ 
//  joystickValueX = analogRead(0);            // reads the value of the joystick (value between 0 and 1023) 
//  int centerAdjust = 20;
//  if (joystickValueX < (512 - centerAdjust)) {
//    posX -= 1;
//    delayTime = (joystickValueX + 1)/5;
//  } else if (joystickValueX > (512 + centerAdjust)) {
//    posX += 1;
//    delayTime = (abs(joystickValueX - 1024) + 1)/5;
//  } else {
//    posX += 0;
//    delayTime = 10;
//  }
//  posX = validatePosition(posX);
//  myServoX.write(posX);
//  Serial.println(delayTime);
//  delay(floor(delayTime));                           // waits for the servo to get there 
//} 
//
//int validatePosition(int posX) {
//  if (posX < MIN_DEGREES_X) {
//    return MIN_DEGREES_X;
//  } else if (posX > MAX_DEGREES_X) {
//    return MAX_DEGREES_X;
//  } else {
//    return posX;
//  }
//
//}



// Alternativ losningsmaate med to dimensjoner:
//void loop() 
//{ 
//  if(debug) {
//      Serial.println(posX);
//  }
//  
//  if(!myServoX.attached()) {
//    myServoX.attach(9);
//  }
//  
//  if (Serial.available() > 0) {
//     int c = Serial.read() - '0';
//     Serial.println("c: " + c);
//     String miNambah = "";
//     while(c != -35 || c != -38) {
//       miNambah += c;
//       //joystickValueX = miNambah.toInt();
//       Serial.println(miNambah);
//       int c = Serial.read() - '0';
//     }
//
//
//  }
//  setPosX(joystickValueX);
//  myServoX.writeMicroseconds(posX);
//  //joystickValueY = analogRead(1);
//  //setPosY(joystickValueY);
//  //myServoY.write(posY);
//} 

void loop() 
{
  
  if(debug) {
      Serial.println(posX);
  }
  
  if(!myServoX.attached()) {
    myServoX.attach(9);
  }
  if(!myServoY.attached()) {
    myServoY.attach(10);
  }
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char 
      // and add it to the string:
      inString += (char)inChar; 
    }
    if (inChar == ' ') {
      valuesFromSerialPort[counter] = inString.toInt();
      inString = "";
      counter += 1;
    } 
    if (inChar == '\n') {
      valuesFromSerialPort[counter] = inString.toInt();
      
      // x-axis
      setPosX(valuesFromSerialPort[0]);
      myServoX.writeMicroseconds(posX);

      // y-axis
      setPosY(valuesFromSerialPort[1]);
      myServoY.writeMicroseconds(posY);
      
      // clear the string for new input:
      inString = ""; 
      counter = 0;
      
    }
  }
  delay(21);
     // Serial.print("Value:");

}

void setPosX(int joystickValueX) {
  int deltaX = getDelta(joystickValueX);       
  //Serial.println(deltaX);

  if ((posX + deltaX) < MIN_DEGREES_X) {
     if(debug) {
       Serial.println("Boundary +");
     }
     posX = MIN_DEGREES_X;
  } else if ((posX + deltaX) > MAX_DEGREES_X) {
    if(debug) {
      Serial.println("Boundary -");
    }
    posX = MAX_DEGREES_X;
  } else {
    if(debug) {
      Serial.println("Inside bounds");
    }
    posX = posX + deltaX;
  }
}

//int setPosX(int joystickValueX) {
//  int deltaX = getDelta(joystickValueX);
//  if ((posX + deltaX) < MIN_DEGREES_X) {
//     if(debug) {
//       Serial.println("Boundary +");
//     }
//     return MIN_DEGREES_X;
//  } else if ((posX + deltaX) > MAX_DEGREES_X) {
//    if(debug) {
//      Serial.println("Boundary -");
//    }
//    return MAX_DEGREES_X;
//  } else {
//    if(debug) {
//      Serial.println("Inside bounds");
//    }
//    return posX + deltaX;
//  }
//}

void setPosY(int joystickValueY) {
  int deltaY = getDelta(joystickValueY);
  if ((posY + deltaY) < MIN_DEGREES_Y) {
     posY = MIN_DEGREES_Y;
  } else if ((posY + deltaY) > MAX_DEGREES_Y) {
    posY = MAX_DEGREES_Y;
  } else {
    posY = posY + deltaY;
  }
}
  
int getDelta(int joystickValue) {
  int centerAdjust = 20;
  if (joystickValue < (512 - centerAdjust)) {
    if(debug) {
      Serial.println("Going down");
    }
    return floor((512 - joystickValue)/20);
  } else if (joystickValue > (512 + centerAdjust)) {
    if(debug) {
      Serial.println("Going up");
    }
    return floor((joystickValue - 512)/20)*(-1);
  } 
  if(debug) {
      Serial.println("Stick at rest");
  }
  myServoX.detach();
  return 0;
}

