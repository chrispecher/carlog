#include "SDHelper.h"
#include "SdFat.h"
extern SdFat SD;

void SDHelperClass::begin(int pin){
  // init SD
  _pin = pin;
  if (!SD.begin(pin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void SDHelperClass::dumpFile(char* path){
  File dataFile = SD.open(path);

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening "); Serial.println(path);
  }
}

void SDHelperClass::wipeCard() {
  int c;
 
  Serial.println("Type 'Y' to wipe all data.");
  while (!Serial.available()) {
    SysCall::yield();
  }
  c = Serial.read();
  if (c != 'Y') {
    Serial.println("Quitting, you did not type 'Y'.");
    return;
  }
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur. 
  if (!SD.begin(_pin, SD_SCK_MHZ(50))) {
    Serial.println("SD init before wipe failed");
  }
  // Use wipe() for no dot progress indicator.
  if (!SD.wipe(&Serial)) {
    Serial.println("Wipe failed.");
  }
  // Must reinitialize after wipe.
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.  
  if (!SD.begin(_pin, SD_SCK_MHZ(50))) {
    Serial.println("SD second init failed.");
  }
  Serial.println("Done");
}

bool SDHelperClass::deleteFile(char* path) {
  return SD.remove(path);
}

SDHelperClass SDHelper;
