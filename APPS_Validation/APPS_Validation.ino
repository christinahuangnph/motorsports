/*  Christina Huang
    ATtiny85 APPS Validation
    Transmits to Beaglebone via I2C
*/

/*  Pin Configurations
    1 - Reset (PB5)                            8 - VCC +
    2 - Analog Input 3 (PB3) APPS input 1      7 - Analog Input 1, SCK (PB2)
    3 - Analog Input 2 (PB4) APPS input 2      6 - PWM, MISO (PB1)
    4 - GND                                    5 - PWM, MOSI, AREF (PB0)

    Connect to i2c-0 on Beaglebone: 0x44E0_B000
*/

#include <"TinyWireS.h"> //I2C ATtiny Library
#include <avr/interrupt.h> //Interrupt Handler Library
#include <avr/io.h>
#include "TimerOne.h"

#define I2C_MASTER_ADDR  0x00 //Beaglebone Address - REPLACE

// define input pins
int inputHi = 2;
int inputLo = 3;

// compare values
float hi_max = 4.4;
float lo_max = 3.6;
float hi_min = 1.6;
float lo_min = 0.8;

// global variables
boolean good = true;
int loopCounter = 0;

void setup() {
  // declare input pins as inputs and set pullups
  pinMode(inputHi, INPUT);
  digitalWrite(inputHi, HIGH);
  pinMode(inputLo, INPUT);
  digitalWrite(inputLo, HIGH);

  // Set Up I2C
  TinyWireS.begin();  // initialize I2C library

  //SET UP BEAGLEBONE COMMUNICATION STUFF
  TinyWireS.beginTransmission(uint8_t I2C_SLAVE_ADDR); //set up slave
}

void loop() {
  // read in pedal values
  float sensorValHi = analogRead(inputHi); //read in raw val
  float sensorValLo = analogRead(inputLo); //read in raw val

  // convert to percents
  float percentHi = convertToPercent((sensorValHi - hi_min), (hi_max - hi_min));
  float percentLo = convertToPercent((sensorValLo - lo_min), (lo_max - lo_min));

  // validate percentages
  boolean good = valid(percentHi - percentLo);
  if (!good) {
      loopCounter++;
      if(loopCounter == 10) { //it's been false for 90 ms
        //send error message
        Serial.print("Bad banana"); //for testing purposes
      }
    } else {
      loopCounter = 0;
    }
  }
  
  // relay pedal values to Beaglebone - FINISH THIS
  Serial.print("High: " + sensorValHi + " Low: " + sensorValLo); //for testing purposes
  
  delay(9); // wait 9 ms;
}

/* Convert raw pedal values to percent of total range
*/
float convertToPercent(float val, float compare) {
  return percent = val / compare;
}

/* Check whether the pedal value difference is valid
*/
boolean valid(float val) {
  if (val > 0.88 || val < 0.72) { //outside the 10% range
    return false;
  } else {
    return true;
  }
}

