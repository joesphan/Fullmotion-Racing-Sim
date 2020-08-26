# Custom Steering Wheel

## Theory of Operation

-needs to be easy to replicate on the HFT corded drill

-Atmega 32u4

-[FFB Library](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary) (thanks!)

-drill motor for "direct drive"

-minimize the use of 3d printed high stress components

-3d print a encoder wheel and use IR sensors to implement a rotary encoder built into the motor

-old 2 bay NAS chassis for housing

## Schematics

[Complete Schematic (pdf)](https://github.com/joesphan/Fullmotion-Racing-Sim/blob/master/Steering%20Wheel/Controller/Schematics%20and%20simulation/FullSchematic.pdf)

[Complete Schematic (diptrace)](https://github.com/joesphan/Fullmotion-Racing-Sim/blob/master/Steering%20Wheel/Controller/Schematics%20and%20simulation/FullSchematic.dch)

Note: 12V net port is not actually 12V

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

## Rotary Encoder

To eliminate the need of any mechanical connection between the drill and parts, the cooling fan inside the motor will be replaced with a rotary encoder wheel.

## H bridge Driver

This is a basic H bridge driver, supports PWM. Only thing is that there is absolutely no circuit protection, so should both analog pwm inputs be active, the HEXFETS will be no more.

The extra transistors tacked on are to create a 12V gate drive.

I simulated the circuit in [circuitmod](https://sourceforge.net/projects/circuitmod/). Although I had access to MultiSim through my uni, the mentioned software is more accessible to most people and is fine.

![HBridgeSimulation](/pics/HBridgeSimulation.png)

The non-full voltage swing across the 10 ohm is caused by the limitation of the simulator's mosfet model. Putting another one in parallel solves this issue, by using a more appropriate MOSFET in the final design will be a better solution

## Power supply

The drill is a 120V drill. Running it on 12V DC provides good Locking torque, yet lacks power to come out of a locked stall.

A voltage doubler circuit would be utilized to boost the voltage to a decent amount.

### Transformer calculations

![TrafoInductanceTest](/pics/TrafoInductanceTest.png)

The circuit consisted of the transformer (secondary open) and a 100 ohm resistor in series. The smaller waveform is the measurement across the resistor while a signal was applied.

Calculations

![formula](https://render.githubusercontent.com/render/math?math=X_L=2%20\pi%20fL)

![formula](https://render.githubusercontent.com/render/math?math=L=\frac{2%20\pi%20f}{X_L})

![formula](https://render.githubusercontent.com/render/math?math=\frac{1.44V}{.36V}=\frac{X_L+100}{100ohm})

![formula](https://render.githubusercontent.com/render/math?math=X_L=300ohm)

![formula](https://render.githubusercontent.com/render/math?math=L=\frac{300ohm}{2%20\pi%20103.1hz}=.463H)

### Voltage doubler and filter simulation

![PowerSupplySim](/pics/PowerSupplySim.png)

