#ifndef System_h
#define System_h

#define SYS_UPDATE_INTERVAL 1
#define VUSB_AVG_READINGS 10
#define VOLTAGE_OFFSET -0.08

#include <Arduino.h>

class System
{
  public:
    System(uint8_t vbat_pin, uint8_t vusb_pin, uint8_t usb_connected_pin, uint8_t gps_power_pin);
    void update();
    void gpsPower(bool on);
    float vbat;
    float vusb;
    float vusb_init;
    bool usb_power;
    bool booted;
  private:
    float _read_vusb(int pin);
    float _to_voltage(int analog_reading);
    static void _setUsbPower();
    uint32_t _update_timer;

    int _readings[VUSB_AVG_READINGS];      // the readings from the analog input
    int _readIndex = 0;              // the index of the current reading
    int _total = 0;                  // the running total
    int _average = 0;

    int _vbat_pin;
    int _vusb_pin;
    int _usb_connected_pin;
    int _gps_power_pin;
};

#endif

