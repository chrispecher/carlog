#ifndef Trip_h
#define Trip_h

#include "SdFat.h"

class Trip {
  public:
    void create();
    File gpsLog;
    File sysLog;
    char gpsLogPath[30];
    char sysLogPath[30];
    char foldername[9];
  private:
    void _createGpsLogFile();
    void _createSysLogFile();
    
};

#endif
