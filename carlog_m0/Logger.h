#ifndef Logger_h
#define Logger_h

#include "Gps.h"
#include "System.h"
#include "Trip.h"

#define LOG_INTERVAL 1000

class Logger {
  public:
    Logger(Gps*);
    void begin();
    Trip trip;
    void log(Gps);
    void log(System);
    void end();
    bool recording = false;
    bool gpsLogged = false;
    bool sysLogged = false;
    uint8_t hour, minute, seconds, year, month, day;
  private:
    uint32_t _log_sys_timer;
    uint32_t _log_gps_timer;
    Gps* _gps;
    char* _dateTime();
};

#endif
