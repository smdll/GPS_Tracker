#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

#define EEPTZ 0

typedef struct
{
  float lat, lon, alt, spd;
  int year;
  byte month, day, hour, minute, second;
  unsigned long hdop;
  unsigned short sat;
} GPSdat;

File GPX;

bool WriteInit(int CSPin)
{
	if(!SD.begin(CSPin)) return false;
	GPX = SD.open("Track.gpx", FILE_WRITE);
	if(GPX) return true;
	else return false;
}

bool IsFile()
{
	if(!GPX) return false;
	return true;
}

bool CreateHead()
{
	if(!GPX) return false;
	GPX.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>");
	
	GPX.println("<gpx>");
	GPX.println("\t<trk>");
	GPX.println("\t\t<name>GPS Track Output</name>");
	GPX.println("\t\t\t<trkseg>");

	return true;
}

bool WriteSeg(GPSdat *dat)
{
	if(!GPX) return false;
	GPX.print("\t\t\t\t<trkpt lat=\"");
	GPX.print(dat->lat);
	GPX.print("\" lon=\"");
	GPX.print(dat->lon);
	GPX.println("\">");
	
	GPX.print("\t\t\t\t\t<ele>");
	GPX.print(dat->alt);
	GPX.println("</ele>");
	
	GPX.print("\t\t\t\t\t<time>20");
	GPX.print(dat->year);
	GPX.print("-");
	GPX.print(dat->month);
	GPX.print("-");
	GPX.print(dat->day);
	GPX.print("T");
	GPX.print(dat->hour);
	GPX.print(":");
	GPX.print(dat->minute);
	GPX.print(":");
	GPX.print(dat->second);
	GPX.println("Z</time>");
	
	GPX.println("\t\t\t\t</trkpt>");
	
	return true;
}

bool CreateEnd()
{
	if(!GPX) return false;
	
	GPX.println("\t\t\t</trkseg>");
	GPX.println("\t\t</trk>");
	
	GPX.println("</gpx>");
	
	GPX.close();  //!!!!!!!!!!!!!!!
	
	return true;
}

void SetTZ(int TZ)
{
	EEPROM.update(EEPTZ, TZ);
}

int ReadTZ()
{
	return EEPROM.read(EEPTZ);
}
