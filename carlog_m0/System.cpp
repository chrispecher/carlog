#include "System.h"

System::System(uint8_t vbat_pin, uint8_t vusb_pin, uint8_t usb_connected_pin, uint8_t gps_power_pin) {
  booted = false;
  
  _vbat_pin = vbat_pin;
  _vusb_pin = vusb_pin;
  _usb_connected_pin = usb_connected_pin;
  _gps_power_pin = gps_power_pin;
  
  for (int thisReading = 0; thisReading < VUSB_AVG_READINGS; thisReading++) {
    _readings[thisReading] = 0;
  }
  
  //init timers
  _update_timer = millis();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(_vbat_pin, INPUT);
  pinMode(_vusb_pin, INPUT);
  pinMode(_usb_connected_pin, INPUT);
  pinMode(_gps_power_pin, OUTPUT);



  //attachInterrupt(digitalPinToInterrupt(_usb_connected_pin), _setUsbPower, CHANGE);
}

void System::update()
{
  // if millis() or timer wraps around, we'll just reset it
  if (_update_timer > millis())  _update_timer = millis();
  // approximately every UPDATE_INTERVAL seconds or so, print out the current stats
  if (millis() - _update_timer < SYS_UPDATE_INTERVAL) {
    return; 
  }
  _update_timer = millis();

  //after 5 sec, we consider the system stable and change status to booted
  if (millis() > 5000 && !booted) {
    booted = true;
    vusb_init = vusb; //store the initial vusb for later comparison
  }

  int analog_vbat = analogRead(_vbat_pin);
  vbat = _to_voltage(analog_vbat);
  
  int analog_vusb = _read_vusb(_vusb_pin);
  vusb = _to_voltage(analog_vusb);
  usb_power = digitalRead(_usb_connected_pin);
}

void System::gpsPower(bool on) {
  digitalWrite(_gps_power_pin, on);
}

float System::_read_vusb(int pin) {
  //SMOOTHING:
  // subtract the last reading:
  _total = _total - _readings[_readIndex];
  // read from the sensor:
  _readings[_readIndex] = analogRead(pin);
  // add the reading to the total:
  _total = _total + _readings[_readIndex];
  // advance to the next position in the array:
  _readIndex = _readIndex + 1;

  // if we're at the end of the array...
  if (_readIndex >= VUSB_AVG_READINGS) {
    // ...wrap around to the beginning:
    _readIndex = 0;
  }

  // calculate the average:
  _average = _total / VUSB_AVG_READINGS;
  // send it to the computer as ASCII digits
  return _average;
}

float System::_to_voltage(int analog_reading) {
    float measuredvbat = analog_reading;
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat + VOLTAGE_OFFSET;
}

void System::_setUsbPower () {
  Serial.println("set usbpower");
}

