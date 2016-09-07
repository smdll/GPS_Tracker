/*
This SSD1306 driver only works on monochrome 0.96-inch 128x64 OLED with IIC
*/

#ifndef GFX_H
#define GFX_H

#include "Arduino.h"
#include "Print.h"

class GFX : public Print
{
	public:
		GFX(int16_t w, int16_t h);	// Constructor

		// This MUST Be Defined By The Subclass:
		virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

		// These Exist Only With GFX (No Subclass Overrides)
		void
			drawChar(int16_t x, int16_t y, unsigned char c, uint8_t size),
			setCursor(int16_t x, int16_t y);

		virtual size_t write(uint8_t);

	protected:
		int16_t
			_width, _height,	// Display W/H
			cursor_x, cursor_y;
		uint8_t textsize;
};
#endif // GFX_H