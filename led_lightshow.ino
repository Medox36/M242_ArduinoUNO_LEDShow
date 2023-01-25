/*******************************************************

   Author:
   Lorenzo Giuntini (Medox36)

   Version:
   1.4.2

   Date:
   2023.1.25 (last modified)

   Description:
   Makes multpile LED blink at driffrent speeds. The speeds can be changed though a menu on an 16x2 lcd display on a lcd key shield.
   In addition, the master speed or clock speed can be changed with a potentiometer.
   Furthermore, the LED only blink when it is dark. 'Dark' is defined through a voltage divider in combinationwith a photoresistor.

   Connection:
   Plug the LCD Keypad Shield to the Arduino UNO.
   All wireings can be determined by looking at the constant variables "Pin Allocations".
   They can be changed, with exceptions to:
    - led_rgb_r & led_rgb_g & led_rgb_b -> must be connectet to PWM pins (marked with "~" on the ArduinoUNO board)
    - settingsButton -> must be connected to a interrupt pin (on the ArduinoUNO pins 2 and 3)
    - Pin A0 is reserved for the keys on the lcd key shield
    - all variables starting with "lcd_" -> due to the key shield having fixed pins the lcd pins are always connected to the same pins.
   
********************************************************/

#include <LiquidCrystal.h>

// Pin Allocations
//
const int keySheld = A0;      // reserved
const int potSpeed = A1;      // Potentiometer
const int pResistor = A2;     // Photoresistor
const int led_1 = A3;         //
const int led_2 = A4;         //
const int led_3 = A5;         //
const int settingsButton = 2; // 
const int led_rgb_r = 3;      // ~
const int lcd_DB4 = 4;        //
const int lcd_DB5 = 5;        // ~
const int lcd_DB6 = 6;        // ~
const int lcd_DB7 = 7;        //
const int lcd_RS = 8;         //
const int lcd_E = 9;          // ~
const int led_rgb_g = 10;     // ~
const int led_rgb_b = 11;     // ~

// Key Shield Keys
#define KeyRight 0
#define KeyUp 1
#define KeyDown 2
#define KeyLeft 3
#define KeySelect 4
#define NoKey 5

// Variables
//
volatile enum {IN, OUT} settingState = OUT; //
LiquidCrystal lcd(lcd_RS, lcd_E, lcd_DB4, lcd_DB5, lcd_DB6, lcd_DB7); // lcd (16x2)
unsigned long lastLedCicle = 0;  // last time the main led cycle executed
unsigned long led_1_cycle = 1000; // interval in ms at which the leds at pin led_1 blink
unsigned long led_1_lastCycle = 0; // last time the state of the leds at pin led_1 changed (mills() call)
unsigned long led_2_cycle = 1500; // interval in ms at which the leds at pin led_2 blink
unsigned long led_2_lastCycle = 0; // last time the state of the leds at pin led_2 changed (mills() call)
unsigned long led_3_cycle = 2000; // interval in ms at which the leds at pin led_3 blink
unsigned long led_3_lastCycle = 0; // last time the state of the leds at pin led_3 changed (mills() call)

//variables to hold RGB color intensities and directions
unsigned long lastRgbCycle = 0;
unsigned int red = 254;
unsigned int green = 1;
unsigned int blue = 127;
unsigned int red_direction = -1;
unsigned int green_direction = 1;
unsigned int blue_direction = -1;

// settings variables
String settingsText[] = {"RGB LED Speed", "LED 1 Speed", "LED 2 Speed", "LED 3 Speed"};
volatile unsigned int line = 0;
unsigned int cursor_col = 0;
unsigned int cursor_row = 0;
unsigned long rgb_speed = 1000;
unsigned long debounceDelay = 0;

/*
  set the pins to their respective modes, attach interrupt and start the lcd display.
*/
void setup() {
  pinMode(keySheld, INPUT);
  pinMode(pResistor, INPUT);
  pinMode(potSpeed, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(settingsButton, INPUT);
  pinMode(led_rgb_r, OUTPUT);
  pinMode(led_rgb_g, OUTPUT);
  pinMode(led_rgb_b, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(settingsButton), changeSettingsStage, CHANGE);

  lcd.begin(16, 2);
  lcd.cursor();
  lcd.blink();
  lcd.clear();
  lcd.setCursor(cursor_col, cursor_row);
  lcd.noDisplay();
}

/*
  Returns the caluclated speed based on the rotation of the potentiometer in ms.
  Value is between 100 and 1500.
*/
int speedCalc() {
  return map(analogRead(potSpeed), 0, 1023, 100, 1500);
}

/*
  Uses the photoresisor in a voltage divider with a 10k Ohm resistor to determine if it is dark or not. 
  Returns ture if the readed value is below 850. 
*/
bool isDark() {
  return analogRead(pResistor) < 850;
}

/*
  the main LED cycle
  
  Rotates through all diffrent LED cycles.
  The given parameter determines the time between to executed cycles.
*/
void led(int cicleDelay) {
  if (millis() - lastLedCicle >= cicleDelay) {
    _led(led_1_lastCycle, led_1_cycle, led_1);
    _led(led_2_lastCycle, led_2_cycle, led_2);
    _led(led_3_lastCycle, led_3_cycle, led_3);
    lastLedCicle = millis();
  }  
}

/*
  the LED sub cycle

  Changes the state of the LED if the time between the last cicle and now is greather or equal to the cycle speed.
  The parameters determine the last time the cycle changed it's state the cycle speed and the LED pin.
*/
void _led(unsigned long &lastCycle, unsigned long led_cycle, unsigned int led_pin) {
  if (millis() - lastCycle >= led_cycle) {
    if (digitalRead(led_pin) == HIGH) {
      digitalWrite(led_pin, LOW);
    } else {
      digitalWrite(led_pin, HIGH);
    }
    lastCycle = millis();
  }  
}

/* 
  Updates the RGB values of the RGB LED by writing the new values to the pins.
*/
void updateRgbLed() {
  analogWrite(led_rgb_r, red);
  analogWrite(led_rgb_g, green);
  analogWrite(led_rgb_b, blue);
}

/*
  the RGB LED cycle

  Sets ne RGB values, which increas upto 255 then decrease down to 0 and increase again.
*/
void rgbLed(int cicleDelay) {
  if (millis() - lastRgbCycle >= cicleDelay) {
    red += red_direction;
    green += green_direction;
    blue += blue_direction;

    if (red >= 255 || red <= 0) {
      red_direction = red_direction * -1;
    }
    if (green >= 255 || green <= 0 ){
      green_direction = green_direction * -1;
    }
    if (blue >= 255 || blue <= 0) {
      blue_direction = blue_direction * -1;
    }
    updateRgbLed();
    lastRgbCycle = millis();
  }
}

/*
  turns off all LEDs and resets the timers for a synchronous restart
*/
void ledsOff() {
  digitalWrite(led_1, LOW);
  led_1_lastCycle = 0;
  digitalWrite(led_2, LOW);
  led_2_lastCycle = 0;
  digitalWrite(led_3, LOW);
  led_3_lastCycle = 0;
  analogWrite(led_rgb_r, 255);
  analogWrite(led_rgb_g, 255);
  analogWrite(led_rgb_b, 255);
}

/*
  returns the currently pressed key on the lcd key shield
*/
int settingsKeyState() {
  int analogKeyVal = analogRead(keySheld);
  if (analogKeyVal > 1000) return NoKey;
  if (analogKeyVal < 50) return KeyRight;
  if (analogKeyVal < 195) return KeyUp;
  if (analogKeyVal < 380) return KeyDown;
  if (analogKeyVal < 555) return KeyLeft;
  if (analogKeyVal < 790) return KeySelect;
 
  return NoKey;
}

/*
  Moves the cursor in the settings menu up.

  The lines on the lcd display are only shifted/scrolled if the cursor is on the top line.
*/
void moveUp() {
  if (line > 0) {
    if (cursor_row == 1) {
      cursor_row = 0;
      line -= 1;
      lcd.setCursor(15, 0);
    } else if (cursor_row == 0) {
      line -= 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(settingsText[line]);
      lcd.setCursor(0, 1);
      lcd.print(settingsText[line + 1]);
      lcd.setCursor(15, 0);
    }
  }
}

/*
  Moves the cursor in the settings menu down.

  The lines on the lcd display are only shifted/scrolled if the cursor is on the bottom line.
*/
void moveDown() {
  if (line < 3) {
    if (cursor_row == 0) {
      cursor_row = 1;
      line += 1;
      lcd.setCursor(15, 1);
    } else if (cursor_row == 1) {
      line += 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(settingsText[line - 1]);
      lcd.setCursor(0, 1);
      lcd.print(settingsText[line]);
      lcd.setCursor(15, 1);
    }
  }
}

/*
  Reprints the settings aka the menu at it's perviously left position.
*/
void reprintSettings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (cursor_row == 0) {
    lcd.print(settingsText[line]);
    lcd.setCursor(0, 1);
    lcd.print(settingsText[line + 1]);
  } else if (cursor_row == 1) {
    lcd.print(settingsText[line - 1]);
    lcd.setCursor(0, 1);
    lcd.print(settingsText[line]);
  }
  lcd.setCursor(15, cursor_row);
}

/*
  Listenes in a loop for lcd key shield settingsButton presses and changes the value of the currently selected setting.
  The currently sleected setting is defined by the given parameter as pointer to assure the values ar changed and saved properly.

  To exit the the loop the select key must be pressed.
*/
void changeSetting(unsigned long &value) {
  delay(200);
  int displayValue = value / 250;
  lcd.print(displayValue);
  int key = settingsKeyState();
  int rotation = 0;
  while (key != KeySelect) {
    if (key == KeyUp && displayValue < 8) {
      displayValue += 1;
      value = 250 * displayValue;
      lcd.setCursor(0, 1);
      lcd.print(displayValue);
    } else if (key == KeyDown && displayValue > 1) {
      displayValue -= 1;
      value = 250 * displayValue;
      lcd.setCursor(0, 1);
      lcd.print(displayValue);
    }
    key = settingsKeyState();
    delay(200);
  }
  reprintSettings();
}

/*
  selects the setting at which the menu cursor is and enables modification of the value.
*/
void selectCurrent() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(settingsText[line]);
  lcd.setCursor(0, 1);
  switch (line) {
    case 0:
      changeSetting(rgb_speed);
      break;
    case 1:
      changeSetting(led_1_cycle);
      break;
    case 2:
      changeSetting(led_2_cycle);
      break;
    case 3:
      changeSetting(led_3_cycle);
      break;
  }
}

/*
  the settings loop

  Upon call this method turns off the LEDs and turns on the lcd display.
  Then enters a loop to navigate the settings menu.
  If the settings settingsButton is pressed again the method exits the loop and turns the lcd display off.
  The LED turn back on again in the main loop, if it's dark.
*/
void settings() {
  ledsOff();
  // lcd on
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print(settingsText[0]);
  lcd.setCursor(0, 1);
  lcd.print(settingsText[1]);
  lcd.setCursor(15, 0);

  while(settingState == IN) {
    int key = settingsKeyState();
    switch (key) {
      case NoKey:
        continue;
      case KeyUp:
        moveUp();
        break;
      case KeyDown:
        moveDown();
        break;
      case KeySelect:
        selectCurrent();
        break;      
    }
    delay(200);
  }
  // lcd off
  lcd.clear();
  lcd.noDisplay();
}

/*
  changes/toggles the settings state on interrupt call on settings settingsButton
*/
void changeSettingsStage() {
  int buttonVal = digitalRead(settingsButton);
  if (buttonVal == LOW) {
    if (millis() - debounceDelay > 200) {
      if (settingState == IN) {
        settingState = OUT;
      } else {
        settingState = IN;
      }
    }
    debounceDelay = millis();
  }
}

/*
  the main loop

  Changes to settings menu if settingsButton was pressed
  or does an LED blink cycle if it is dark else turns the LEDs off.
*/
void loop() {
  if (settingState == IN) {
    settings();
  }
  if (isDark()) {
    led(speedCalc());
    rgbLed(rgb_speed / 100);
  } else {
    ledsOff();
  }
}
