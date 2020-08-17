# Custom Steering Wheel

### Electronics

MadCatz xbox wheel decoding:

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

### Theory of Operation

-Atmega 32u4

-[FFB Library](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary) (thanks!)

-drill motor for "direct drive"

-old 2 bay NAS chassis for housing

```sequence
PC-Atmega32u4:FFB data
Atmega32u4--PC:Steering wheel angle, buttons
Atmega32u4-Motor Driver:Motor direction, force
potentiometer-Atmega32u4:
```

Inside the ATmega32u4, the code will go something like this:

```sequence
USB Interface-USB Library:USB
USB Library-ArduinoJoystickWithFFBLibrary:HID/PID FFB Data
ArduinoJoystickWithFFBLibrary--USB Library:Steering wheel angle
ArduinoJoystickWithFFBLibrary-MotorDriverFunction:Effect Data
```

### Schematics

#### H bridge Driver

This is a basic H bridge driver, supports PWM. Only thing is that there is absolutely no circuit protection, so should both analog pwm inputs be active, the HEXFETS will be no more.

The extra transistors tacked on are to create a 12V gate drive.

I simulated the circuit in [circuitmod](https://sourceforge.net/projects/circuitmod/). Although I had access to MultiSim through my uni, the mentioned software is more accessible to most people and is fine.

![HBridgeSimulation](/pics/HBridgeSimulation.png)

The non-full voltage swing across the 10 ohm is caused by the limitation of the simulator's mosfet model. Putting another one in parallel solves this issue, by using a more appropriate MOSFET in the final design will be a better solution

#### Power supply

To test the stall current of the 775 motor, I had a set of pliers locked on the axle and gave it power. At 6V, the motor drew roughly 4 amps. At 12V, the motor pegged my multimeter at 10 amps. Just by the sound test, 12V creates too many rpms for the 775 motor.

This sets the premises for the power supply. 

- My go-to is an ATX power supply, but given the ioMega StorCenter metal case I will be using, that would not fit. 
- A typical Dell/HP 19.5V laptop power adapter seems good, but it only puts out a few amps. The motor will trip the overcurrent protection within seconds of gameplay
- My only choice left is to build a custom power supply using a chunky 14.5V/1.6A transformer I had laying around

This at a glance seems like the worst choice, but hear me out. 

- The transformer itself has no circuit protection, this allows short term high current bursts of energy
- I can use capacitors to smooth out the current draw

To get an accurate simulation, the inductance of the transformer was measured via frequency generator and oscilloscope

![TrafoInductanceTest](/pics/TrafoInductanceTest.png)

The circuit consisted of the transformer (secondary open) and a 100 ohm resistor in series. The smaller waveform is the measurement across the resistor while a signal was applied.

Calculations

![formula](https://render.githubusercontent.com/render/math?math=X_L=2%20\pi%20fL)

![formula](https://render.githubusercontent.com/render/math?math=L=\frac{2%20\pi%20f}{X_L})

![formula](https://render.githubusercontent.com/render/math?math=\frac{1.44V}{.36V}=\frac{X_L}{100Ω})

![formula](https://render.githubusercontent.com/render/math?math=X_L=400Ω)

![formula](https://render.githubusercontent.com/render/math?math=L=\frac{2%20\pi%20103.1hz}{400Ω}=1.62H)


At about 14 amps, a 10,000uF capacitor will provide a 6v swing signal. Good enough for the girls I go out with

![PowerSupplySim](/pics/PowerSupplySim.png)

At 2 amps, the signal is a 1V swing. Nice!

![PowerSupplySimLowCurrent](/pics/PowerSupplySimLowCurrent.png)

Measurements reflect the worst case scenarios 

[Complete Schematic](https://github.com/joesphan/Fullmotion-Racing-Sim/blob/master/Steering%20Wheel/Controller/Schematics%20and%20simulation/FullSchematic.pdf)

## Mechanics

I conveniently found a old IOmega NAS 

