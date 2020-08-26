//#define ANGLE_DEG_PER_STEP 0.75   //480 steps per 360 degree turn, 1:8 drill gearbox
#define ENCODER1_PIN 2          //must support external interrupt
#define ENCODER2_PIN 3          // ""    ""       ""       ""
int Steps = 0;
byte CurrentStep;

/*
   EncoderDriverInit
   initilization(duh)
*/
void EncoderDriverInit() {
  Calibrate();
  pinMode(ENCODER1_PIN, INPUT);
  pinMode(ENCODER2_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER1_PIN), UpdateSteps, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_PIN), UpdateSteps, CHANGE);
}

/*
    UpdateSteps
    Updates CurrentStep with the current num steps from rotary encoder
    !!!Call on change only!!!
    Order:
      {Encoder1, Encoder2}
        00
        01
        11
        10
        00
        repeat
*/
void UpdateSteps() {
  byte dataIn;
  bitWrite(dataIn, 0, digitalRead(ENCODER1_PIN));
  bitWrite(dataIn, 1, digitalRead(ENCODER1_PIN));
  CurrentStep = CurrentStep & B00000011;             //bit mask
  switch (CurrentStep) {
    case B00:
      switch (dataIn) {
        case B10:
          Steps++;
          break;
        case B01:
          Steps--;
          break;
      }
      break;
    case B01:
      switch (dataIn) {
        case B00:
          Steps++;
          break;
        case B11:
          Steps--;
          break;
      }
      break;
    case B11:
      switch (dataIn) {
        case B01:
          Steps++;
          break;
        case B10:
          Steps--;
          break;
      }
      break;
    case B10:
      switch (dataIn) {
        case B11:
          Steps++;
          break;
        case B00:
          Steps--;
          break;
      }
      break;
  }
  CurrentStep = dataIn;
}

/*
   Calibrate
   Sets current steering wheel position as angle 0
*/
void Calibrate() {
  Steps = 0;
}

/*
   CheckMaxAngle
   returns: 0 if not max angle
   positive is past right full lock
   negative is past left full lock
*/
int CheckMaxAngle() {
  int pastAmount = abs(Steps) - MAX_STEPS;
  if (pastAmount >= 0 && Steps > 0) {     //past right full lock
    //no need to do anything, as steps is positive
  } else if (pastAmount >= 0 && Steps < 0) {
    pastAmount = -pastAmount;
  } else {                                //return 0 if all the checks pass, not past full lock
    pastAmount = 0;
  }
  return pastAmount;
}
