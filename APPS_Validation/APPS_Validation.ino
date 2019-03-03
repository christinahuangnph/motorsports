#include <TinyWire.h>

/*  Christina Huang
    ATtiny85 APPS Validation
    Transmits to Beaglebone via I2C
*/

/*  Pin Configurations
    1 - Reset (PB5) Unused                     8 - VCC +
    2 - Analog Input 3 (PB3) APPS input 1      7 - Analog Input 1, SCK (PB2) SCL
    3 - Analog Input 2 (PB4) APPS input 2      6 - PWM, MISO (PB1) Unused
    4 - GND                                    5 - PWM, MOSI, AREF (PB0) SDA
*/

/* I2C Configurations
 * 0x03 - Good/Bad 00-Bad, 01-Good
 * 0x04 - Pedal Value Hi
 * 0x05 - Pedal Value Lo
 * 
 * Connect to i2c-2 on Beaglebone
 */


// define I2C address
byte I2C_OWN_ADDR  = 10;

// define input pins
int inputHi = 3;
int inputLo = 4;
int SDA = 0;
int SCL = 2;
int LED = 0;

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
  //declare output pins as outputs and set pullups
  TinyWire.begin(I2C_OWN_ADDR);
  TinyWire.onRequest(I2C_ISR);

  // test
  pinMode(LED, OUTPUT);
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

void I2C_ISR(byte val) {
  TinyWire.send(val);
}

void loop() {

  // read in pedal values
  float sensorValHi = analogRead(inputHi); //read in raw val
  float sensorValLo = analogRead(inputLo); //read in raw val

  // convert to percents
  byte percentHi = (sensorValHi - hi_min)/(hi_max - hi_min); //between 0 and 100
  byte percentLo = (sensorValLo - lo_min)/(lo_max - lo_min) + 101; //between 101-200

  byte val = percentHi;
  val = percentLo;

  // validate percentages
  boolean good = valid(percentHi - percentLo);
  if (!good) {
      loopCounter++;
      if(loopCounter == 10) { //it's been false for 90 ms
        //send error message
        val = 255; //bad
      }
    } else {
      loopCounter = 0;
      val = 254; //good
    }
}

