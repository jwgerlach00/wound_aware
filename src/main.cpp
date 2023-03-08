#include <Arduino.h>
#include <EEPROM.h>

unsigned long startTime;
unsigned long loopStartTime;
unsigned long currentTime;

int pwrLEDPin = PD6;
int tempLEDPin = PD7;
int moistLEDPin = PB0;
int timeLEDPin = PB1;
int buzzPin = PB2;

int pwrAddress = 0;
int impedanceAddress = 1;
int tempAddress = 2;

int fiveMins = 5*60*1000;
int seventyTwoHours = 72*60*60*1000;

int impedanceThres = 50000; // 50kOhms
int impedanceWindowSize = 12; // 60mins / 5mins
int tempWindowSize = 12; // 60mins / 5mins


void setup() {
  if (EEPROM.read(pwrAddress) == 0) { // If turned on previously; default EEPROM is 255
    exit(0); // Break loop
  }

  // Sample the start time (for the 72 hour limit)
  startTime = millis();

  // Connect pins to LED indicators
  pinMode(pwrLEDPin, OUTPUT);
  pinMode(tempLEDPin, OUTPUT);
  pinMode(moistLEDPin, OUTPUT);
  pinMode(timeLEDPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  // if (EEPROM.read(pwrAddress) == 255) { // If turned on previously
  //   powerOffDevice();
  // } else {
  digitalWrite(pwrLEDPin, HIGH); // Turn on power indicator light
  EEPROM.write(pwrAddress, 0); // Mark as turned on
}

void loop() {
  currentTime = millis();
  if (currentTime >= (loopStartTime + fiveMins)) { // Only enter if its been 5 mins since last run
    loopStartTime = millis(); // Reset
    main();
  }
}

void main() {
  measureImpedance();
  measureTemp();
  check72Hour();
}

// void powerOffDevice() {
//   // Turn off buzzers and LED lights
//   digitalWrite(pwrLEDPin, LOW);
//   digitalWrite(tempLEDPin, LOW);
//   digitalWrite(moistLEDPin, LOW);
//   digitalWrite(timeLEDPin, LOW);
//   digitalWrite(buzzPin, LOW);
// }

float calcImpedance() {
  // TODO: FIX ME
}

void measureImpedance() {
  // If impedance is greater than 50kOhms
  if (calcImpedance() > impedanceThres) {
    if (EEPROM.read(impedanceAddress) == 255) { // This means this is the first measurement (empty EEPROM)
      EEPROM.write(impedanceAddress, 1);
    } else { // Increment impedance count
      EEPROM.write(impedanceAddress, EEPROM.read(1) + 1);
    }
    // If impedance count is greater than or equal to 12
    if (EEPROM.read(impedanceAddress) >= impedanceWindowSize) {
      // Turn on moisture indicator light and audio signal
      digitalWrite(moistLEDPin, HIGH);
      digitalWrite(buzzPin, HIGH);
    }
  }
  else { // Clear impedance counts if impedance within proper range
    EEPROM.write(impedanceAddress, 0);
  }
}

float tempConversion(float temp) {
  // TODO: FIX ME
}

float calcTempDifferential() {
  // TODO: FIX ME
  int periwoundTemp = NULL;
  int woundTemp = NULL;
  abs(tempConversion(periwoundTemp - woundTemp));
}

void measureTemp() {
  // TODO: FIX ME Measure temperature
      // Measure periwound temperature
      // Measure wound temperature
      // Calculate temperature difference
  // If differential measurement increased by greater than 
  // or equal to 1.5C

  if (calcTempDifferential() >= 1.5) {
    // Increment temperature count
    if (EEPROM.read(tempAddress) == 255) { // This means this is the first measurement (empty EEPROM)
      EEPROM.write(tempAddress, 1);
    } else { // Increment temperature count
      EEPROM.write(tempAddress, EEPROM.read(tempAddress) + 1);
    }
    // If temperature count is greater than or equal to 12
    // Turn on temperature indicator light and audio signal
    if (EEPROM.read(tempAddress) >= tempWindowSize) {
      // Turn on temperature indicator light and audio signal
      digitalWrite(tempLEDPin, HIGH);
      digitalWrite(buzzPin, HIGH);
    }
  } else { // Clear temperature count
    EEPROM.write(tempAddress, 0);
  }
}

void check72Hour() {
  // Check if device on time has reached 72 hour limit
  if(currentTime >= (startTime + seventyTwoHours) ) {
    while (millis() - currentTime < fiveMins) { // Signal for 5 mins
      digitalWrite(timeLEDPin, HIGH);
      digitalWrite(buzzPin, HIGH);
    }
    turnOff();
  }
}

void turnOff() {
  EEPROM.read(pwrAddress) == 0; // Mark as turned off
  exit(0);
}

