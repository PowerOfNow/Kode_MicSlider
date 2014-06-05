#include <LiquidCrystal.h>

#include <MicSliderComm.h>

// LCD backlight PIN
#define BackLight  7

// Button pin
const int buttonPin = 8;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin


MicSliderCommSender sender;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// Constants/variables controller one (analog servos on rails)
const int C_ONE_X_DELTA = 5;
const int C_ONE_Y_DELTA = 5;
const int C_ONE_MIN_X_POS = 1000;
const int C_ONE_MAX_X_POS = 2000;
const int C_ONE_MIN_Y_POS = 1000;
const int C_ONE_MAX_Y_POS = 2000;
int posOneX = 1500; // Actual calculated current position of servo 1X
int posOneY = 1500; // Actual calculated current position of servo 1Y


// Constants/variables controller two (digital servos, tilt/rotate)
const int C_TWO_X_DELTA = 15;
const int C_TWO_Y_DELTA = 15;
const int C_TWO_MIN_X_POS = 1000;
const int C_TWO_MAX_X_POS = 2000;
const int C_TWO_MIN_Y_POS = 1000;
const int C_TWO_MAX_Y_POS = 2000;
int posTwoX = 1500; // Actual calculated current position of servo 2X
int posTwoY = 1500; // Actual calculated current position of servo 2Y


// PRESET values
int presetPosOneX = posOneX;
int presetPosOneY = posOneY;
int presetPosTwoX = posTwoX;
int presetPosTwoY = posTwoY;


// READ ONLY. Only write these values from updateJoypads()
int curJoyOneX = 512;
int curJoyOneY = 512;
int curJoyTwoX = 512;
int curJoyTwoY = 512;
void updateJoypads()
{
  curJoyOneX = analogRead(A0);
  curJoyOneY = analogRead(A1);
  curJoyTwoX = analogRead(A2);
  curJoyTwoY = analogRead(A3);
}

// General joypad queries
boolean queryMoveUp(int joypadValue)
{
   return joypadValue > 674;
}

boolean queryMoveDown(int joypadValue)
{
  return joypadValue < 350;
}
// Query Controller One
boolean queryMoveOneXUp()
{
  return queryMoveUp(curJoyOneX);
}
boolean queryMoveOneXDown()
{
  return queryMoveDown(curJoyOneX);
}
boolean queryMoveOneYUp()
{
  return queryMoveUp(curJoyOneY);
}
boolean queryMoveOneYDown()
{
  return queryMoveDown(curJoyOneY);
}

// Query Controller Two
boolean queryMoveTwoXUp()
{
  return queryMoveUp(curJoyTwoX);
}
boolean queryMoveTwoXDown()
{
  return queryMoveDown(curJoyTwoX);
}
boolean queryMoveTwoYUp()
{
  return queryMoveUp(curJoyTwoY);
}
boolean queryMoveTwoYDown()
{
  return queryMoveDown(curJoyTwoY);
}

// Inc/Dec Controller One
void decreaseOneXPos()
{
  if (posOneX > C_ONE_MIN_X_POS) {
    posOneX -= C_ONE_X_DELTA;
  }
  if (posOneX < C_ONE_MIN_X_POS) {
    posOneX = C_ONE_MIN_X_POS;
  }
}
void increaseOneXPos()
{
  if (posOneX < C_ONE_MAX_X_POS) {
    posOneX += C_ONE_X_DELTA;
  }
  if (posOneX > C_ONE_MAX_X_POS) {
    posOneX = C_ONE_MAX_X_POS;
  }
}
void decreaseOneYPos()
{
  if (posOneY > C_ONE_MIN_Y_POS) {
    posOneY -= C_ONE_Y_DELTA;
  }
  if (posOneY < C_ONE_MIN_Y_POS) {
    posOneY = C_ONE_MIN_Y_POS;
  }
}
void increaseOneYPos()
{
  if (posOneY < C_ONE_MAX_Y_POS) {
    posOneY += C_ONE_Y_DELTA;
  }
  if (posOneY > C_ONE_MAX_Y_POS) {
    posOneY = C_ONE_MAX_Y_POS;
  }
}

// Inc/Dec Controller Two
void decreaseTwoXPos()
{
  if (posTwoX > C_TWO_MIN_X_POS) {
    posTwoX -= C_TWO_X_DELTA;
  }
  if (posTwoX < C_TWO_MIN_X_POS) {
    posTwoX = C_TWO_MIN_X_POS;
  }
}
void increaseTwoXPos()
{
  if (posTwoX < C_TWO_MAX_X_POS) {
    posTwoX += C_TWO_X_DELTA;
  }
  if (posTwoX > C_TWO_MAX_X_POS) {
    posTwoX = C_TWO_MAX_X_POS;
  }
}
void decreaseTwoYPos()
{
  if (posTwoY > C_TWO_MIN_Y_POS) {
    posTwoY -= C_TWO_Y_DELTA;
  }
  if (posTwoY < C_TWO_MIN_Y_POS) {
    posTwoY = C_TWO_MIN_Y_POS;
  }
}
void increaseTwoYPos()
{
  if (posTwoY < C_TWO_MAX_Y_POS) {
    posTwoY += C_TWO_Y_DELTA;
  }
  if (posTwoY > C_TWO_MAX_Y_POS) {
    posTwoY = C_TWO_MAX_Y_POS;
  }
}

// Move Controller One
boolean moveOneXUp()
{
  if (queryMoveOneXUp()) {
    increaseOneXPos();
    return true;
  }
  return false;
}
boolean moveOneXDown()
{
  if (queryMoveOneXDown()) {
    decreaseOneXPos();
    return true;
  }
  return false;
}
boolean moveOneYUp()
{
  if (queryMoveOneYUp()) {
    increaseOneYPos();
    return true;
  }
  return false;
}
boolean moveOneYDown()
{
  if (queryMoveOneYDown()) {
    decreaseOneYPos();
    return true;
  }
  return false;
}

// Move Controller Two
boolean moveTwoXUp()
{
  if (queryMoveTwoXUp()) {
    increaseTwoXPos();
    return true;
  }
  return false;
}
boolean moveTwoXDown()
{
  if (queryMoveTwoXDown()) {
    decreaseTwoXPos();
    return true;
  }
  return false;
}
boolean moveTwoYUp()
{
  if (queryMoveTwoYUp()) {
    increaseTwoYPos();
    return true;
  }
  return false;
}
boolean moveTwoYDown()
{
  if (queryMoveTwoYDown()) {
    decreaseTwoYPos();
    return true;
  }
  return false;
}

void debugInfo()
{
  if (queryMoveOneXUp()) {
    Serial.println("Move X UP");
  } else if (queryMoveOneXDown()) {
    Serial.println("Move X Down");
  }

  if (queryMoveOneYUp()) {
    Serial.println("Move Y UP");
  } else if (queryMoveOneYDown()) {
    Serial.println("Move Y Down");
  }

  Serial.print("1X:"); Serial.print(posOneX); Serial.print(" 1Y:"); Serial.print(posOneY); Serial.println();
}

void calculatePositions()
{
  static boolean previousState = false;
  static int previousInt = posOneX;
  boolean currentStateOne = moveOneXUp() || moveOneXDown() || moveOneYUp() || moveOneYDown();
  boolean currentStateTwo = moveTwoXUp() || moveTwoXDown() || moveTwoYUp() || moveTwoYDown();
  /*
  if (currentState != previousState) {
    if (previousState) {
      int backtrack = abs(posOneX - previousInt) / 10;
      if (posOneX > previousInt) {
        posOneX -= 50;
      } else {
        posOneX += 50;
      }
      previousInt = posOneX;
    }
    previousState = currentState;

  }
  */
  /*
  if (queryMoveOneXUp()) {
    posOneX = 1;
  } else if (queryMoveOneXDown()) {
    posOneX = -1;
  } else {
    posOneX = 0;
  }
  */
}

const int lcdLeftRightPos = 0;
const int lcdFrontBackPos = 4;
const int lcdPanPos = 8;
const int lcdTiltPos = 12;

void lcdSetHeaders()
{
  lcd.setCursor(lcdLeftRightPos, 0);
  lcd.print("LR");

  lcd.setCursor(lcdFrontBackPos,0);
  lcd.print("FB");

  lcd.setCursor(lcdPanPos, 0);
  lcd.print("Pan");

  lcd.setCursor(lcdTiltPos, 0);
  lcd.print("Tilt");
}

void lcdInit() {
  static bool init = false;
  if (!init) {
    lcdSetHeaders();
    init = true;
  }
}

String formatPercentageString(int inputValue)
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
  lcd.print(formatPercentageString(map(posOneX, C_ONE_MIN_X_POS, C_ONE_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdFrontBackPos, 1);
  lcd.print(formatPercentageString(map(posOneY, C_ONE_MIN_Y_POS, C_ONE_MAX_Y_POS, 0, 100)));

  lcd.setCursor(lcdPanPos, 1);
  lcd.print(formatPercentageString(map(posTwoX, C_TWO_MIN_X_POS, C_TWO_MAX_X_POS, 0, 100)));

  lcd.setCursor(lcdTiltPos, 1);
  lcd.print(formatPercentageString(map(posTwoY, C_TWO_MIN_Y_POS, C_TWO_MAX_Y_POS, 0, 100)));
}

void setup() {
  Serial.begin(9600);

  // LCD Setup
  lcd.begin(16, 2); // Initialize LCD-object with 16 columns and 2 lines
  pinMode(BackLight, OUTPUT); //
  digitalWrite(BackLight, HIGH); // Powers the display with light
  lcdInit();

  // Button/Led Setup
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

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
  int currentButtonState = digitalRead(buttonPin);
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
    digitalWrite(ledPin, ledState);
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
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
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


void loop() {
  updateButtonState();
  updateLedState();
  if (buttonState == buttonStateTapRelease) {
    choosePreset();
  } else {
    updateJoypads();
    calculatePositions();
    
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





