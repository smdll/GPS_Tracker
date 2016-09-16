#include <SPI.h>
#include <SD.h>

class WriteGPX
{
	WriteGPX();
	public:
		int CSPin = 4; // CS pin is D4 by default
		unsigned char fname = 1;
	bool init(int pin)
	{
		if(pin) CSPin = pin;
		if(!SD.begin(CSPin)) return false;
		while(SD.exists(fname+'0'))
	}
	
}