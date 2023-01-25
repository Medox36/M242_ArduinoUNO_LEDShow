# M242_ArduinoUNO_LEDShow

This repository holds the code to a project created at school.<br><br>
The code makes multpile LEDs blink at driffrent speeds. The speeds can be changed though a menu on an 16x2 lcd display sitting on a lcd key shield.<br>
In addition, the master speed or clock speed can be changed with a potentiometer.<br>
Furthermore, the LED only blink when it is dark. 'Dark' is defined through a voltage divider in combinationwith a photoresistor.<br>

## Used Parts
* ArduinoUNO
* LCD 16x2 
* LCD Keypad Shield
* RGB LED (common anode)
* Three different coloured LEDs
* Potentiometer (linear, 10kΩ)
* Photoresistor (not sure but a 10kΩ one)
* Button (normally open)
* Resistors
  * 1x 10kΩ Resistor
  * 1x 1kΩ Resistor (anotherone will do as well, is for the button)
  * 3x 40Ω Resistors
  
Optional:
> * Juper cables
> * Breadboars

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

## Included Libraries
[LiquidCristal](https://github.com/arduino-libraries/LiquidCrystal)

## Circuit Diagramm

May be added in the near future.
