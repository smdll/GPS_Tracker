#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // I2C / TWI
int inp = 0;

void setup()
{
  Serial.begin(9600);
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_unifont);
}

void loop()
{
  u8g.firstPage();
  while (Serial.available() > 0)
    inp = Serial.read();
  do u8g.drawStr( 0, 22, (char*)inp); while (u8g.nextPage());
}
