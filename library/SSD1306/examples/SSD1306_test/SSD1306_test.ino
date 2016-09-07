#include <SSD1306.h>

#define SDA_PIN 8
#define SCL_PIN 9

SSD1306 display(SDA_PIN,SCL_PIN);

void setup()
{
  display.initialize();
}
void loop()
{
  display.setCursor(0,0);
  display.print("Hello, world!");
  display.update();
  delay(20);
  display.clear();
}
