#pragma once
#include "framework.h"

struct ColorRGB8bit;
// A color with 3 components: r, g, and b
struct ColorRGB
{
	int r;
	int g;
	int b;

	ColorRGB(uint8 r, uint8 g, uint8 b);
	ColorRGB(const ColorRGB8bit& color);
	ColorRGB();
};

ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2);
ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2);
ColorRGB operator*(const ColorRGB& color, int a);
ColorRGB operator*(int a, const ColorRGB& color);
ColorRGB operator/(const ColorRGB& color, int a);

bool operator==(const ColorRGB& color, const ColorRGB& color2);
bool operator!=(const ColorRGB& color, const ColorRGB& color2);

static const ColorRGB RGB_Black(0, 0, 0);
static const ColorRGB RGB_Red(255, 0, 0);
static const ColorRGB RGB_Green(0, 255, 0);
static const ColorRGB RGB_Blue(0, 0, 255);
static const ColorRGB RGB_Cyan(0, 255, 255);
static const ColorRGB RGB_Magenta(255, 0, 255);
static const ColorRGB RGB_Yellow(255, 255, 0);
static const ColorRGB RGB_White(255, 255, 255);
static const ColorRGB RGB_Gray(128, 128, 128);
static const ColorRGB RGB_Grey(192, 192, 192);
static const ColorRGB RGB_Maroon(128, 0, 0);
static const ColorRGB RGB_Darkgreen(0, 128, 0);
static const ColorRGB RGB_Navy(0, 0, 128);
static const ColorRGB RGB_Teal(0, 128, 128);
static const ColorRGB RGB_Purple(128, 0, 128);
static const ColorRGB RGB_Olive(128, 128, 0);

// A color with 3 components: r, g, and b
struct ColorRGB8bit
{
	uint8 r;
	uint8 g;
	uint8 b;

	ColorRGB8bit(uint8 r, uint8 g, uint8 b);
	ColorRGB8bit(const ColorRGB& color);
	ColorRGB8bit();
};

struct ColorHSL
{
	int h;
	int s;
	int l;

	ColorHSL(uint8 h, uint8 s, uint8 l);
	ColorHSL();
};

// A color with 3 components: h, s, and v (value)
struct ColorHSV
{
	int h;
	int s;
	int v;

	ColorHSV(uint8 h, uint8 s, uint8 v);
	ColorHSV();
};


// COLOR CONVERSIONS //
ColorHSL RGBtoHSL(const ColorRGB& colorRGB);
ColorRGB HSLtoRGB(const ColorHSL& colorHSL);
ColorHSV RGBtoHSV(const ColorRGB& colorRGB);
ColorRGB HSVtoRGB(const ColorHSV& colorHSV);
uint32   RGBtoINT(const ColorRGB& colorRGB);
ColorRGB INTtoRGB(uint32 colorINT);

//HINT https: //docs.microsoft.com/en-us/windows/win32/uxguide/vis-color
//  Hue 0 - 360, Saturation 0 - 100, Luminosity 0 --100












