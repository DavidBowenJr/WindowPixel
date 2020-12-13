//#include "framework.h"
//#include "ImageHeader.h"
#include "ImageSource.h"

//#include "???"
//#include <stdio.h>

uint8_t* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader) {
	FILE* filePtr; // our file pointer
	BITMAPFILEHEADER bitmapFileHeader; // our bitmap file header
	uint8_t* bitmapImage; // store image data

	uint32_t imageIdx = 0; // image index counter
	if (imageIdx) {}; // not used?....


	fopen_s(&filePtr, (char*)filename, "rb");

	if (filePtr == nullptr) return NULL;
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	if (bitmapFileHeader.bfType != 'MB') // 0X4D42
	{
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, (long)bitmapFileHeader.bfOffBits, SEEK_SET);

	// 32 bits
	int extraBytes = bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight;

	bool bPrep32bit = true;
	if (bPrep32bit)
	{
		// Make a 32 bit buffer Brute Force
		bitmapImage = (uint8_t*)malloc(bitmapInfoHeader->biSizeImage + extraBytes);

		for (SIZE_T z = 0; z < (bitmapInfoHeader->biSizeImage + extraBytes); z++)
			bitmapImage[z] = 0;
	}
	else {
		// Work in Native format. here 24 bits.
		// Havent tested 
		bitmapImage = (uint8_t*)malloc(bitmapInfoHeader->biSizeImage);
	}

	// Verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage); 
		fclose(filePtr);
		return NULL;
	}

	// Read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

	// Make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	//	FLIP the component  g r b

	// If we like to have our image in 32 bits with alpha then use 
	if (bPrep32bit)
	{
		auto sub = bitmapInfoHeader->biSizeImage + extraBytes;
		unsigned char* t = new unsigned char[sub];
		// delete [] t;

		int eindex = 0; if (eindex) {} // not used...
		int inc3 = 0;
		for (SIZE_T i = 0; i < (bitmapInfoHeader->biSizeImage + extraBytes); i += 4)
		{
			t[i]     = (uint8_t)bitmapImage[inc3];
			t[i + 1] = (uint8_t)bitmapImage[inc3 + 1];
			t[i + 2] = (uint8_t)bitmapImage[inc3 + 2];
			t[i + 3] = (uint8_t)bitmapImage[inc3 + 3];
			inc3 += 3;
		}

		for (SIZE_T i = 0; i < (bitmapInfoHeader->biSizeImage + extraBytes); i++)
		{
			bitmapImage[i] = (uint8_t)t[i];
		}

		delete[] t;

	}

	// Close file and return bitmap image data
	fclose(filePtr);
	return (uint8_t*) bitmapImage;

}


///
/*
BITMAPINFOHEADER bitmapInfoHeader;
unsinged char *bitmapData;
....
bitmapData = LoadBitmapFile("mypic.bmp", &bitmapInfoHeader);
// 
//
// Swap the r and b values to get RGB (bitmap is BGR) 

for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
{
	tempRGB = bitmapImage[imageIdx];
	bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
	bitmapImage[imageIdx + 2] = tempRGB;
	}

	// Code has been modified  so don't go by any of this
	// Hazy...
	// Im only useing 32 bit images.



*/