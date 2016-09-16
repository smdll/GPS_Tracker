/*
This SSD1306 driver only works on monochrome 0.96-inch 128x64 OLED with IIC
*/

#include "GFX.h"
#include "glcdfont.c"

#ifdef __AVR__
	#include <avr/pgmspace.h>
#else
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

GFX::GFX(int16_t w, int16_t h)
{
	_width = 128;
	_height = 64;
	cursor_y = cursor_x = 0;
	textsize = 1;
}

size_t GFX::write(uint8_t c)
{
	if (c == '\n')
	{
		cursor_y += textsize*8;
		cursor_x  = 0;
	}
	else
	{
		drawChar(cursor_x, cursor_y, c, textsize);
		cursor_x += textsize*6;
		if (cursor_x > (_width - textsize*6))
		{
			cursor_y += textsize*8;
			cursor_x = 0;
		}
	}
	return 1;
}

// Draw A Character
void GFX::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t size)
{
	if((x >= _width) || (y >= _height) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0))	// Clip Right | Bottom | Left | Top
		return;
	for (int8_t i=0; i<6; i++ )
	{
		uint8_t line;
		if (i == 5) 
			line = 0x0;
		else 
			line = pgm_read_byte(font+(c*5)+i);
		for (int8_t j = 0; j<8; j++)
		{
			if (line & 0x1)
				drawPixel(x+i, y+j, 1);
			else
				drawPixel(x+i, y+j, 0);
			line >>= 1;
		}
	}
}

void GFX::setCursor(int16_t x, int16_t y)
{
	cursor_x = x;
	cursor_y = y;
}