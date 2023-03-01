#include <Arduino.h>
#include <EEPROM.h>

unsigned long startTime;
unsigned long loopStartTime;

void setup() {
  // Put your setup code here, to run once:
  // Wait for power button to be pressed
  // Sample the start time (for the 72 hour limit)
  startTime = millis();

  // Connect pins to LEDs
  pinMode( PD6, OUTPUT); // Power indicator light
  pinMode( PD7, OUTPUT); // Temperature indicator light
  pinMode( PB0, OUTPUT); // Moisture indicator light
  pinMode( PB1, OUTPUT); // Time limit indicator light
  pinMode( PB2, OUTPUT); // Buzzer for auditory indication

  while (EEPROM.read(1) == 1) { //This means it was turned off previously
    // Don't let anything turn on
    // Infinite loop
  }
  
  // Turn on power indicator light
  digitalWrite(<PowerLEDNumber>, HIGH);
}

void loop() {
  // Sample start time at beginning (for 5 minute sampling loop)
  loopStartTime = millis();
  measure_impedance();
  measure_temperature();
  check_72_hour();
  
  // Loop here while waiting for 5 minutes to sample again
  unsigned long currentTime = millis();
  while (currentTime < (loopStartTime + 300000)) {
    // If power button has been pressed
    if (digitalRead(powerButtonPin) == LOW) {
      powerOffDevice();
    }
    currentTime = millis();
  }
}

void measure_impedance() {
  // TODO: FIX ME Measure impedance
  
  // If impedance is greater than 50kOhms
  if (impedance > 50000) {
    if (EEPROM.read(1) == 255) { // This means this is the first measurement
      EEPROM.write(1, 1);
    } else { // Increment impedance count
      EEPROM.write(1, EEPROM.read(1)+1);
    }
    // If impedance count is greater than or equal to 12
    if (EEPROM.read(1) >= 12) {
      // Turn on moisture indicator light and audio signal
      digitalWrite( PB0 , HIGH);
      digitalWrite( PB2 , HIGH);
    }
  }
  else { // Clear impedance counts if impedance within proper range
    EEPROM.write(1, 0);
  }
}

void measure_temperature() {
  // TODO: FIX ME Measure temperature
      // Measure periwound temperature
      // Measure wound temperature
      // Calculate temperature difference
  // If differential measurement increased by greater than 
  // or equal to 1.5C
  if (differential >= 1.5) {
    // Increment temperature count
    if (EEPROM.read(2) == 255) { // This means this is the first measurement
      EEPROM.write(2, 1);
    } else { // Increment temperature count
      EEPROM.write(2, EEPROM.read(2)+1);
    }
    // If temperature count is greater than or equal to 12
    // Turn on temperature indicator light and audio signal
    if (EEPROM.read(2) >= 12) {
      // Turn on temperature indicator light and audio signal
      digitalWrite( PD7 , HIGH);
      digitalWrite( PB2 , HIGH);
    }
  } else { // Clear temperature count
    EEPROM.write(2, 0);
  }
}

void check_72_hour() {
  // Check if device on time has reached 72 hour limit
  if( millis() >= (startTime + 259200000) ) {
    // Turn on change dressing indicator light and audio signal
    digitalWrite( PB1, HIGH);
    digitalWrite( PB2 , HIGH);
  }
}

void powerOffDevice() {
  EEPROM.write(0, 1);
  // Turn off buzzers and LED lights
  digitalWrite( PD6, LOW); // Power indicator light
  digitalWrite( PB0, LOW); // Moisture indicator light
  digitalWrite( PD7, LOW); // Temperature indicator light
  digitalWrite( PB1, LOW); // Time limit indicator light
  digitalWrite( PB2 , LOW); // Buzzer for auditory indication
  // TODO: FIX ME, maybe a more efficient way to turn off...
  while (EEPROM.read(1) == 1) {
    // Don't let anything turn on
    // Infinite loop
  }
}