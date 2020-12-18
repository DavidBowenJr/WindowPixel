
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

// Are both colors equal?
bool operator==(const ColorRGB& color, const ColorRGB& color2)
{
	return(color.r == color2.r && color.g == color2.g && color.b == color2.b);
}

// Are both colors not equal?
bool operator!=(const ColorRGB& color, const ColorRGB& color2)
{
	return(!(color.r == color2.r && color.g == color2.g && color.b == color2.b));
}


ColorHSL::ColorHSL(uint8 h, uint8 s, uint8 l)
{
    this->h = h;
    this->s = s;
    this->l = l;
}
ColorHSL::ColorHSL()
{
    this->h = 0;
    this->s = 0;
    this->l = 0;
}
ColorHSV::ColorHSV(uint8 h, uint8 s, uint8 v)
{
    this->h = h;
    this->s = s;
    this->v = v;
}
ColorHSV::ColorHSV()
{
    this->h = 0;
    this->s = 0;
    this->v = 0;
}






//Converts an RGB color to HSL color
ColorHSL RGBtoHSL(const ColorRGB& colorRGB)
{
    float r, g, b, h = 0.f, s = 0.f, l; //this function works with floats between 0 and 1
    r = colorRGB.r / 256.0f;
    g = colorRGB.g / 256.0f;
    b = colorRGB.b / 256.0f;

    float param1 = max(g, b);
    float maxColor = max(r, param1); //std::max(g, b));
    param1 = min(g, b);
    float minColor = min(r, param1); // std::min(g, b));

    if (minColor == maxColor) //R = G = B, so it's a shade of grey
    {
        h = 0.f; //it doesn't matter what value it has
        s = 0.f;
        l = r; //doesn't matter if you pick r, g, or b
    }
    else
    {
        l = (minColor + maxColor) / 2;

        if (l < 0.5f) s = (maxColor - minColor) / (maxColor + minColor);
        if (l >= 0.5f) s = (maxColor - minColor) / (2.0f - maxColor - minColor);

        if (r == maxColor) h = (g - b) / (maxColor - minColor);
        if (g == maxColor) h = 2.0f + (b - r) / (maxColor - minColor);
        if (b == maxColor) h = 4.0f + (r - g) / (maxColor - minColor);

        h /= 6; //to bring it to a number between 0 and 1
        if (h < 0) h += 1;
    }

    ColorHSL colorHSL;
    colorHSL.h = int(h * 255.0f);
    colorHSL.s = int(s * 255.0f);
    colorHSL.l = int(l * 255.0f);
    return colorHSL;
}

//Converts an HSL color to RGB color
ColorRGB HSLtoRGB(const ColorHSL& colorHSL)
{
    float r, g, b, h, s, l; //this function works with floats between 0 and 1
    float temp1, temp2, tempr, tempg, tempb;
    h = colorHSL.h / 256.0f;
    s = colorHSL.s / 256.0f;
    l = colorHSL.l / 256.0f;

    //If saturation is 0, the color is a shade of grey
    if (s == 0.f) r = g = b = l;
    //If saturation > 0, more complex calculations are needed
    else
    {
        //set the temporary values
        if (l < 0.5f) temp2 = l * (1 + s);
        else temp2 = (l + s) - (l * s);
        temp1 = 2 * l - temp2;
        tempr = h + 1.0f / 3.0f;
        if (tempr > 1.0f) tempr--;
        tempg = h;
        tempb = h - 1.0f / 3.0f;
        if (tempb < 0.0f) tempb++;

        //red
        if (tempr < 1.0f / 6.0f) r = temp1 + (temp2 - temp1) * 6.0f * tempr;
        else if (tempr < 0.5f) r = temp2;
        else if (tempr < 2.0f / 3.0f) r = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempr) * 6.0f;
        else r = temp1;

        //green
        if (tempg < 1.0f / 6.0f) g = temp1 + (temp2 - temp1) * 6.0f * tempg;
        else if (tempg < 0.5f) g = temp2;
        else if (tempg < 2.0f / 3.0f) g = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempg) * 6.0f;
        else g = temp1;

        //blue
        if (tempb < 1.0f / 6.0f) b = temp1 + (temp2 - temp1) * 6.0f * tempb;
        else if (tempb < 0.5f) b = temp2;
        else if (tempb < 2.0f / 3.0f) b = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempb) * 6.0f;
        else b = temp1;
    }

    ColorRGB colorRGB;
    colorRGB.r = int(r * 255.0f);
    colorRGB.g = int(g * 255.0f);
    colorRGB.b = int(b * 255.0f);
    return colorRGB;
}

//Converts an RGB color to HSV color
ColorHSV RGBtoHSV(const ColorRGB& colorRGB)
{
    float r, g, b, h = 0.0f, s = 0.0f, v; //this function works with floats between 0 and 1
    r = colorRGB.r / 256.0f;
    g = colorRGB.g / 256.0f;
    b = colorRGB.b / 256.0f;

    float param1 = max(g, b);
    float maxColor = max(r, param1); // std::max(g, b));
    param1 = min(g, b);
    float minColor = min(r, param1); // std::min(g, b));

    v = maxColor;

    if (maxColor != 0.0f) //avoid division by zero when the color is black
    {
        s = (maxColor - minColor) / maxColor;
    }

    if (s == 0.0f)
    {
        h = 0.0f; //it doesn't matter what value it has
    }
    else
    {
        if (r == maxColor) h = (g - b) / (maxColor - minColor);
        if (g == maxColor) h = 2.0f + (b - r) / (maxColor - minColor);
        if (b == maxColor) h = 4.0f + (r - g) / (maxColor - minColor);

        h /= 6.0f; //to bring it to a number between 0 and 1
        if (h < 0.0f) h++;
    }

    ColorHSV colorHSV;
    colorHSV.h = int(h * 255.0f);
    colorHSV.s = int(s * 255.0f);
    colorHSV.v = int(v * 255.0f);
    return colorHSV;
}

//Converts an HSV color to RGB color
ColorRGB HSVtoRGB(const ColorHSV& colorHSV)
{
   
    float r, g, b, h, s, v; //this function works with floats between 0 and 1
    h = colorHSV.h / 256.0f;
    s = colorHSV.s / 256.0f;
    v = colorHSV.v / 256.0f;

    //if saturation is 0, the color is a shade of grey
    if (s == 0.0f) r = g = b = v;
    //if saturation > 0, more complex calculations are needed
    else
    {
        float f, p, q, t;
        int i;
        h *= 6.0f; //to bring hue to a number between 0 and 6, better for the calculations
        i = int(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
        f = h - i;//the fractional part of h

        p = v * (1.0f - s);
        q = v * (1.0f - (s * f));
        t = v * (1.0f - (s * (1.0f - f)));

        switch (i)
        {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        default: r = g = b = 0; break;
        }
    }
    ColorRGB colorRGB;
    colorRGB.r = int(r * 255.0f);
    colorRGB.g = int(g * 255.0f);
    colorRGB.b = int(b * 255.0f);
    return colorRGB;
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
    colorRGB.b = colorINT % 256;
    return colorRGB;
}


