/*
This SSD1306 driver only works on monochrome 0.96-inch 128x64 OLED with IIC
*/

#ifndef SSD1306_H_
#define SSD1306_H_

#include "Arduino.h"
#include <GFX.h>

using namespace std;

class SSD1306 : public GFX
{
	public:
		SSD1306(int sda, int scl);
		~SSD1306();
		//Initialize In Setup
		virtual bool initialize();

		//Update The Framebuffer
		virtual void update();
		//Totoally 8 Rows In Vertical Direction
		virtual void updateRow(int rowIndex);
		virtual void updateRow(int startRow, int endRow);
	
		//Draw One Pixel
		virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

		//Clear
		void clear(bool isUpdateHW = false);

	protected:
		//Write One Byte
		void writeByte(unsigned char  b);
		void writeCommand(unsigned char  cmd);

		//Atomic Control
		void startIIC();	//Turn On The IIC
		void stopIIC();		//Turn Off The IIC
		void startDataSequence();

	protected:
		int m_sda;
		int m_scl;
		unsigned char* m_pFramebuffer;//The Frame Buffer For The GFX. Size=64x8 Bytes
};
#endif