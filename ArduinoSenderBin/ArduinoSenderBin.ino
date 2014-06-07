#include <LiquidCrystal.h>

#include <MicSliderComm.h>

// Objects
MicSliderCommSender sender;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin setup
const int joystickOneXPin     = A0;
const int joystickOneYPin     = A1;
const int joystickTwoXPin     = A2;
const int joystickTwoYPin     = A3;
const int lcdBacklightPin     = 7;
const int presetOneButtonPin  = 8;
const int presetOneLedPin     = 13;

// LCD column positions
const int lcdLeftRightPos = 0;
const int lcdFrontBackPos = 4;
const int lcdPanPos       = 8;
const int lcdTiltPos      = 12;

// Delta values for both controllers (how much should x/y be de/increased by per iteration while joystick is active)
const int C_ONE_X_DELTA = 5;
const int C_ONE_Y_DELTA = 5;
const int C_TWO_X_DELTA = 15;
const int C_TWO_Y_DELTA = 15;

// Joystick constants
const int joystickMiddle    = 512;
const int joystickDeadzoneX = 162;
const int joystickDeadzoneY = 162;

// PRESET values
int presetPosOneX = 1500;
int presetPosOneY = 1500;
int presetPosTwoX = 1500;
int presetPosTwoY = 1500;


// These four variables contain the current position of all for servos.
// Only modify these through updatePos*()
int posOneX = 1500;
int posOneY = 1500;
int posTwoX = 1500;
int posTwoY = 1500;
void updatePosOneX(int delta)
{
  posOneX = constrain(posOneX + delta, C_ONE_MIN_X_POS, C_ONE_MAX_X_POS);
}

void updatePosOneY(int delta)
{
  posOneY = constrain(posOneY + delta, C_ONE_MIN_Y_POS, C_ONE_MAX_Y_POS);
}

void updatePosTwoX(int delta)
{
  posTwoX = constrain(posTwoX + delta, C_TWO_MIN_X_POS, C_TWO_MAX_X_POS);
}

void updatePosTwoY(int delta)
{
  posTwoY = constrain(posTwoY + delta, C_TWO_MIN_Y_POS, C_TWO_MAX_Y_POS);
}


// JOYSTICK FUNCTIONS
int joystickGetXModifier(int joypos)
{
  static const int deadzoneXDown = joystickMiddle - joystickDeadzoneX;
  static const int deadzoneXUp   = joystickMiddle + joystickDeadzoneX;

  if (joypos < deadzoneXDown) {
    return -1;
  } else if (joypos > deadzoneXUp) {
    return 1;
  }
  return 0;
}

int joystickGetYModifier(int joypos)
{
  static const int deadzoneYDown = joystickMiddle - joystickDeadzoneY;
  static const int deadzoneYUp   = joystickMiddle + joystickDeadzoneY;

  if (joypos < deadzoneYDown) {
    return -1;
  } else if (joypos > deadzoneYUp) {
    return 1;
  }
  return 0;
}

int joystickCalculateOneXDelta(int joypos)
{
  return C_ONE_X_DELTA * joystickGetXModifier(joypos);
}

int joystickCalculateOneYDelta(int joypos)
{
  return C_ONE_Y_DELTA * joystickGetYModifier(joypos);
}

int joystickCalculateTwoXDelta(int joypos)
{
  return C_TWO_X_DELTA * joystickGetXModifier(joypos);
}

int joystickCalculateTwoYDelta(int joypos)
{
  return C_TWO_Y_DELTA * joystickGetYModifier(joypos);
}

void updatePositionsFromJoysticks()
{
  int curJoyOneX = analogRead(joystickOneXPin);
  int curJoyOneY = analogRead(joystickOneYPin);
  int curJoyTwoX = analogRead(joystickTwoXPin);
  int curJoyTwoY = analogRead(joystickTwoYPin);

  int deltaOneX = joystickCalculateOneXDelta(curJoyOneX);
  int deltaOneY = joystickCalculateOneYDelta(curJoyOneY);
  int deltaTwoX = joystickCalculateTwoXDelta(curJoyTwoX);
  int deltaTwoY = joystickCalculateTwoYDelta(curJoyTwoY);

  updatePosOneX(deltaOneX);
  updatePosOneY(deltaOneY);
  updatePosTwoX(deltaTwoX);
  updatePosTwoY(deltaTwoY);
}


// LCD FUNCTIONS
void lcdSetHeaders(String headerOne, String headerTwo, String headerThree, String headerFour)
{
  lcd.setCursor(lcdLeftRightPos, 0);
  lcd.print(headerOne);

  lcd.setCursor(lcdFrontBackPos,0);
  lcd.print(headerTwo);

  lcd.setCursor(lcdPanPos, 0);
  lcd.print(headerThree);

  lcd.setCursor(lcdTiltPos, 0);
  lcd.print(headerFour);
}

void lcdInit() {
  static bool init = false;
  if (!init) {
    lcdSetHeaders("LR", "FB", "Pan", "Tilt");
    init = true;
  }
}

String lcdPercentString(int inputValue)
{
  String value = String (inputValue);
  value += "%";
  for (int i = 0; i < 4 - value.length(); i++) {
    value += " ";
  }
  return value;
}

void lcdUpdate()
{
  lcd.setCursor(lcdLeftRightPos, 1);
  lcd.print(lcdPercentString(map(posOneX, C_ONE_MIN_X_POS, C_ONE_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdFrontBackPos, 1);
  lcd.print(lcdPercentString(map(posOneY, C_ONE_MIN_Y_POS, C_ONE_MAX_Y_POS, 0, 100)));

  lcd.setCursor(lcdPanPos, 1);
  lcd.print(lcdPercentString(map(posTwoX, C_TWO_MIN_X_POS, C_TWO_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdTiltPos, 1);
  lcd.print(lcdPercentString(map(posTwoY, C_TWO_MIN_Y_POS, C_TWO_MAX_Y_POS, 0, 100)));
}


// PRESET FUNCTIONS
const int buttonStatePressed = 0;
const int buttonStateNotPressed = 1;
const int buttonStateHoldRelease = 2;
const int buttonStateTapRelease = 3;

const unsigned long buttonHoldLimit = 2000;

int previousButtonState = LOW;
unsigned long buttonPushStart = 0;
int buttonState = buttonStateNotPressed;
void updateButtonState()
{
  int currentButtonState = digitalRead(presetOneButtonPin);
  int compareButtonState = previousButtonState;
  previousButtonState = currentButtonState;

  if (compareButtonState == LOW && currentButtonState == HIGH) {
    buttonPushStart = millis();
    buttonState =  buttonStatePressed;
    return;
  }

  if (compareButtonState == HIGH && currentButtonState == LOW) {
    unsigned long holdTime = millis() - buttonPushStart;
    if (holdTime >= buttonHoldLimit) {
      buttonState = buttonStateHoldRelease;
      return;
    } else {
      buttonState = buttonStateTapRelease;
      return;
    }
  }

  if (compareButtonState == HIGH) {
    buttonState = buttonStatePressed;
    return;
  }

  buttonState = buttonStateNotPressed;
}

void updatePresetValues()
{
  presetPosOneX = posOneX;
  presetPosOneY = posOneY;
  presetPosTwoX = posTwoX;
  presetPosTwoY = posTwoY;
}

void choosePreset()
{
  posOneX = presetPosOneX;
  posOneY = presetPosOneY;
  posTwoX = presetPosTwoX;
  posTwoY = presetPosTwoY;
}


const unsigned long ledBlinkTime = 600;
int ledState = LOW;
void performLedActionBlink(unsigned long actionTime)
{
  int currentLedState = LOW;
  if (actionTime < ledBlinkTime) {
    currentLedState = HIGH;
  } else if (actionTime < (ledBlinkTime * 2)) {
    currentLedState = LOW;
  } else if (actionTime < (ledBlinkTime * 3)) {
    currentLedState = HIGH;
  }
  if (currentLedState != ledState) {
    ledState = currentLedState;
    digitalWrite(presetOneLedPin, ledState);
  }
}



const int ledActionIdle = 0;
const int ledActionBlink = 1;
const int ledActionLight = 2;

int currentAction = ledActionIdle;

unsigned long ledActionStartTime = 0;
const unsigned long totalLedBlinkTime = 2000;


int currentlyOnPreset()
{
  if (posOneX == presetPosOneX && posOneY == presetPosOneY && posTwoX == presetPosTwoX && posTwoY == presetPosTwoY) {
    return 1;
  }
  return 0;
}

void updateLedState()
{
  if (currentAction == ledActionIdle) {
    int currentPreset = currentlyOnPreset();
    if (buttonState == buttonStateHoldRelease) {
      ledActionStartTime = millis();
      currentAction = ledActionBlink;
    } else if (currentPreset > 0) {
      digitalWrite(presetOneLedPin, HIGH);
    } else {
      digitalWrite(presetOneLedPin, LOW);
    }
  } else if (currentAction == ledActionBlink) {
    unsigned long actionTime = millis() - ledActionStartTime;
    if (actionTime >= totalLedBlinkTime) {
      currentAction = ledActionIdle;
      ledActionStartTime = 0;
    } else {
      performLedActionBlink(actionTime);
    }
  }
}

void setup() {
  Serial.begin(9600);

  // LCD Setup
  lcd.begin(16, 2); // Initialize LCD-object with 16 columns and 2 lines
  pinMode(lcdBacklightPin, OUTPUT); //
  digitalWrite(lcdBacklightPin, HIGH); // Powers the display with light
  lcdInit();

  // Button/Led Setup
  pinMode(presetOneLedPin, OUTPUT);
  pinMode(presetOneButtonPin, INPUT);
}

void loop() {
  updateButtonState();
  updateLedState();
  if (buttonState == buttonStateTapRelease) {
    choosePreset();
  } else {
    updatePositionsFromJoysticks();
    
    if (buttonState == buttonStateHoldRelease) {
      updatePresetValues();
    }
  }
  
  lcdUpdate();

  sender.setControllerOneData(posOneX, posOneY);
  sender.setControllerTwoData(posTwoX, posTwoY);

  sender.sendControlData();
  delay(50);        // delay in between sends for stability
}

