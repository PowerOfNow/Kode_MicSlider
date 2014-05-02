#include <MicSliderComm.h>

MicSliderCommSender sender;

void setup() {
  Serial.begin(9600);
}

void loop() {
  static controlData controllerOne;
  static controlData controllerTwo;
  
  sender.setControllerOneData(analogRead(A0), analogRead(A1));
  sender.setControllerTwoData(0, 0); // TODO: update with correct data when available

  sender.sendControlData();
  delay(10);        // delay in between reads for stability
}





