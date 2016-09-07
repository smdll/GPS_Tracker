#include"NMEA.h"

uint c2i(uchar inp)
{
	return inp - 48;
}

GPS GPS_Proc(char *GGA, char *RMC)
{
	GPS now;
	uint i=0,j;
	{//Processing GPGGA Data
		i=7;	//Begin Of The First Section: Time(hhmmss)
		now.h = 10*c2i(GGA[i]) + c2i(GGA[i+1]);
		now.m = 10*c2i(GGA[i+2]) + c2i(GGA[i+3]);
		now.s = 10*c2i(GGA[i+4]) + c2i(GGA[i+5]);

		i+=10;	//The Second Section: Latitude(ddmm.sssss),LatHemi(N/S)
		now.lat = 1000*c2i(GGA[i]) + 100*c2i(GGA[i+1]) + 10*c2i(GGA[i+2]) + c2i(GGA[i+3]) + 0.1*c2i(GGA[i+5]) + 0.01*c2i(GGA[i+6]) + 0.001*c2i(GGA[i+7]) + 0.0001*c2i(GGA[i+8]) + 0.00001*c2i(GGA[i+9]);
		now.latf=GGA[i+11];

		i+=13;	//The Third Section: Longitude(dddmm.sssss),LonHemi(W/E)
		now.lon = 10000*c2i(GGA[i]) + 1000*c2i(GGA[i+1]) + 100*c2i(GGA[i+2]) + 10*c2i(GGA[i+3]) + c2i(GGA[i+4]) + 0.1*c2i(GGA[i+6]) + 0.01*c2i(GGA[i+7]) + 0.001*c2i(GGA[i+8]) + 0.0001*c2i(GGA[i+9]) + 0.00001*c2i(GGA[i+10]);
		now.lonf=GGA[i+12];

		i+=14;	//The Forth Section: GPS Status(x)
		now.stat = c2i(RMC[i]);
		if(now.stat==0||now.stat==6) return now;

		i+=2;	//The Fifth Section: Satelite Count(xx)
		now.sat = 10*c2i(GGA[i]) + c2i(GGA[i+1]);

		i+=8;	//The Final Section: Height(xxxxx.x)
		now.hgt = c2i(GGA[i]);
		while(GGA[i+1]!='.')
		{
			now.hgt = 10*now.hgt + c2i(GGA[i+1]);
			i++;
		}
		i+=2;
		now.hgt += 0.1*c2i(GGA[i]);
	}
	{//Processing GPRMC Data
		i=48;//Ignore All The Same Sections, Targeting On The 
		j=0;
		while(j <= 2)
		{
			while(RMC[i] != ',')
			{
				i++;
				j++;
			}
		}
		i+=2;
		now.d = 10*c2i(RMC[i]) + c2i(RMC[i+1]);
		now.M = 10*c2i(RMC[i+2]) + c2i(RMC[i+3]);
		now.y = 2000 + 10*c2i(RMC[i+4]) + c2i(RMC[i+5]);
	}
	return now;
}
