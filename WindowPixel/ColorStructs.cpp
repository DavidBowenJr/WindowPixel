
#include "colorstruct.h"

ColorRGB::ColorRGB(uint8 r, uint8 g, uint8 b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

ColorRGB::ColorRGB(const ColorRGB8bit& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
}
ColorRGB::ColorRGB()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
}
ColorRGB8bit::ColorRGB8bit(uint8 r, uint8 g, uint8 b)
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

ColorRGB8bit::ColorRGB8bit(const ColorRGB& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
}
ColorRGB8bit::ColorRGB8bit()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

// Add two colors
ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2)
{
	ColorRGB c;
	c.r = color.r + color2.r;
	c.g = color.g + color2.g;
	c.b = color.b + color2.b;
	return c;
}


//Subtract two colors
ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2)
{
	ColorRGB c;
	c.r = color.r - color2.r;
	c.g = color.g - color2.g;
	c.b = color.b - color2.b;
	return c;
}

//Multiplies a color with an integer
ColorRGB operator*(const ColorRGB& color, int a)
{
	ColorRGB c;
	c.r = color.r * a;
	c.g = color.g * a;
	c.b = color.b * a;
	return c;
}

//Multiplies a color with an integer
ColorRGB operator*(int a, const ColorRGB& color)
{
	ColorRGB c;
	c.r = color.r * a;
	c.g = color.g * a;
	c.b = color.b * a;
	return c;
}

//Divides a color through an integer
ColorRGB operator/(const ColorRGB& color, int a)
{
	if (a == 0) return color;
	ColorRGB c;
	c.r = color.r / a;
	c.g = color.g / a;
	c.b = color.b / a;
	return c;
}


//Are both colors equal?
bool operator==(const ColorRGB& color, const ColorRGB& color2)
{
	return(color.r == color2.r && color.g == color2.g && color.b == color2.b);
}

//Are both colors not equal?
bool operator!=(const ColorRGB& color, const ColorRGB& color2)
{
	return(!(color.r == color2.r && color.g == color2.g && color.b == color2.b));
}

uint32 RGBtoINT(const ColorRGB& colorRGB)
{
	return 65536 * colorRGB.r + 256 * colorRGB.g + colorRGB.b;
}

ColorRGB INTtoRGB(uint32 colorINT)
{
	ColorRGB colorRGB;
	colorRGB.r = (colorINT / 65536) % 256;
	colorRGB.g = (colorINT / 256) % 256;
	colorRGB.b =  colorINT % 256;
	return colorRGB;
}




