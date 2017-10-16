#include "Gps.h"
#include "System.h"
#include "Logger.h"
#include "SDHelper.h"

#include <SdFat.h>
SdFat SD;

// config
#define DEBUG true
#define LOGNOMOVE true

#define GPSSerial Serial1

Adafruit_GPS ada_gps(&GPSSerial);
Gps gps(&ada_gps);

int vbatPin = A7;
int vusbPin = A0;
int usbConnectedPin = A1;
System sys(vbatPin, vusbPin, usbConnectedPin);

Logger logger(&gps);

void setup()
{
  #if DEBUG
    delay(500);
    Serial.begin(115200);
    Serial.println("Debug");
  #endif

  // TODO: connect the battery

  // start up gps
  gps.begin();

  // init card on ss pin 4
  SDHelper.begin(4);
 
  // start the logger to create the trip
  logger.begin();
}

uint32_t timer = millis();
uint32_t secToFix;

void loop()
{
  gps.update();
  sys.update();

  if (!sys.booted) return;

  // log system information
  logger.log(sys);

  if((gps.move() || LOGNOMOVE) && sys.usb_power) {
    logger.log(gps);
  }

  if (logger.recording && !sys.usb_power) {
    Serial.println("usb power lost. Should wait 5 sec.");
    //delay(5000);
    if (!sys.usb_power) {
      logger.end();     
    }
  }

  if (!logger.recording && sys.usb_power) {
    Serial.println("usb power back on. Should wait 2 sec.");
   // delay(2000);
    if (sys.usb_power) {
      //start new trip
      logger.begin();     
    }
  }

  #if DEBUG
    debug();
  #endif
}

void debug() {
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 1 second or so, print out the current stats
  if (millis() - timer > 1000) { 
    timer = millis(); // reset the timer

    // DEBUG:
    if(gps.fix && !secToFix) {
      secToFix = millis();
      Serial.print("Fix after ");
      Serial.println(secToFix);
    
    }
//    gps.print_debug();
//    Serial.print("VBat: ");Serial.println(sys.vbat);
//    Serial.print("VUsb: ");Serial.println(sys.vusb);

    digitalWrite(LED_BUILTIN, sys.usb_power);
  }
}


//void setup() {
//  // init i2c
//  // init gps
//  // init sd
//  // pull operation pin high (for the master to know if slave is ready)
//
//  usb.onConnect = handleUsbConnect;
//  usb.onDisconnect = handleUsbDisconnect;
//}
//
//void loop() {
//  master.commandQueue();
//  if (usb.disconnect()) {
//    master.request("transfer_trip");
//  }dev
//  else {
//    if(gps.fix && gps.move) {
//      if(!sd.isRecording()) {
//        sd.tripStart(gps.dateTime);
//      }
//      else {
//        sd.tripLog(gps.data);
//      }
//    }   
//  }
//}
//
//void handleUsbConnect() {
//   master.request("slave_usb_connect");
//}
//
//void handleUsbDisconnect() {
//  master.request("slave_usb_disconnect");
//  sd.tripEnd();
//  gps.off();
//}
//
//void wakeUpCallback() {
//  gps.on();
//  vbat = battery.getVoltage();
//  if (vbat < battery.cutoff) {
//    system.powerdown(); // set pin high/low to make transistor cut power completly 
//  }
//  while(!gps.dateTime);
//  sd.syslog(gps.dateTime, "vbat", vbat);
//  slave.command("sleep");
//}

