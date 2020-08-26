# Custom Steering Wheel

## Theory of Operation

-Atmega 32u4

-[FFB Library](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary) (thanks!)

-Big mige 130st

-2 bay NAS chassis for housing

## Schematics



## MadCatz xbox wheel

|             Color/Stripe              |         Function         |
| :-----------------------------------: | :----------------------: |
|              Blue/Black              |           GND            |
|              Green/White              |            Y             |
|                 Green                 |            X             |
|             Orange/White              |            B             |
|             Yellow/Black              |            A             |
|              Blue/White               |           RSB            |
|               Red/White               |           LSB            |
|                 Blue                  | RB (Flappy paddle right) |
| [Brown](https://youtu.be/lzqCQxi3ENE) | LB (Flappy paddle left)  |
| Yellow/Orange | Back |
| Black | Start |
| Gray | Xbox Button |
| Yellow | Up |
|              Gray/White               |               Down                |
|              Brown/White              |               Left                |
|                Orange                 |         Rumble Motor Left         |
|                Purple                 |        Rumble Motor Right         |
|                  Red                  |                Vcc                |
| White | 5V |
| Orange/Black | P1 LED (active low, use resistor) |
| Red/Black | P2 LED (active low, use resistor) |
| Brown/Black | P3 LED (active low, use resistor) |
| Pink/Black | P4 LED (active low, use resistor) |

The steering wheel will carry an onboard SPI IO expander (mcp23s17), interfacing with all the buttons.

A spring cable will carry the SPI signals between the 32u4 and the wheel.

## Microcontroller Flowchart

![diagram](/pics/diag1.png)

Inside the ATmega32u4, the code will go something like this:

![diagram](/pics/diag2.png)

## H bridge Driver



## Power supply


