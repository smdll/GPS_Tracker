#ifndef TinyGPS_h
#define TinyGPS_h

#include "Arduino.h"

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

  inline unsigned char satellites() { return _numsats; }

  // horizontal dilution of precision in 100ths
  inline unsigned long hdop() { return _hdop; }

  void f_get_position(double *latitude, double *longitude);
  void get_datetime(byte *year, byte *month, byte *day, byte *hour, byte *minute, byte *second);
  float f_altitude();
  float f_speed();

private:
  enum {_GPS_SENTENCE_GPGGA, _GPS_SENTENCE_GPRMC, _GPS_SENTENCE_OTHER};

  // properties
  unsigned long _time;
  unsigned long _date;
  long _latitude;
  long _longitude;
  long _altitude;
  unsigned long  _speed;
  unsigned char  _hdop;
  unsigned char _numsats;

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
