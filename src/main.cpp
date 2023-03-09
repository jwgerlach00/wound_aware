#include <Arduino.h>
#include <EEPROM.h>
#include <helpers.h>


unsigned long startTime = millis();
unsigned long loopStartTime;
unsigned long currentTime;

int pwrLEDPin = PD6;
int tempLEDPin = PD7;
int moistLEDPin = PB0;
int timeLEDPin = PB1;
int buzzPin = PB2;

int periWoundTempPin = PC0;
int woundTempPin = PC1;
int impedancePin1 = PC2;
int impedancePin2 = PC3;

int pwrAddress = 0;
int impedanceAddress = 1;
int tempAddress = 2;

int impedanceThres = 50000; // kOhms
float tempThres = 1.5; // degrees C

int impedanceWindowSize = 12; // 60mins / 5mins
int tempWindowSize = 12; // 60mins / 5mins


void setup() {
  if (EEPROM.read(pwrAddress) == 0) { // If turned on previously; default EEPROM is 255
    exit(0); // Break loop
  }

  // Connect pins to LED indicators
  pinMode(pwrLEDPin, OUTPUT);
  pinMode(tempLEDPin, OUTPUT);
  pinMode(moistLEDPin, OUTPUT);
  pinMode(timeLEDPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  pinMode(periWoundTempPin, INPUT);
  pinMode(woundTempPin, INPUT);
  pinMode(impedancePin1, INPUT);
  pinMode(impedancePin2, INPUT);

  digitalWrite(pwrLEDPin, HIGH); // Turn on power indicator light
  EEPROM.write(pwrAddress, 0); // Mark as turned on

  loopStartTime = millis();
}

void check72Hour() {
  /*
    Check if device on time has reached 72 hour limit.
    If so, turn on indicators
  */
  if(currentTime >= (startTime + seventyTwoHours) ) {
    digitalWrite(timeLEDPin, HIGH);
    digitalWrite(buzzPin, HIGH);
  }
}

void loop() {
  currentTime = millis();
  if (currentTime >= (loopStartTime + fiveMins)) { // Only enter if its been 5 mins since last run
    loopStartTime = millis(); // Reset
    measuredValueLogic(calcImpedance(analogRead(impedancePin2), analogRead(impedancePin1)), impedanceThres,
      impedanceWindowSize, impedanceAddress, moistLEDPin, buzzPin);
    measuredValueLogic(calcTempDifferential(analogRead(periWoundTempPin), analogRead(woundTempPin)),
      tempThres, tempWindowSize, tempAddress, tempLEDPin, buzzPin);
    check72Hour();
  }
}
