#pragma once
#include "framework.h"
#pragma pack(push, 1)
typedef struct XtagBITMAPFILEHEADER
{
	uint16_t bfType;      // specifies the file type
	uint32_t bfSize;      // specifies the size in bytes
	uint16_t bfReserved1; // must be 0
	uint16_t bfReserved2; // must be 0
	uint32_t bOffBits;    // specifies the offset in Bytes from the bitmapfileheader to the bitmap bits

} XBITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct XtagBITMAPINFOHEADER
{
	uint32_t biSize;	// specifies the number of Bytes required for the struct
	int32_t  biWidht;	// width in pixels
	int32_t  biHeight;	// height in pixels
	uint16_t biPlanes;	// specifies the number of color planes, must be 1
	uint16_t biBitCount; // the number of bits per pixel
	uint32_t biCompression;	// type of compression
	uint32_t biSizeImage;	// size of image in Bytes
	int32_t  biXPelsPerMeter;	// number of pixels per meter in x axis
	int32_t  biYPelsPerMeter;	// number of pixels per meter in y axis
	uint32_t biClrUsed;			// number of colors used by the bitmap
	uint32_t biClrImportant;	// number of colors that are important

} XBITMAPINFOHEADER;

#pragma pack(pop)

//uint8_t* LBF

