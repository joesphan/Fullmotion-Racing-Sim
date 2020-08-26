#define ENDSTOP_DAMPENING 100      //dampening factor when turned past full lock
#define FFB_MULTIPLIER 0.35       //required multiplier to get it into 2 * 255 domain
#define HBridgePinR 5             //HBridge pin that drives everything to right
#define HBridgePinL 6             //   ""    ""  ""   ""        ""     "" left
/*
 * BridgeDriverInit
 * initializes the driver
 */
void BridgeDriverInit() {
  pinMode(HBridgePinR, OUTPUT);
  pinMode(HBridgePinL, OUTPUT);

}
/*
   BridgeDriver
   inputs: value, negative/positive left/right respectively
   processes data and checks for endstops
*/
void BridgeDriver(int val) {
  int PastAmount = CheckMaxAngle();
  if (CheckMaxAngle() == 0)    // if max angle is reached, brake
  {
    HBridgeInterface((int)(float)val * FFB_MULTIPLIER);
  }
  else                       //past full lock, left or right
  {
    HBridgeInterface(PastAmount * ENDSTOP_DAMPENING);
  }
}

/*
 * !!!DANGEROPS FUNCTION, EXPLODEYBOI MOSFETS!!!
 * HBridgeInterface
 * input: +/-value (int)
 * Turns off both before it writes anything else for safety reasons
 */
void HBridgeInterface(int val) {
  analogWrite(HBridgePinR, 0);
  analogWrite(HBridgePinL, 0);
  if (val > 0) {            //right
    analogWrite(HBridgePinR, val);
  } else if (val < 0) {     //left
    analogWrite(HBridgePinL, abs(val));
  }
}
