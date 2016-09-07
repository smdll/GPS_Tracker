#include <SSD1306.h>

#define SDA_PIN 8
#define SCL_PIN 9

typedef unsigned int uint;

SSD1306 display(SDA_PIN,SCL_PIN);

int inp = 0;

void show()
{
  display.setCursor(0,0);
  display.print();
  display.update();
}

void setup()
{
  Serial.begin(9600);
  display.initialize();
  display.setCursor(0,0);
  display.print("GPS_Tracker By SMD");
  display.update();
  delay(1500);
  display.clear();
}

void loop()
{
  
}
