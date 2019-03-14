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
int inputHi = 3; //HI
int inputLo = 4; //LOW
int SDA = 0;
int SCL = 2;

// compare values
float hi_max = 9.1; //4.4V
float hi_min = 3.4; //1.6V

float lo_max = 7.8; //3.6V
float lo_min = 1.7; //0.8V

// global variables
boolean good = true;
int loopCounter = 0;
int val;

void setup() {
  // declare input pins as inputs and set pullups
  pinMode(inputHi, INPUT);
  digitalWrite(inputHi, HIGH);
  pinMode(inputLo, INPUT);
  digitalWrite(inputLo, HIGH);
  //declare output pins as outputs and set pullups
  TinyWire.begin(I2C_OWN_ADDR);
  TinyWire.onRequest(I2C_ISR);
}

/* Check whether the pedal value difference is valid
*/
boolean valid(float val) {
  return (val <= 40 && val >= 12);
}

void I2C_ISR() {
  // read in pedal values
  float sensorValHi = analogRead(inputHi)/100.0; //read in raw val/255*10
  //float sensorValLo = analogRead(inputLo); //read in raw val

  // convert to percents
  sensorValHi = (sensorValHi - hi_min + 20)/77.0*100.0; //between 0 and 100

  //sensorValLo = (sensorValLo - lo_min)/(61.0)*100.0; //between 101-201
  val = (sensorValHi - 23)*10 - 5;
  TinyWire.send(val);
    //val = sensorValLo;
    //TinyWire.send(val);
}

void loop() {
}

