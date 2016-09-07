#include <SSD1306.h>
#include <NMEA.h>

#define SDA_PIN 8
#define SCL_PIN 9

typedef unsigned int uint;

SSD1306 display(SDA_PIN,SCL_PIN);
GPS now;

int inp = 0;

void show()
{
  display.clear();
  display.setCursor(0,0);
  display.print(now.lat);
  display.setCursor(0,8);
  display.print(now.lon);
  display.update();
}

void strcpy(char *a, char *b,int len)
{
  for(int i=0;i<=len;i++) a[i]=b[i];
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
  char comdata[100],*p3;
  char gga[100], rmc[100], *p1, *p2;
  int i=0;
  p1=gga;
  p2=rmc;
  p3=comdata;
  while(Serial.available()>0)
  {
    comdata[i]=char(Serial.read());
    i++;
    delay(2);
  }
  if(comdata[0]=='$')
  {
    if(comdata[3]=='R') strcpy(p2, comdata, i);
    else if(comdata[4]=='G'&&comdata[5]=='A') strcpy(p1, comdata, i);
  }
  now=GPS_Proc(p1, p2);
  if(now.stat!=0) show();
}
