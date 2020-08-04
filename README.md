# Fullmotion-Racing-Sim
This is a dream project of mine for a while. Until I can price out a spec sheet that is under $300, I won't start building any hardware.
## Goals
* 4 axis
  * pitch (forward backwards)
  * roll (left right)
  * rotation?   
  * bumps (up/down)
  
* At least 20 degrees of motion 
  * +/- 10 degrees, modeled by D series pickup in BeamNG
   
* Somewhat accurate bump simulation
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
