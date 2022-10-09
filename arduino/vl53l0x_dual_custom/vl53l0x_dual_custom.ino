#include "Adafruit_VL53L0X.h"
#include "config.h"

// address we will assign if dual sensor is present
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

double volume = 0.0f;
float prev_scans[SENSORS_NUM];
float new_scans[SENSORS_NUM];

uint16_t r1 = 0;
uint16_t r2 = 0;

uint16_t r1_prev = 0;
uint16_t r2_prev = 0;

void accumulate_volume(float* prev_scans, float* new_scans) {
  // проверь функцию + оптимизируй на указатели/память/переменные и прочее

  for (int i = 0; i < SENSORS_NUM - 1; i++) {
    float r1_o = prev_scans[i];
    float r1_d = new_scans[i];
    float r2_o = prev_scans[i + 1];
    float r2_d = new_scans[i + 1];

    volume += (
        2 * r1_o * r1_o +
        2 * r1_d * r1_d +
        2 * r2_o * r2_o +
        2 * r2_d * r2_d +
        2 * r1_o * r2_o +
        2 * r1_o * r1_d +
        2 * r1_d * r2_d +
        2 * r2_o * r2_d +
        r1_o * r2_d +
        r1_d * r2_o
      );
  }
  float r1_o = prev_scans[SENSORS_NUM - 1];
  float r1_d = new_scans[SENSORS_NUM - 1];
  float r2_o = prev_scans[0];
  float r2_d = new_scans[0];

  volume += (
    2 * r1_o * r1_o +
    2 * r1_d * r1_d +
    2 * r2_o * r2_o +
    2 * r2_d * r2_d +
    2 * r1_o * r2_o +
    2 * r1_o * r1_d +
    2 * r1_d * r2_d +
    2 * r2_o * r2_d +
    r1_o * r2_d +
    r1_d * r2_o
  );

}

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
  lox1.setMeasurementTimingBudgetMicroSeconds(1);
  lox1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED);
  lox1.startRangeContinuous(1);
  delay(100);
  lox2.setMeasurementTimingBudgetMicroSeconds(1);
  lox2.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED);
  lox2.startRangeContinuous(1);
  delay(100);

  memset(&prev_scans[0], 0, SENSORS_NUM);
  memset(&new_scans[0], 0, SENSORS_NUM);

  for (int i = 0; i < 100; i++) {
    r1 = lox1.readRangeResult();
    r2 = lox2.readRangeResult();
    // ....
    // r24 = lox24.readRange();
    memcpy(&prev_scans[0], &new_scans[0], SENSORS_NUM); // prev = new

    new_scans[0] = RING_RADIUS - ((float)r1)/1000;
    new_scans[1] = RING_RADIUS - ((float)r2)/1000;

    accumulate_volume(&prev_scans[0], &new_scans[0]);
  }
  Serial.println(volume  * SCAN_LENGTH * ALPHA / 36);

}

void loop() {

}