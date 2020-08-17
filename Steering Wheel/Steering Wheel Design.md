# Custom Steering Wheel

Since COVID, I have decided to revert to becoming a degenerate and moving in with my parents again. This means I can't hog space for the fullmotion sim. That being said, I will be building the wheel first.

### Electronics

Theoretically any wheel would work, the one I have on hand is a Madcatz xbox wheel. This xbox wheel has all the buttons, a vibration motor built in, is well built, and its weighted decently well. It also has somewhat of a shaft and journal bearing setup, making it easy to integrate my own system.

I have decoded the spaghetti of wiring coming from the wheel. The buttons are all active low. The color coding is as follows:

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

It took a while to gather up all the research required to put together the required libraries and hardware required to pull this off. USB and HID in general are both a big pain. Luckily I found [this library](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary). Unluckily, I had already bought a STM32 and the library was only good for the 32u4. The general idea flowchart is something like this:

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

![HBridgeSimulation](\pics\HBridgeSimulation.png)

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

![TrafoInductanceTest](\pics\TrafoInductanceTest.png)

The circuit consisted of the transformer (secondary open) and a 100 ohm resistor in series. The smaller waveform is the measurement across the resistor while a signal was applied.
$$
X_L=2\pi f L
$$

$$
L=\frac{2\pi f}{X_L}
$$

$$
\frac{1.44V}{.36V}=\frac{X_L}{100Ω}
$$

$$
X_L=400Ω
$$

$$
L=\frac{2\pi 103.1hz}{400Ω}=1.62H
$$

At about 14 amps, a 10,000uF capacitor will provide a 6v swing signal. Good enough for the girls I go out with

![PowerSupplySim](\pics\PowerSupplySim.png)

At 2 amps, the signal is a 1V swing. Nice!

![PowerSupplySimLowCurrent](C:\Users\joesp\Desktop\fullmotion racing sim\pics\PowerSupplySimLowCurrent.png)

These measurements are taken at the worst case scenario. In reality, it will be PWM'd to only 8-10V, the motor is rated for 6V. 

Here is the complete schematic. Diptrace files should be in their respective directories. Because I'm going to be building this on a perfboard, a PCB will not be generated. If someone wants to do that, do a pull request.

![image-20200816172851211](C:\Users\joesp\AppData\Roaming\Typora\typora-user-images\image-20200816172851211.png)

## Mechanics

I conveniently found a old IOmega NAS 

