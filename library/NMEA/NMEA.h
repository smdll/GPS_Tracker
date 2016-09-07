#ifndef NMEA_H_
#define NMEA_H_

typedef unsigned int uint;
typedef unsigned char uchar;
typedef struct
{
	uint y;		//Date
	uint M;
	uint d;
	uint h;		//Time
	uint m;
	uint s;
	double lat;	//Latitude
	double lon;	//Longitude
	float hgt;	//Height
	uint satC;	//Satelite
	uchar latf;	//Latitude flag
	uchar lonf;	//Longitude flag
	uchar stat;	//Status
	uint sat[12];
} GPS;

uint c2i(uchar inp);

GPS GPS_Proc(char *GGA, char *RMC);

#endif
