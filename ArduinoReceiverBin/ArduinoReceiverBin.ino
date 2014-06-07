#include <MicSliderComm.h>
#include <Servo.h> 
 
// Whether to enable debug output
boolean debug = false;

//  Objects
Servo servoOneX;
Servo servoOneY;
Servo servoTwoX;
Servo servoTwoY;

MicSliderCommReceiver receiver;

// Pin setup
const int pinServoOneX = 9;
const int pinServoOneY = 10;
const int pinServoTwoX = 5;
const int pinServoTwoY = 6;

// Calculate const values
const int middleOneXVal = (C_ONE_MAX_X_POS - C_ONE_MIN_X_POS) / 2;
const int middleOneYVal = (C_ONE_MAX_Y_POS - C_ONE_MIN_Y_POS) / 2;
const int middleTwoXVal = (C_TWO_MAX_X_POS - C_TWO_MIN_X_POS) / 2;
const int middleTwoYVal = (C_TWO_MAX_Y_POS - C_TWO_MIN_Y_POS) / 2;


void debugPrint (int oneX, int oneY, int twoX, int twoY)
{
  if (debug) {
    Serial.print( "1X:"); Serial.print(oneX); Serial.print(" 1Y:"); Serial.print(oneY);
    Serial.print(" 2X:"); Serial.print(twoX); Serial.print(" 2Y:"); Serial.print(twoY);
    Serial.println();
  }
}

void setup()
{
  Serial.begin(9600);

  servoOneX.attach(pinServoOneX);
  servoOneX.writeMicroseconds(middleOneXVal);

  servoOneY.attach(pinServoOneY);
  servoOneY.writeMicroseconds(middleOneYVal);

  servoTwoX.attach(pinServoTwoX);
  servoTwoX.writeMicroseconds(middleTwoXVal);
  
  servoTwoY.attach(pinServoTwoY);
  servoTwoY.writeMicroseconds(middleTwoYVal);

  delay(5000);
}

void loop()
{
  static int ctrlOneX = middleOneXVal;
  static int ctrlOneY = middleOneYVal;
  static int ctrlTwoX = middleTwoXVal;
  static int ctrlTwoY = middleTwoYVal;

  receiver.receiveControlData(); // Will lock until necessary data has been read

  // Debug
  debugPrint(ctrlOneX, ctrlOneY, ctrlTwoX, ctrlTwoY);
  
  // Get controller data received by receiveControlData() above
  receiver.getControllerOneData(&ctrlOneX, &ctrlOneY);
  receiver.getControllerTwoData(&ctrlTwoX, &ctrlTwoY);
  
  // Set position of servos controlled by controller one
  servoOneX.writeMicroseconds(ctrlOneX);
  servoOneY.writeMicroseconds(ctrlOneY);
  
  // Set position of servos controlled by controller two
  servoTwoX.writeMicroseconds(ctrlTwoX);
  servoTwoY.writeMicroseconds(ctrlTwoY);
}
