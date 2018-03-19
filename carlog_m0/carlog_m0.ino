#undef max
#undef min
#include "Gps.h"
#include "System.h"
#include "Logger.h"
#include "SDHelper.h"
#include <SimpleTimer.h>
#include <SdFat.h>


SdFat SD;

// config
#define DEBUG true
#define LOGNOMOVE true

#define GPSSerial Serial1

Adafruit_GPS ada_gps(&GPSSerial);
Gps gps(&ada_gps);

uint8_t vbatPin = A7;
uint8_t vusbPin = A0;
uint8_t usbConnectedPin = A1;
uint8_t gpsPowerPin = A2;
uint8_t sysPowerPin = A3;
System sys(vbatPin, vusbPin, usbConnectedPin, gpsPowerPin);

Logger logger(&gps);

SimpleTimer timer;

void setup()
{
  #if DEBUG
    // don't wait longer than 3000 for Serial
    while (!Serial && millis() < 3000) {};
    Serial.begin(115200);
    Serial.println("Debug");
    int setDebugInterval = timer.setInterval(1000, debug);
    Serial.print("setDebugInterval: " + setDebugInterval);
  #endif

  // set sys power
  pinMode(sysPowerPin, OUTPUT);
  digitalWrite(sysPowerPin, 1);
  Serial.println("Sys high");

  // start up gps
  sys.gpsPower(1);
  gps.begin();

  // init card on ss pin 4
  SDHelper.begin(4);
  // option to wipe the card by sending 'Y' on serial
  //SDHelper.wipeCard();
    
  // start the logger to create the trip
  logger.begin();
}

uint32_t debugTimer = millis();
uint32_t secToFix;

int loggerToggleTimeout = 0;
bool recordingNoPower;
bool stoppedButPower;
bool requiresToogle;

void loop()
{
  gps.update();
  sys.update();
  
  // this is where the "polling" occurs
  timer.run();

  if (!sys.booted) return;

  // log system information
  logger.log(sys);

  if((gps.move() || LOGNOMOVE) && sys.usb_power) {
    logger.log(gps);
  }

  // TODO: move to logger class
  recordingNoPower = logger.recording && !sys.usb_power;
  stoppedButPower = !logger.recording && sys.usb_power;
  requiresToogle = recordingNoPower || stoppedButPower;
  // need to to toogle the logger and timeout isn't set yets
  if (requiresToogle && !loggerToggleTimeout) {
    Serial.println("requiresToogle and no loggerToogleTimeout");
    loggerToggleTimeout = timer.setTimeout(3000, [] {
      Serial.println("loggerToggleTimeout");
      if (stoppedButPower) {
        digitalWrite(sysPowerPin, 1);
        Serial.println("Sys high callback");
        sys.gpsPower(1);
        logger.begin();
      }
      else if (recordingNoPower) {
         sys.gpsPower(0);
         logger.end();
         delay(1000);
         digitalWrite(sysPowerPin, 0);
         Serial.println("Sys low");
      }
      loggerToggleTimeout = 0;
    });
  }
}

void debug() {

  // DEBUG:
  if(gps.fix && !secToFix) {
    secToFix = millis();
    Serial.print("Fix after ");
    Serial.println(secToFix);
  
  }
  //gps.print_debug();
    Serial.print("VBat: ");Serial.println(sys.vbat);
    Serial.print("VUsb: ");Serial.println(sys.vusb);

  digitalWrite(LED_BUILTIN, sys.usb_power);

}

