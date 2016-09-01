#include "u8glib.h"

String Re;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available() > 0)
    Re = Serial.readString();
}
