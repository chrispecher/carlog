#ifndef Gps_h
#define Gps_h

#include <Adafruit_GPS.h>

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false
#define GPS_UPDATE_INTERVAL 1
#define MOVE_THRESHOLD_KNTS 1
//#define KNOTS_KMH_RATIO 0.539957

class Gps
{
  public:
    Gps(Adafruit_GPS *ser);
    void begin();
    void update();
    boolean move();
    void serialize();
    void print_debug(); 
    
    char* RMC;
    char* GGA;

    // Adafruit GPS
    uint8_t hour, minute, seconds, year, month, day;
    uint16_t milliseconds;
    // Floating point latitude and longitude value in degrees.
    float latitude, longitude;
    // Fixed point latitude and longitude value with degrees stored in units of 1/100000 degrees,
    // and minutes stored in units of 1/100000 degrees.  See pull #13 for more details:
    //   https://github.com/adafruit/Adafruit-GPS-Library/pull/13
    int32_t latitude_fixed, longitude_fixed;
    float latitudeDegrees, longitudeDegrees;
    float geoidheight, altitude;
    float speed, angle, magvariation, HDOP;
    char lat, lon, mag;
    boolean fix;
    uint8_t fixquality, satellites;

  private:
    Adafruit_GPS* _gps;
    uint32_t _update_timer;
};

#endif
