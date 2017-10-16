// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

// SCL - analog pin 5
// SDA - analog pin 4

void setup() {
    Wire.begin(4);                // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event
    Wire.onRequest(requestEvent); // register event
    Serial.begin(9600);           // start serial for input and output
    Serial.println("I2C SLAVE 5V");
    delay(2000); // the slave should become ready first
    Serial.println("Ready");
}

void loop() {
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while(Wire.available())
    {
      char c = Wire.read();
      Serial.print(c);
    }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
    if (Serial.available()) {
        char inChar = '0'; 
        inChar = (char)Serial.read(); 
        Wire.write(inChar);
    } else { //send some dummy data if no data is available from serial
        Wire.write("?");
    }
}
