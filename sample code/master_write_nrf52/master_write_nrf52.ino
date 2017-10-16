/*
 * Copyright (c) 2016 RedBear
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
/*    nRF5x                       UNO
*Example:
*     master_write              slave_receiver
*Pin:
*     SCL                         SCL
*     SDA                         SDA
*address:
*     0x04                        0x04
*/

#include <Wire.h>
// SCL - analog pin 5
// SDA - analog pin 4

void setup() {    
  Wire.begin(); //join I2C as master
  Serial.begin(9600); //setup serial for input and output
  Serial.println("I2C MASTER 3.3V");
  delay(5000); // the master should be ready after the slave
  Serial.println("Ready");
}

void loop() {
  if (Serial.available()) {
    char inChar = (char)Serial.read(); 
    Wire.beginTransmission(4);
    Wire.write(inChar);
    Wire.endTransmission();
  } 

  Wire.requestFrom(4, 1); // request 1 byte from slave device #4
  while(!Wire.available());
  char d = Wire.read();
  // ignore special character ?
  if (d != '?') Serial.print(d);
}

