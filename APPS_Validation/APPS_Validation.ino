/*  Christina Huang
 *  ATtiny85 APPS Validation
 *  Transmits to Beaglebone via I2C
 *  Updated 1/12/2019
 */

 /*  Pin Configurations
  *  1 - Reset (PB5)              8 - VCC +
  *  2 - Analog Input 3 (PB3)     7 - Analog Input 1, SCK (PB2)
  *  3 - Analog Input 2 (PB4)     6 - PWM, MISO (PB1)
  *  4 - GND                      5 - PWM, MOSI, AREF (PB0)
  *  
  *  Connect to i2c-0 on Beaglebone: 0x44E0_B000
  */
  
  #include <"TinyWireS.h"> //I2C ATtiny Library
  #define I2C_SLAVE_ADDR  0x00 //I2C ATtiny Address - REPLACE
  
 void setup() {
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
