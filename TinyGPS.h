#ifndef TinyGPS_h
#define TinyGPS_h

#include "Arduino.h"

#include <stdlib.h>

#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001

class TinyGPS
{
public:

  TinyGPS();
  bool encode(char c); // process one character received from GPS
  TinyGPS &operator << (char c) {encode(c); return *this;}

  inline long altitude() { return _altitude; }

  // satellites used in last full GPGGA sentence
  inline unsigned short satellites() { return _numsats; }

  // horizontal dilution of precision in 100ths
  inline unsigned long hdop() { return _hdop; }

  void f_get_position(float *latitude, float *longitude);
  void get_datetime(int *year, byte *month, byte *day, byte *hour, byte *minute, byte *second);
  float f_altitude();
  float f_speed();

private:
  enum {_GPS_SENTENCE_GPGGA, _GPS_SENTENCE_GPRMC, _GPS_SENTENCE_OTHER};

  // properties
  unsigned long _time, _new_time;
  unsigned long _date, _new_date;
  long _latitude, _new_latitude;
  long _longitude, _new_longitude;
  long _altitude, _new_altitude;
  unsigned long  _speed, _new_speed;
  unsigned long  _hdop, _new_hdop;
  unsigned short _numsats, _new_numsats;

  unsigned long _last_time_fix, _new_time_fix;
  unsigned long _last_position_fix, _new_position_fix;

  // parsing state variables
  byte _parity;
  bool _is_checksum_term;
  char _term[15];
  byte _sentence_type;
  byte _term_number;
  byte _term_offset;
  bool _gps_data_good;

  // internal utilities
  int from_hex(char a);
  unsigned long parse_decimal();
  unsigned long parse_degrees();
  bool term_complete();
  bool gpsisdigit(char c) { return c >= '0' && c <= '9'; }
  long gpsatol(const char *str);
  int gpsstrcmp(const char *str1, const char *str2);
};

#endif
