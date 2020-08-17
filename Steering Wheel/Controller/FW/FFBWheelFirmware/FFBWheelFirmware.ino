#include "Joystick.h"

//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 0, 0, true, true, true, false, false, false, false, false, false, false, false);
Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};
int32_t friction[2] = {0};

void setup() {
  pinMode(A2, INPUT);
  Joystick.setXAxisRange(-512, 512);
  mygains[0].totalGain = 100;//0-100
  //mygains[0].springGain = 100;//0-100
  Joystick.setGains(mygains);
  Joystick.begin();
  myeffectparams[0].springMaxPosition = 512;
}

void loop() {
  int val = analogRead(A2) - 512;
  myeffectparams[0].springPosition = steeringPos; //-512-512
  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  BridgeDriver(forces[0]);
  Joystick.setXAxis(val);
}
