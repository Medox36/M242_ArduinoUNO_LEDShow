# M242_ArduinoUNO_LEDShow

This repository holds the code to a project created at school.<br><br>
The code makes multpile LEDs blink at driffrent speeds and cicles through (almoast) all colours on an RGB LED. The speeds can be changed though a menu on an 16x2 lcd display sitting on a lcd key shield.<br>
In addition, the master speed or clock speed can be changed with a potentiometer.<br>
Furthermore, the LED only blink when it is dark. 'Dark' is defined through a voltage divider in combination with a photoresistor.<br>
> Futher documentation und functionalities can be found in the code within the ``.ino`` file

## Used Parts
> Required:
* ArduinoUNO
* LCD 16x2 
* LCD Keypad Shield
* RGB LED (common anode)
* Three different coloured LEDs
* Potentiometer (linear, 10kΩ)
* Photoresistor (not sure but a 10kΩ one)
* Button (normally open)
* Resistors
  * 2x 10kΩ Resistor
  * 6x 100Ω Resistors
* Wires
  
> Optional:
* Jumper cables
* Breadboars

## Connection
Plug the LCD Keypad Shield onto the Arduino UNO.
All further wire connections can be determined by looking at the Pin Allocations futher down or by looking at the constant variables Pin Allocations in the code.

## Pin Allocations
> A0 - reseved for Buttons on LCD Keypad Shield<br>
> A1 - Potentiometer<br>
> A2 - Photoresistor<br>
> A3 - LED(s) 1<br>
> A4 - LED(s) 2<br>
> A5 - LED(s) 3<br>

> D2  - Button to get into the settings menu<br>
> D3  - RGB LED red<br>
> D4  - LCD DB4<br>
> D5  - LCD DB5<br>
> D6  - LCD DB6<br>
> D7  - LCD DB7<br>
> D8  - LCD RS<br>
> D9  - LCD E<br>
> D10 - RGB LED green<br>
> D11 - RGB LED blue<br>

Pin allocations can be changed by setting the constant variables in the code respecively
Changes can be done wit hthe following exceptions:
* ``led_rgb_r``, ``led_rgb_g`` and ``led_rgb_b`` must be connectet to a PWM pin (marked with ``~`` on the ArduinoUNO board)
* ``settingsButton`` must be connected to a interrupt pin (on the ArduinoUNO pins 2 and 3)
* Pin AO (variable ``keySheld``) is reserved for the keys on the lcd key shield
* All variables starting with ``lcd_`` have to stay the same and must not be changed. The reason beeing due to the key shield having fixed pin thoughput from the lcd pins. Which means that the lcd pins are always connected to the same lcd keypad shield pins.

## Included Libraries
[LiquidCristal](https://github.com/arduino-libraries/LiquidCrystal)

## Circuit Diagramm

May be added in the near future.
