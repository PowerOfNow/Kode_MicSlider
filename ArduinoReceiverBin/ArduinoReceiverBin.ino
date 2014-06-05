#include <MicSliderComm.h>
#include <Servo.h> 
 
Servo servoOneX;
Servo servoOneY;
Servo servoTwoX;
Servo servoTwoY;

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

int ctrlOneX = 1500;
int ctrlOneY = 1500;
int ctrlTwoX = 1500;
int ctrlTwoY = 1500;

boolean debug = false;

MicSliderCommReceiver receiver;

void printBuffer(unsigned char* buffer, int bufSize) // Debug function
{
  for (int i = 0; i < bufSize; ++i) {
    Serial.print(buffer[i]);
    Serial.print("|");
  }
  Serial.println("");
}

void setup()
{
  Serial.begin(9600);

  servoOneX.attach(9);
  posX = MIDDLE_POS_X;
  servoOneX.writeMicroseconds(1500);

  servoOneY.attach(10);
  posY = MIDDLE_POS_Y;
  servoOneY.writeMicroseconds(1500);

  servoTwoX.attach(5);
  posX = MIDDLE_POS_X;
  servoTwoX.writeMicroseconds(1500);
  
  servoTwoY.attach(6);
  posY = MIDDLE_POS_Y;
  servoTwoY.writeMicroseconds(1500);

  delay(5000);
}
int counter = 0;
void loop()
{
  //if(debug) {
  //  Serial.println(posX);
  //}
  /*
  if(!servoOneX.attached()) {
    servoOneX.attach(9);
  }
  if(!servoOneY.attached()) {
    servoOneY.attach(10);
  }
  */
  receiver.receiveControlData(); // Will lock here until necessary data has been read
  receiver.getControllerOneData(&ctrlOneX, &ctrlOneY);
  receiver.getControllerTwoData(&ctrlTwoX, &ctrlTwoY); // TODO: uncomment when implemented
  
  // Controller one
  //setPosX(ctrlOneX);
  servoOneX.writeMicroseconds(ctrlOneX);
  //setPosY(ctrlOneY);
  servoOneY.writeMicroseconds(ctrlOneY);
  // Controller two
  servoTwoX.writeMicroseconds(ctrlTwoX);
  servoTwoY.writeMicroseconds(ctrlTwoY);
  
  // Debug
  Serial.print("1X:"); Serial.print(ctrlOneX);  Serial.print(" 1Y:"); Serial.print(ctrlOneY);
  Serial.print(" 2X:"); Serial.print(ctrlTwoX);  Serial.print(" 2Y:"); Serial.print(ctrlTwoY);
  Serial.println();
        
  //delay(20); // Is delay necessary here?
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
  servoOneX.detach();
  return 0;
}

