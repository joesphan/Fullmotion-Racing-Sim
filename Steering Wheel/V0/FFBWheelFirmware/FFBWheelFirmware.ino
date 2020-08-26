#include "Joystick.h"
#define MAX_STEPS 720
#define MAX_FULL_STEPS (MAX_STEPS * 2)



//extern vars
extern int Steps;
extern int32_t forces[2];

void setup() {
  Serial.begin(9600);
//inits
  JoystickInit();
  BridgeDriverInit();
  EncoderDriverInit();
}

void loop() {
  SendSerial(Steps, forces[0]);
  JoystickRun();
  BridgeDriver(forces[0]);
}
