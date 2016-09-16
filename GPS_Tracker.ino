#include "SSD1306.h"
#include "TinyGPS.h"

#define SDA_PIN 8
#define SCL_PIN 9

typedef unsigned int uint;

SSD1306 display(SDA_PIN,SCL_PIN);
TinyGPS gps;

void show(float *data)
{
  display.clear();
  display.setCursor(0,0);
  display.print("Latitude:");
  display.setCursor(50,0);
  display.print(data[0]);
  display.setCursor(0,8);
  display.print("Longitude:");
  display.setCursor(50,8);
  display.print(data[1]);
  display.setCursor(0,16);
  display.print("Satellite:");
  display.setCursor(50,16);
  display.print(data[2]);
  display.setCursor(0,24);
  display.print("HDOP:");
  display.setCursor(50,24);
  display.print(data[3]);
  display.update();
}

void setup()
{
  Serial.begin(9600);
  display.initialize();
}

void loop()
{
  bool newData = false;
  unsigned long chars;

  // For one second we parse GPS data and report some key values
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
    float data[4];
    unsigned long age;
    gps.f_get_position(&data[0], &data[1], &age);
    data[2]=(float)gps.satellites();
    data[3]=(float)gps.hdop();
    show(data);
  }
}
