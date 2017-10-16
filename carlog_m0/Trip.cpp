#include "Trip.h"
#include "sdfat.h"
#include "System.h"

extern SdFat SD;

void Trip::create() {
  Serial.println("trip create");

  _createGpsLogFile();
  _createSysLogFile();
}

void Trip::_createGpsLogFile() {
  strcpy(foldername, "TRIP0000/");
  for (uint16_t i = 0; i < 10000; i++) {
    foldername[4] = '0' + i/1000%10;
    foldername[5] = '0' + i/100%10;
    foldername[6] = '0' + i/10%10;
    foldername[7] = '0' + i%10;
    
    if (! SD.exists(foldername)) {
      break;
    }
  }

  Serial.print("foldername: "); Serial.println(foldername);

  // create the folder
  if (!SD.mkdir(foldername)) {
    Serial.println("trip folder creation failed!");
    return;
  }
  Serial.println("folder created!");
  
  char* gpsLogFilename = "gps.log";
  strncpy(gpsLogPath, foldername, sizeof(gpsLogPath));
  strncat(gpsLogPath, gpsLogFilename, (sizeof(gpsLogFilename) - strlen(gpsLogFilename)) );
  Serial.print("gpsLogPath: "); Serial.println(gpsLogPath);

  gpsLog = SD.open(gpsLogPath, FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (gpsLog) {
    Serial.println("gpsLog open.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening sysLog");
  }
}

void Trip::_createSysLogFile() {
   
  char* sysLogFilename = "sys.log";
  strncpy(sysLogPath, foldername, sizeof(sysLogPath));
  strncat(sysLogPath, sysLogFilename, (sizeof(sysLogFilename) - strlen(sysLogFilename)) );
  Serial.print("sysLogPath: "); Serial.println(sysLogPath);

  sysLog = SD.open(sysLogPath, FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (sysLog) {
    Serial.println("sysLog open.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening sysLog");
  }
}


