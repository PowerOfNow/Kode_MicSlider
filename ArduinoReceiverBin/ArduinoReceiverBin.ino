#include <MicSliderComm.h>
#include <Servo.h> 
 
Servo myServoX;
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

int ctrlOneX, ctrlOneY, ctrlTwoX, ctrlTwoY;

boolean debug = false;

MicSliderCommReceiver receiver;

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


void printBuffer(unsigned char* buffer, int bufSize) // Debug function
{
  for (int i = 0; i < bufSize; ++i) {
    Serial.print(buffer[i]);
    Serial.print("|");
  }
  Serial.println("");
}

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
  
  receiver.receiveControlData(); // Will lock here until necessary data has been read
  receiver.getControllerOneData(&ctrlOneX, &ctrlOneY);
  // receiver.getControllerTwoData(&ctrlTwoX, &ctrlTwoY); // TODO: uncomment when implemented
  
  // Controller one
  setPosX(ctrlOneX);
  myServoX.writeMicroseconds(posX);
  setPosY(ctrlOneY);
  myServoY.writeMicroseconds(posY);
  
  // TODO: Controller two
        
  delay(21); // TODO: Is delay necessary here?
}


void setPosX(int joystickValueX) {
  int deltaX = getDelta(joystickValueX);       

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

