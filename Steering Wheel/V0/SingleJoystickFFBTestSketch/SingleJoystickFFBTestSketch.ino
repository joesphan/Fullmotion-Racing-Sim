#include "Joystick.h"

//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 0, 0,
                   true, false, false, //X,Y,Z
                   true, false, false,//Rx,Ry,Rz
                   false, false, false, false, false);

Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  //throttle
  Joystick.setRxAxisRange(0, 1023);
  //Steering wheel
  Joystick.setXAxisRange(-512, 512);
  //set X Axis gains
  mygains[0].totalGain = 100;//0-100
  mygains[0].springGain = 100;
  //enable gains REQUIRED
  Joystick.setGains(mygains);
  Joystick.begin();
  myeffectparams[0].springMaxPosition = 512;
}

void loop() {
  int value = analogRead(A2);
  Joystick.setRxAxis(analogRead(A1));
  //set X Axis Spring Effect Param
  //Steering wheel
  myeffectparams[0].springPosition = value - 512; //-512-512

  //Recv HID-PID data from PC and caculate forces
  //Steering wheel
  Joystick.setXAxis(value - 512);
  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  Serial.println(forces[0]);
  delay(1);
}
