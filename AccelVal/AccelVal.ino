#include <TinyWire.h>

/*  Christina Huang
    ATtiny85 Accelerator Pedal Validation
    Transmits to Beaglebone via I2C
*/

/*  Pin Configurations
    1 - Reset (PB5) Unused                     8 - VCC +
    2 - Analog Input 3 (PB3) APPS input 1      7 - Analog Input 1, SCK (PB2) SCL
    3 - Analog Input 2 (PB4) APPS input 2      6 - PWM, MISO (PB1) Unused
    4 - GND                                    5 - PWM, MOSI, AREF (PB0) SDA
*/

/* I2C Configurations 
 * Connect to i2c-2 on Beaglebone
 */


// define I2C address
byte I2C_SLAVE_ADDR  = 10;

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

  // configure I2C 
  TinyWire.begin();

  // set up test LED
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

void transmit(byte val){
      // begin a master transmission
    TinyWire.beginTransmission(I2C_SLAVE_ADDR);
    TinyWire.send(val); //fill buffer
    if(TinyWire.endTransmission() != 0){ //send buffer
      // turn LED on, to indicate an error
      digitalWrite(LED, HIGH);
    }
}

void loop() {
  // read in pedal values
  float sensorValHi = analogRead(inputHi); //read in raw val
  float sensorValLo = analogRead(inputLo); //read in raw val

  // convert to percents
  byte percentHi = (sensorValHi - hi_min)/(hi_max - hi_min); //between 0 and 100
  transmit(percentHi);
  byte percentLo = (sensorValLo - lo_min)/(lo_max - lo_min) + 101; //between 101-200
  transmit(percentLo);

  // validate percentages
  boolean good = valid(percentHi - percentLo);
  if (!good) {
      loopCounter++;
      if(loopCounter == 10) { //it's been false for 90 ms
        //send error message
        transmit(255); //bad
      }
    } else {
      loopCounter = 0;
      transmit(254); //good
    }

  delay(100);
}
