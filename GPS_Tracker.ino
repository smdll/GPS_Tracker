#include "SSD1306.h"
#include "TinyGPS.h"

#define SDA_PIN 8
#define SCL_PIN 9

struct
{
  float lat, lon, alt, spd;
  int year;
  byte month, day, hour, minute, second;
  unsigned long hdop;
  unsigned short sat;
} now;

SSD1306 display(SDA_PIN,SCL_PIN);
TinyGPS gps;

void show()
{
  display.clear();
  display.setCursor(0,0);
  display.print("Lat:");
  display.print(now.lat);

  display.setCursor(0,9);
  display.print("Lon:");
  display.print(now.lon);

  display.setCursor(0,18);
  display.print("Sat:");
  display.print(now.sat);
  
  display.setCursor(0,27);
  display.print("Alt:");
  display.print(now.alt);
  
  display.setCursor(0,36);
  display.print("HDOP:");
  display.print(now.hdop);
  
  display.setCursor(0,45);
  display.print("Date:");
  display.print(now.year);
  display.print("-");
  display.print(now.month);
  display.print("-");
  display.print(now.day);
  
  display.setCursor(0,54);
  display.print("Time:");
  display.print(now.hour);
  display.print(":");
  display.print(now.minute);
  display.print(":");
  display.print(now.second);
  
  display.update();
}

void showvalid()
{
  display.clear();
  display.setCursor(0,0);
  display.print("Searching...");
  display.update();
}

void setup()
{
  Serial.begin(9600);
  display.initialize();
  display.setCursor(0,0);
  display.print("GPS Tracker By SMDLL");
  display.update();
  delay(1500);
}

void loop()
{
  bool newData = false;
  unsigned long chars;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    gps.f_get_position(&now.lat, &now.lon);
    gps.get_datetime(&now.year, &now.month, &now.day, &now.hour, &now.minute, &now.second);
    now.sat=gps.satellites();
    now.alt=gps.altitude();
    now.hdop=gps.hdop()/100;
    show();
  }
  else showvalid();
}
