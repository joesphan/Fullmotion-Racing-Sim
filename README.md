# Fullmotion-Racing-Sim
This is a dream project of mine for a while. Until I can price out a spec sheet that is under $300, I won't start building any hardware.
## Goals
* 5 axis
  * pitch
  * roll
  * heave
  * surge, sort of, simulated through tilting?
  * sway, same as above
  ![axis](/pics/dof.png)
  
* At least 45 degrees of motion 
  * +/- 22.5 degrees, modeled by D series pickup in BeamNG
   
* Somewhat accurate bump simulation, very fast acceleration
  * roughly 3,800 m/s/s to simulate a 45 degree ramp (1 x 1 meter) at 100km/h
  * enough to feel back pain
   
* Compatibility across games

## Inspiration:
https://www.xsimulator.net/community/threads/flypt-6dof-brushless-diy-actuators.10799/
* Problems
  * Too expensive for my taste
  * Motors cost $120 a piece, they need 6 of them
* Good
  * I like the design, it simplifies the design of moving in all of the axis
  * Easy calculations, all based on triangulation
* Better
  * Using the actuator layout, I think I can implment a pneumatic air system. As long as my HFT compressor can keep up with air demands, this shouldn't be an issue.
* Cost cuts
  * No point in yaw for driving, sway and surge can be simulated. This leaves me with only needing 3 actuators
  
https://www.simcraft.com/apex-pro-6dof-racing-simulator-chassis-motion-system-yaw-pitch-roll-sway-surge-heave-independent-full-motion-driver-development-full-service/
* ultimate goal
## Ideas
* Use FPGA instead of microcontroller for processing, for lowest latency
* Serial port communication
* Race controls
  * Ebay ricer steering wheel and seat
  * Junkyard throttle body and accelerator pedal
  * Junkyard brake and clutch booster
  * Accurate feel H pattern shifter

# Brain log
## version 0.1
Electronics:
* usb to rs232
* Altera ep2c5t144C8 for triangulation calculations
* Pneumatic solenoids
* Custom driver board
 
Software:
* Figure out how to grab data from BeamNG, either by simulating a controller, or directly grabbing lua
* FPGA dev, find the equations for caluclating triangle based on the input data
 
Hardware:
* DIY 3" pipe as pneumatic cylinders
 
Calculations:
* lengths of actuators for a equalateral triangle seat base of 1m sides
  * tan(20) = ~14", I will make it 24"
  * 3" air cylinder diameter
* air compressor spec requirement
  * cylinder volume: 0.097801 cubic feet
  * total cylinder volume: .2934 cubic feet
  * [harbor freight compressor](https://www.harborfreight.com/air-tools-compressors/air-compressors-tanks/8-gallon-2-hp-125-psi-oil-lube-air-compressor-68740.html): 4.5 CFM, derate by 75% to 3.375 CFM
  * equates to 11.5 full inflates per minute (moving from bottom to top)
  * [if this becomes a problem I'll pickup this bad boy and a shitty toyota engine](https://www.harborfreight.com/air-tools-compressors/air-compressors-tanks/145-psi-5-hp-twin-cylinder-air-compressor-pump-67698.html) 
  
## version 0.2
Electronics:
* RS-232
* Altera ep2c5t144C8 for triangulation calculations
* Custom driver board
 
Software:
* C# implementation of sending BeamNG MotionSim data to a serial port, with a UI to control the strength etc.
* FPGA dev, find the equations for caluclating triangle based on the input data
 
Hardware:
* dumping the air compressor idea because of the major lack of portability.
* HFT corded drills attached to a rack and pinion system, or belt drive.
 
Calculations:
* lengths of actuators for a equalateral triangle seat base of 1m sides
  * tan(20) = ~14", I will make it 24"
  * 3" air cylinder diameter

