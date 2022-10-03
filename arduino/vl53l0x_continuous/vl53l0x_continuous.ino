#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#define sensor A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin(0x31)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

  // start continuous ranging
  lox.startRangeContinuous();
  Serial.println("---");

  int i = 0;
  while (i < 100)
  {
    if (lox.isRangeComplete()) {
      Serial.print("VLX ");
      Serial.print(0.9857497f * (float)lox.readRange() - 23.819183f);
      // 5v
      float volts = analogRead(sensor) * 0.0048828125; // value from sensor * (5/1024)
      float distance = 130.f * pow(volts, -1); // worked out from datasheet graph
      delay(100); // slow down serial port
      Serial.print("      IR ");
      Serial.println(distance);   // print the distance

      i++;
    }
  }
}

void loop() {
  // gotovo (see setup block)
}
