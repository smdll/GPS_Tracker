//#include <SD.h>
#include <SoftwareSerial.h>
#include "SSD1306.h"
#include "TinyGPS.h"

struct
{
  double lat, lon;
  float alt, spd;
  byte year, month, day, hour, minute, second;
  unsigned long hdop;
  unsigned char sat;
} now;
SoftwareSerial mySerial(5, 6); // RX, TX
SSD1306 display(A4, A5);
TinyGPS gps;
//File GPX;

unsigned char TZ = 8; // set timezone here
/*
volatile bool isRec = false, isFile = false;

bool CreateHead()
{
  if(!SD.begin(4)) return false;
  GPX = SD.open("Track.gpx", FILE_WRITE);
  if(!GPX) return false;
  GPX.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n<gpx>\n\t<trk>\n\t\t<name>GPS Track Output</name>\n\t\t\t<trkseg>");
  isFile = true;
  return true;
}

bool WriteSeg()
{
  GPX.print("\t\t\t\t<trkpt lat=\"");
  GPX.print(now.lat);
  GPX.print("\" lon=\"");
  GPX.print(now.lon);
  GPX.print("\">\n\t\t\t\t\t<ele>");
  GPX.print(now.alt);
  GPX.print("</ele>\n\t\t\t\t\t<time>20");
  GPX.print(now.year);
  GPX.print("-");
  GPX.print(now.month);
  GPX.print("-");
  GPX.print(now.day);
  GPX.print("T");
  GPX.print(now.hour);
  GPX.print(":");
  GPX.print(now.minute);
  GPX.print(":");
  GPX.print(now.second);
  GPX.println("Z</time>\n\t\t\t\t</trkpt>");
  return true;
}

bool CreateEnd()
{
  GPX.println("\t\t\t</trkseg>\n\t\t</trk>\n</gpx>");
  GPX.close();
  return true;
}
*/
void show()
{
  display.clear();
  display.setCursor(0,0);
  display.print("Lat:");
  display.print(now.lat);
  display.print(" Lon:");
  display.print(now.lon);

  display.setCursor(0,9);
  display.print("Speed:");
  display.print(now.spd);
  display.print("m/s");
  
  display.setCursor(0,18);
  display.print("Alt:");
  display.print(now.alt/100.0);
  display.print("m");
  
  display.setCursor(0,27);
  display.print("Sat:");
  display.print(now.sat);
  display.print("  HDOP:");
  display.print(now.hdop);
  
  display.setCursor(0,36);
  display.print("Date:20");
  display.print(now.year);
  display.print("-");
  display.print(now.month);
  display.print("-");
  display.print((now.hour < 16) ? now.day : now.day + 1);
  
  display.setCursor(0,45);
  display.print("Time:");
  display.print((now.hour < 16) ? now.hour + TZ : now.hour + TZ-24);
  display.print(":");
  display.print(now.minute);
  display.print(":");
  display.print(now.second);

  /*if(isRec)
  {
    display.setCursor(54,0);
    display.print("Recording Track Points");
  }
  */
  display.update();
}
/*
void showNoCardFile()
{
  display.clear();
  display.setCursor(0,0);
  display.print("No SD Card Found or Can't Create File");
  display.update();
  delay(1000);
}
*/
void showNoData()
{
  display.clear();
  display.setCursor(0,0);
  display.print("Searching...");
  display.update();
}
/*
void Rec()
{
  if(!isRec)
    isRec = -isRec;
  else
  {
    CreateEnd();
    isRec = -isRec;
  }
}
*/
void setup()
{
  //attachInterrupt(3, Rec, CHANGE);
  mySerial.begin(9600);
  display.initialize();
  display.setCursor(0,0);
  display.print("GPS Tracker By SMDLL");
  display.update();
  delay(1500);
  //if(!CreateHead()) showNoCardFile();
}

void loop()
{
  bool newData = false;

  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (mySerial.available())
    {
      char c = mySerial.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    gps.f_get_position(&now.lat, &now.lon);
    gps.get_datetime(&now.year, &now.month, &now.day, &now.hour, &now.minute, &now.second);
    now.spd = gps.f_speed();
    now.sat = gps.satellites();
    now.alt = gps.altitude();
    now.hdop = gps.hdop()/100;
    show();
    //if(isRec) WriteSeg();
  }
  else showNoData();
}
