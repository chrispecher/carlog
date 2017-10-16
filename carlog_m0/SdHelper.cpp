#include "SdHelper.h"
#include "SdFat.h"
extern SdFat SD;

void SDHelperClass::begin(int pin){
  // init SD
  if (!SD.begin(4)) {
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
  
}

SDHelperClass SDHelper;
