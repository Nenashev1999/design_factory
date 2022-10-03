#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 6
#define SHT_LOX2 7

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */
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

void read_dual_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  //Serial.print(F("1: "));
  if(measure1.RangeStatus != 4) {     // if not out of range
   // Serial.print(measure1.RangeMilliMeter);
  } else {
   // Serial.print(F("Out of range"));
  }
  
  //Serial.print(F(" "));

  // print sensor two reading
 // Serial.print(F("2: "));
  if(measure2.RangeStatus != 4) {
   // Serial.print(measure2.RangeMilliMeter);
  } else {
    //Serial.print(F("Out of range"));
  }
  
  //Serial.println();
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

  //lox1.readRange();
  //lox2.readRange();
}

long long int prev = 0;
long int counter1 = 0;
long int counter2 = 0;
long int counter = 0;

uint16_t r1 = 0;
uint16_t r2 = 0;

uint16_t r1_prev = 0;
uint16_t r2_prev = 0;

void loop() {

  if (millis() - prev > 10000)
  {
    prev = millis();
    Serial.print("  r1  ");
    Serial.print(r1);
    Serial.print("  r2  ");
    Serial.print(r2);
    Serial.print("  c2  ");
    Serial.print(counter1);
    Serial.print("  c1  ");
    Serial.print(counter2);
    Serial.print("    counter ");
    Serial.println(counter);
    counter1 = 0;
    counter2 = 0;
    counter = 0;
  }
  //read_dual_sensors();
  for (int i = 0; i <8; i++)
  {
    r1 = lox1.readRangeResult();
  }
  counter++;

/*
  if (r1 != r1_prev)
  {
      counter1++;
      r1_prev = r1;
  }
  if (r2 != r2_prev)
  {
      counter2++;
      r2_prev = r2;
  }
*/ 

}
