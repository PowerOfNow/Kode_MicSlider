#include <LiquidCrystal.h> //

#define BackLight  7  

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
lcd.begin(16, 2); // Initialize LCD-object with 16 columns and 2 lines
pinMode(BackLight,OUTPUT); //
digitalWrite(BackLight,HIGH); // Powers the display with light
}

void loop()
{
  
// Indicate position left to right
lcd.setCursor(0,0); // set the column and line position
lcd.print("LR"); // 
lcd.setCursor(0,1); // set the column and line position
lcd.print("59%"); //temporary example number

// Forslag til formel:
// Gir posisjon med tall mellom 0 og 100. Forutsetter at x aldri blir lik Max PWM.
// x = PWM verdien til enhver tid
//   ((Max PWM - Min PWM)/Max PWM - x PWM ) * 100 
// Alle desimaler må rundes av 

// Indicate position front to back
lcd.setCursor(4,0); // set the column and line position
lcd.print("FB");
lcd.setCursor(4,1); // set the column and line position
lcd.print("28%");//temporary example number

// Indicate panning position
lcd.setCursor(8,0); // set the column and line position
lcd.print("Pan");
lcd.setCursor(8,1); // set the column and line position
lcd.print("13%");//temporary example number

// Indicate tilt position
lcd.setCursor(12,0); // set the column and line position
lcd.print("Tilt");
lcd.setCursor(12,1); // set the column and line position
lcd.print("10%");//temporary example number



}

