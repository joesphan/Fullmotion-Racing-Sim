//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 0, 0,
                   true, true, true,
                   false, false, false,
                   false, false, false, false, false);
Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};
int32_t friction[2] = {0};

void JoystickInit() {
  Joystick.setXAxisRange(-MAX_STEPS, MAX_STEPS);
  mygains[0].totalGain = 100;//0-100
  Joystick.setGains(mygains);
  Joystick.begin();
  myeffectparams[0].springMaxPosition = MAX_FULL_STEPS;
}
void JoystickRun() {
  myeffectparams[0].springPosition = Steps;
  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  Joystick.setXAxis(Steps);
}
