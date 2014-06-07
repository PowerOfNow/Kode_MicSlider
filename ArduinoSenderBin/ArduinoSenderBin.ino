#include <LiquidCrystal.h>

#include <MicSliderComm.h>

// Objects
MicSliderCommSender sender;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin setup
const int joystickOneXPin       = A0;
const int joystickOneYPin       = A1;
const int joystickTwoXPin       = A2;
const int joystickTwoYPin       = A3;
const int lcdBacklightPin       = 7;
const int presetOneButtonPin    = 8;
const int presetTwoButtonPin    = 9;
const int presetThreeButtonPin  = 10;
const int presetFourButtonPin   = 11;
const int presetOneLedPin       = 13;
const int presetTwoLedPin       = 14;
const int presetThreeLedPin     = 15;
const int presetFourLedPin      = 16;

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

// 'Enum' buttonstate
const int buttonStatePressed      = 0;
const int buttonStateNotPressed   = 1;
const int buttonStateHoldRelease  = 2;
const int buttonStateTapRelease   = 3;

// Button constants
const unsigned long buttonHoldLimit = 2000;

// LED constants
const unsigned long ledBlinkTime = 600;
const unsigned long ledTotalBlinkTime = 2000;

////
// Position related
const int posOneXIndex = 0;
const int posOneYIndex = 1;
const int posTwoXIndex = 2;
const int posTwoYIndex = 3;

// Position presets
int presetOne[4]    = {1500, 1500, 1500, 1500};
int presetTwo[4]    = {   0,    0,    0,    0};
int presetThree[4]  = {   0,    0,    0,    0};
int presetFour[4]   = {   0,    0,    0,    0};

// Current position
// Only modify these through updatePos*()
int currentPosition[4] = {1500, 1500, 1500, 1500};

void updatePosOneX(int delta)
{
  currentPosition[posOneXIndex] = constrain(currentPosition[posOneXIndex] + delta, C_ONE_MIN_X_POS, C_ONE_MAX_X_POS);
}

void updatePosOneY(int delta)
{
  currentPosition[posOneYIndex] = constrain(currentPosition[posOneYIndex] + delta, C_ONE_MIN_Y_POS, C_ONE_MAX_Y_POS);
}

void updatePosTwoX(int delta)
{
  currentPosition[posTwoXIndex] = constrain(currentPosition[posTwoXIndex] + delta, C_TWO_MIN_X_POS, C_TWO_MAX_X_POS);
}

void updatePosTwoY(int delta)
{
  currentPosition[posTwoYIndex] = constrain(currentPosition[posTwoYIndex] + delta, C_TWO_MIN_Y_POS, C_TWO_MAX_Y_POS);
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
  lcd.print(lcdPercentString(map(currentPosition[posOneXIndex], C_ONE_MIN_X_POS, C_ONE_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdFrontBackPos, 1);
  lcd.print(lcdPercentString(map(currentPosition[posOneYIndex], C_ONE_MIN_Y_POS, C_ONE_MAX_Y_POS, 0, 100)));

  lcd.setCursor(lcdPanPos, 1);
  lcd.print(lcdPercentString(map(currentPosition[posTwoXIndex], C_TWO_MIN_X_POS, C_TWO_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdTiltPos, 1);
  lcd.print(lcdPercentString(map(currentPosition[posTwoYIndex], C_TWO_MIN_Y_POS, C_TWO_MAX_Y_POS, 0, 100)));
}


// PRESET FUNCTIONS
int updateButtonState(int chosenPreset, int buttonPin)
{
  static int previousButtonStateOne   = LOW;
  static int previousButtonStateTwo   = LOW;
  static int previousButtonStateThree = LOW;
  static int previousButtonStateFour  = LOW;

  static unsigned long buttonPushStartOne = 0;
  static unsigned long buttonPushStartTwo = 0;
  static unsigned long buttonPushStartThree = 0;
  static unsigned long buttonPushStartFour = 0;

  int currentButtonState = digitalRead(buttonPin);
  int compareButtonState = 0;
  if (chosenPreset == 1) {
    compareButtonState = previousButtonStateOne;
    previousButtonStateOne = currentButtonState;
  } else if (chosenPreset == 2) {
    compareButtonState = previousButtonStateTwo;
    previousButtonStateTwo = currentButtonState;
  } else if (chosenPreset == 3) {
    compareButtonState = previousButtonStateThree;
    previousButtonStateThree = currentButtonState;
  } else if (chosenPreset == 4) {
    compareButtonState = previousButtonStateFour;
    previousButtonStateFour = currentButtonState;
  }
  

  if (compareButtonState == LOW && currentButtonState == HIGH) {
    if (chosenPreset == 1) {
      buttonPushStartOne = millis();
    } else if (chosenPreset == 2) {
      buttonPushStartTwo = millis();
    } else if (chosenPreset == 3) {
      buttonPushStartThree = millis();
    } else if (chosenPreset == 4) {
      buttonPushStartFour = millis();
    }
    return buttonStatePressed;
  }

  if (compareButtonState == HIGH && currentButtonState == LOW) {
    unsigned long holdTime = 0;
    if (chosenPreset == 1) {
      holdTime = millis() - buttonPushStartOne;
    } else if (chosenPreset == 2) {
      holdTime = millis() - buttonPushStartTwo;
    } else if (chosenPreset == 3) {
      holdTime = millis() - buttonPushStartThree;
    } else if (chosenPreset == 4) {
      holdTime = millis() - buttonPushStartFour;
    }

    if (holdTime >= buttonHoldLimit) {
      return buttonStateHoldRelease;
    } else {
      return buttonStateTapRelease;
    }
  }

  if (compareButtonState == HIGH) {
    return buttonStatePressed;
  }

  return buttonStateNotPressed;
}

int buttonStateOne   = buttonStateNotPressed;
int buttonStateTwo   = buttonStateNotPressed;
int buttonStateThree = buttonStateNotPressed;
int buttonStateFour  = buttonStateNotPressed;
void updateButtonStates()
{
  buttonStateOne    = updateButtonState(1, presetOneButtonPin);
  buttonStateTwo    = updateButtonState(2, presetTwoButtonPin);
  buttonStateThree  = updateButtonState(3, presetThreeButtonPin);
  buttonStateFour   = updateButtonState(4, presetFourButtonPin);
}


void presetTurnLedsOff()
{
  digitalWrite(presetOneLedPin, LOW);
  digitalWrite(presetTwoLedPin, LOW);
  digitalWrite(presetThreeLedPin, LOW);
  digitalWrite(presetFourLedPin, LOW);
}

void presetSetLedState(int ledState, int chosenPreset)
{
  if (chosenPreset == 1) {
    digitalWrite(presetOneLedPin, ledState);
  } else if (chosenPreset == 2) {
    digitalWrite(presetTwoLedPin, ledState);
  } else if (chosenPreset == 3) {
    digitalWrite(presetThreeLedPin, ledState);
  } else if (chosenPreset == 4) {
    digitalWrite(presetFourLedPin, ledState);
  }
}

void updatePresetValues(int chosenPreset)
{
  if (chosenPreset == 1) {
    presetOne[posOneXIndex] = currentPosition[posOneXIndex];
    presetOne[posOneYIndex] = currentPosition[posOneYIndex];
    presetOne[posTwoXIndex] = currentPosition[posTwoXIndex];
    presetOne[posTwoYIndex] = currentPosition[posTwoYIndex];
  } else if (chosenPreset == 2) {
    presetTwo[posOneXIndex] = currentPosition[posOneXIndex];
    presetTwo[posOneYIndex] = currentPosition[posOneYIndex];
    presetTwo[posTwoXIndex] = currentPosition[posTwoXIndex];
    presetTwo[posTwoYIndex] = currentPosition[posTwoYIndex];
  } else if (chosenPreset == 3) {
    presetThree[posOneXIndex] = currentPosition[posOneXIndex];
    presetThree[posOneYIndex] = currentPosition[posOneYIndex];
    presetThree[posTwoXIndex] = currentPosition[posTwoXIndex];
    presetThree[posTwoYIndex] = currentPosition[posTwoYIndex];
  } else if (chosenPreset == 4) {
    presetFour[posOneXIndex] = currentPosition[posOneXIndex];
    presetFour[posOneYIndex] = currentPosition[posOneYIndex];
    presetFour[posTwoXIndex] = currentPosition[posTwoXIndex];
    presetFour[posTwoYIndex] = currentPosition[posTwoYIndex];
  }
  
}

void choosePreset(int chosenPreset)
{
  if (chosenPreset == 1) {
    currentPosition[posOneXIndex] = presetOne[posOneXIndex];
    currentPosition[posOneYIndex] = presetOne[posOneYIndex];
    currentPosition[posTwoXIndex] = presetOne[posTwoXIndex];
    currentPosition[posTwoYIndex] = presetOne[posTwoYIndex];
  } else if (chosenPreset == 2) {
    currentPosition[posOneXIndex] = presetTwo[posOneXIndex];
    currentPosition[posOneYIndex] = presetTwo[posOneYIndex];
    currentPosition[posTwoXIndex] = presetTwo[posTwoXIndex];
    currentPosition[posTwoYIndex] = presetTwo[posTwoYIndex];
  } else if (chosenPreset == 3) {
    currentPosition[posOneXIndex] = presetThree[posOneXIndex];
    currentPosition[posOneYIndex] = presetThree[posOneYIndex];
    currentPosition[posTwoXIndex] = presetThree[posTwoXIndex];
    currentPosition[posTwoYIndex] = presetThree[posTwoYIndex];
  } else if (chosenPreset == 4) {
    currentPosition[posOneXIndex] = presetFour[posOneXIndex];
    currentPosition[posOneYIndex] = presetFour[posOneYIndex];
    currentPosition[posTwoXIndex] = presetFour[posTwoXIndex];
    currentPosition[posTwoYIndex] = presetFour[posTwoYIndex];
  }
}

void performLedActionBlink(unsigned long actionTime, int activePreset)
{
  static int lastChange = -1;
  int numberOfBlinkChanges = actionTime / ledBlinkTime;

  if (lastChange != numberOfBlinkChanges) { // Only set led state when it has changed
    int ledState = ((numberOfBlinkChanges % 2) == 1) ? LOW : HIGH;
    
    presetSetLedState(ledState, activePreset);
    lastChange = numberOfBlinkChanges;
  }
}

int currentlyOnPreset()
{
  if (currentPosition[posOneXIndex] == presetOne[posOneXIndex] &&
      currentPosition[posOneYIndex] == presetOne[posOneYIndex] &&
      currentPosition[posTwoXIndex] == presetOne[posTwoXIndex] &&
      currentPosition[posTwoYIndex] == presetOne[posTwoYIndex])
  {
    return 1;
  } else if (currentPosition[posOneXIndex] == presetTwo[posOneXIndex] &&
             currentPosition[posOneYIndex] == presetTwo[posOneYIndex] &&
             currentPosition[posTwoXIndex] == presetTwo[posTwoXIndex] &&
             currentPosition[posTwoYIndex] == presetTwo[posTwoYIndex])
  {
    return 2;
  } else if (currentPosition[posOneXIndex] == presetThree[posOneXIndex] &&
             currentPosition[posOneYIndex] == presetThree[posOneYIndex] &&
             currentPosition[posTwoXIndex] == presetThree[posTwoXIndex] &&
             currentPosition[posTwoYIndex] == presetThree[posTwoYIndex])
  {
    return 3;
  } else if (currentPosition[posOneXIndex] == presetFour[posOneXIndex] &&
             currentPosition[posOneYIndex] == presetFour[posOneYIndex] &&
             currentPosition[posTwoXIndex] == presetFour[posTwoXIndex] &&
             currentPosition[posTwoYIndex] == presetFour[posTwoYIndex])
  {
    return 4;
  }
  return 0;
}

void updateLedState()
{
  static unsigned long ledActionStartTime = 0;
  static boolean ledIdle = true;
  static int currentPreset = 0;
  static int currentBlinkingPreset = 0;

  if (ledIdle) {
    currentPreset = currentlyOnPreset();
    if (buttonStateOne == buttonStateHoldRelease) {
      currentBlinkingPreset = 1;
      ledActionStartTime = millis();
      ledIdle = false;
    } else if (buttonStateTwo == buttonStateHoldRelease) {
      currentBlinkingPreset = 2;
      ledActionStartTime = millis();
      ledIdle = false;
    } else if (buttonStateThree == buttonStateHoldRelease) {
      currentBlinkingPreset = 3;
      ledActionStartTime = millis();
      ledIdle = false;
    } else if (buttonStateFour == buttonStateHoldRelease) {
      currentBlinkingPreset = 4;
      ledActionStartTime = millis();
      ledIdle = false;
    } else if (currentPreset > 0) {
      presetSetLedState(HIGH, currentPreset);
    } else {
      presetTurnLedsOff();
    }
  } else {
    unsigned long actionTime = millis() - ledActionStartTime;
    if (actionTime >= ledTotalBlinkTime) {
      ledIdle = true;
      ledActionStartTime = 0;
    } else {
      performLedActionBlink(actionTime, currentBlinkingPreset);
    }
  }
}

void setup() {
  Serial.begin(9600);

  // LCD Setup
  lcd.begin(16, 2);                     // Initialize LCD, 16 columns and 2 lines
  pinMode(lcdBacklightPin, OUTPUT);
  digitalWrite(lcdBacklightPin, HIGH);  // Powers the display with light
  lcdInit();                            // Writes headers

  // Button/Led Setup
  pinMode(presetOneLedPin, OUTPUT);
  pinMode(presetOneButtonPin, INPUT);
  pinMode(presetTwoLedPin, OUTPUT);
  pinMode(presetTwoButtonPin, INPUT);
  pinMode(presetThreeLedPin, OUTPUT);
  pinMode(presetThreeButtonPin, INPUT);
  pinMode(presetFourLedPin, OUTPUT);
  pinMode(presetFourButtonPin, INPUT);
}

void loop() {
  updateButtonStates();
  updateLedState();
  if (buttonStateOne == buttonStateTapRelease) {
    choosePreset(1);
  } else if (buttonStateTwo == buttonStateTapRelease) {
    choosePreset(2);
  } else if (buttonStateThree == buttonStateTapRelease) {
    choosePreset(3);
  } else if (buttonStateFour == buttonStateTapRelease) {
    choosePreset(4);
  } else {
    updatePositionsFromJoysticks();
    
    if (buttonStateOne == buttonStateHoldRelease) {
      updatePresetValues(1);
    } else if (buttonStateTwo == buttonStateHoldRelease) {
      updatePresetValues(2);
    } else if (buttonStateThree == buttonStateHoldRelease) {
      updatePresetValues(3);
    } else if (buttonStateFour == buttonStateHoldRelease) {
      updatePresetValues(4);
    }
  }
  
  lcdUpdate();

  sender.setControllerOneData(currentPosition[posOneXIndex], currentPosition[posOneYIndex]);
  sender.setControllerTwoData(currentPosition[posTwoXIndex], currentPosition[posTwoYIndex]);

  sender.sendControlData();
  delay(50); // delay in between sends for stability
}
