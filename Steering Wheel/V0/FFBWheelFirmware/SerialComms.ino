/*
 * sends MCU data over to serial, for debug or other purposes
 */
void SendSerial(int Steps, int FFBInput) {
  if (Serial.available() > 0) {
      Serial.println("Angle in steps: ");
      Serial.print(Steps);
      Serial.print("  Motor Force: ");
      Serial.print(FFBInput);
  }
}
