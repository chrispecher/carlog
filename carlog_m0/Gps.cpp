#include "Gps.h"

Gps::Gps(Adafruit_GPS *ada_gps)
{
  _gps = ada_gps;
}

void Gps::begin()
{
  _gps->begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  _gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate
  _gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  _gps->sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  // Turn off updates on antenna status, if the firmware permits it
  _gps->sendCommand(PGCMD_NOANTENNA);

  //init timers
  _update_timer = millis();
}

void Gps::update() {
   // if millis() or timer wraps around, we'll just reset it
  if (_update_timer > millis())  _update_timer = millis();
  // approximately every UPDATE_INTERVAL seconds or so, print out the current stats
  if (millis() - _update_timer < GPS_UPDATE_INTERVAL) {
    return; 
  }
  _update_timer = millis();

  char c = (_gps->read());
  if (GPSECHO)
        if (c) Serial.print(c);
  if (_gps->newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    
    // Don't call lastNMEA more than once between parse calls!  Calling lastNMEA 
    // will clear the received flag and can cause very subtle race conditions if
    // new data comes in before parse is called again.
    char *stringptr = _gps->lastNMEA();
    
    if (!_gps->parse(stringptr))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another

    // valid parse only sets the fix
    fix = _gps->fix;
    
    // only continue if there is no data yet
    if(!fix) 
      return;
    
    // map data
    if (strstr(stringptr, "RMC")) 
      this->RMC =  stringptr;
    if (strstr(stringptr, "GGA")) 
      this->GGA =  stringptr;
    this->hour = _gps->hour;
    this->minute = _gps->minute;
    this->seconds = _gps->seconds;
    this->year = _gps->year;
    this->month = _gps->month;
    this->day = _gps->day;
    this->fix = _gps->fix;
    this->speed = _gps->speed;
    this->altitude = _gps->altitude;
    this->latitudeDegrees = _gps->latitudeDegrees;
    this->longitudeDegrees = _gps->longitudeDegrees;
    this->satellites = _gps->satellites;
  }
}

bool Gps::move () {
  return fix && speed > MOVE_THRESHOLD_KNTS;
}

void Gps::print_debug() {
    Serial.print("\nTime: ");
    Serial.print(_gps->hour, DEC); Serial.print(':');
    Serial.print(_gps->minute, DEC); Serial.print(':');
    Serial.print(_gps->seconds, DEC); Serial.print('.');
    Serial.println(_gps->milliseconds);
    Serial.print("Date: ");
    Serial.print(_gps->day, DEC); Serial.print('/');
    Serial.print(_gps->month, DEC); Serial.print("/20");
    Serial.println(_gps->year, DEC);
    Serial.print("Fix: "); Serial.print((int)_gps->fix);
    Serial.print(" quality: "); Serial.println((int)_gps->fixquality); 
    if (_gps->fix) {
      Serial.print("Location: ");
      Serial.print(_gps->latitude, 4); Serial.print(_gps->lat);
      Serial.print(", "); 
      Serial.print(_gps->longitude, 4); Serial.println(_gps->lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(_gps->latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(_gps->longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(_gps->speed);
      Serial.print("Angle: "); Serial.println(_gps->angle);
      Serial.print("Altitude: "); Serial.println(_gps->altitude);
      Serial.print("Satellites: "); Serial.println((int)_gps->satellites);
    }
}


