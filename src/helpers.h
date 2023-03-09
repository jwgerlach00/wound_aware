#include <Arduino.h>
#include <EEPROM.h>


int ADCBits = 1023; // 10-bit ADC
int RImpedance = 220; // 220 Ohms

unsigned int fiveMins = 300000; // 5*60*1000
unsigned int seventyTwoHours = 259200000; // 72*60*60*1000;


float tempConversion(float tempAnalog) {
  return map(tempAnalog, 0, ADCBits, -40, 150);
}

float calcTempDifferential(int periWoundRead, int woundRead) {
  return tempConversion(woundRead) - tempConversion(periWoundRead);
}

float calcImpedance(int impedanceReadNum, int impedanceReadDenom) {
  /*
    Ratio max of 1024
    1024*220 = 225280 which is greater than impedanceThres
  */
  return float((impedanceReadNum + 1) / (impedanceReadDenom + 1)) * RImpedance;
}

void measuredValueLogic(int value, int threshold, int windowSize, int address, int LEDPin, int buzzPin) {
  if (value >= threshold) {
    if (EEPROM.read(address) == 255) { // If first measurement (empty EEPROM)
      EEPROM.write(address, 1);
    } else { // Increment value count
      EEPROM.write(address, EEPROM.read(address) + 1);
    }
    if (EEPROM.read(address) >= windowSize) { // If count >= windowSize
      // Turn on temperature indicator light and audio signal
      digitalWrite(LEDPin, HIGH);
      digitalWrite(buzzPin, HIGH);
    }
  } else { // If !> threshold for one datapoint, clear temperature count
    EEPROM.write(address, 0);
  }
}
