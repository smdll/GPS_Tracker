/*
This SSD1306 driver only works on monochrome 0.96-inch 128x64 OLED with IIC
*/

#include "SSD1306.h"

SSD1306::SSD1306(unsigned char sda, unsigned char scl):
GFX(128,64)
{
	m_sda = sda;
	m_scl = scl;
	m_pFramebuffer = 0;
}

SSD1306::~SSD1306()
{
	//Releasing Memory.
	if(m_pFramebuffer)
		free(m_pFramebuffer);
}

//Initialized The SSD1306 In The Setup Function
bool SSD1306::initialize()
{
	//Setup The Pin Mode
	pinMode(m_sda, OUTPUT);
	pinMode(m_scl, OUTPUT);

	//Malloc The Framebuffer
	m_pFramebuffer = (unsigned char*)malloc(1024);
	if(m_pFramebuffer == 0)
		return false;
	memset(m_pFramebuffer, 0, 1024);	//Clear It

	//Write Command To The Screen Registers
	writeCommand(0xAE);	//Display Off

	writeCommand(0x00);	//Memory Addressing
	writeCommand(0x10);	//00:Horizontal Addressing;01:Vertical Addressing;10:Page Addressing (RESET);11:Invalid

	writeCommand(0x40);	//Page Start Address for Page Addressing Mode,0-7
	writeCommand(0xB0);	//COM Output Scan Direction
	writeCommand(0x81);	//Low Column Address
	writeCommand(0xCF);	//High Column Address
	writeCommand(0xA1);	//Start Line Address
	writeCommand(0xA6);	//Contrast Control Register
	writeCommand(0xA8);
	writeCommand(0x3F);	//Segment Re-map 0 to 127
	writeCommand(0xC8);	//Normal Display
	writeCommand(0xD3);	//Multiplex Ratio(1 to 64)
	writeCommand(0x00);
	writeCommand(0xD5);	//0xa4:Output follows RAM content;0xa5:Output ignores RAM content
	writeCommand(0x80);	//Display Offset
	writeCommand(0xD9);
	writeCommand(0xF1);	//Display Clock Divide Ratio/Oscillator Frequency
	writeCommand(0xDA);	//Divide Ratio
	writeCommand(0x12);	//Pre-charge Period
	writeCommand(0xDB);
	writeCommand(0x40);	//Com Pins Hardware Configuration
	writeCommand(0x8D);	//Vcomh
	writeCommand(0x14);	//0x20,0.77xVcc
	writeCommand(0xAF);	//DC-DC Enable

	writeCommand(0xAF);	//Turn On OLED Panel 

	delay(10);	//Wait For Loading
	return true;
}

void SSD1306::clear(bool isUpdateHW)
{
	memset(m_pFramebuffer, 0, 1024);	//Clear The Back Buffer
	if(isUpdateHW)
		update();	//Update The HW
}

void SSD1306::writeCommand(unsigned char cmd)
{
	startIIC();
	writeByte(0x78);	//Slave address SA0=0
	writeByte(0x00);	//Write Command
	writeByte(cmd);
	stopIIC();
}

void SSD1306::writeByte(unsigned char b)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		if((b << i) & 0x80)
			digitalWrite(m_sda, HIGH);
		else
			digitalWrite(m_sda, LOW);
		digitalWrite(m_scl, HIGH);
		digitalWrite(m_scl, LOW);
	}
	digitalWrite(m_sda, HIGH);
	digitalWrite(m_scl, HIGH);
	
	digitalWrite(m_scl, LOW);
}

void SSD1306::startIIC()
{
	digitalWrite(m_scl, HIGH);
	digitalWrite(m_sda, HIGH);
	digitalWrite(m_sda, LOW);
	digitalWrite(m_scl, LOW);
}

void SSD1306::stopIIC()
{
	digitalWrite(m_scl, LOW);
	digitalWrite(m_sda, LOW);
	digitalWrite(m_scl, HIGH);
	digitalWrite(m_sda, HIGH);	
}

void SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	unsigned char row;
	unsigned char offset;
	unsigned char  preData;	//Previous Data.
	unsigned char val;
	int16_t  index;

	if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64) || ( m_pFramebuffer==0))
		return;

	//Get The Previous Data
	row = y/8;
	offset =y%8;
	index = row*128 + x;
	preData = m_pFramebuffer[index];

	//Set Pixel
	val = 1<<offset;
	if(color!=0)	//White Set Bit
		m_pFramebuffer[index] = preData | val;
	else			//Black Clear Bit
		m_pFramebuffer[index] = preData & (~val);
}

void SSD1306::startDataSequence()
{
	startIIC();
	writeByte(0x78);
	writeByte(0x40);	//Write Data
}

void SSD1306::update()
{
	unsigned int  i=0;
	unsigned char m,n;
	for(m=0; m<8; m++)
	{
		writeCommand(0xb0+m);	//Page0->Page1
		writeCommand(0x00);		//Low Column Start Address
		writeCommand(0x10);		//High Column Start Address
		
		startDataSequence();
		for(n=0; n<128; n++)
			writeByte(m_pFramebuffer[i++]);
		stopIIC();
	}
}

void SSD1306::updateRow(int rowID)
{
	unsigned char x;
	unsigned int  index;
	if(rowID>=0 && rowID<8 && m_pFramebuffer)
	{
		//Set The Position
		startIIC();
		writeByte(0x78);	//Slave Address,SA0=0
		writeByte(0x00);	//Write Command

		writeByte(0xb0+rowID);
		writeByte(((x&0xf0)>>4)|0x10);	//|0x10
		writeByte((x&0x0f)|0x01);		//|0x01

		stopIIC();

		//Start Painting The Buffer
		startDataSequence();
		for(x=0; x<128; x++)
		{
			index = rowID*128+x;
	  		writeByte(m_pFramebuffer[index]);
		}
		stopIIC();
	}
}

void SSD1306::updateRow(int startID, int endID)
{
	unsigned char y = 0;
	for(y=startID; y<endID; y++)
		updateRow(y);
}
