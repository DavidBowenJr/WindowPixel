//#include "framework.h"
//#include "ImageHeader.h"
#include "ImageSource.h"

//#include "???"
//#include <stdio.h>
#ifndef _MY_BITMAP_FILE_H_
#define _MY_BITMAP_FILE_H_
/*
#include <cstdio>
#include <cstdint>
#include <cstdlib>
*/
#ifdef _WORKING_CODE
#define _WORKING_CODE
std::uint8_t* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* pBitmapInfoHeader) {
	FILE* pFile;
//#undef BITMAPFILEHEADER
	BITMAPFILEHEADER bitmapFileHeader;
	std::uint8_t* bitmapImage = nullptr;

	errno_t err = fopen_s(&pFile, filename, "rb");
	if (err != 0 || pFile == nullptr) {
		return nullptr;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
	if (bitmapFileHeader.bfType != 0x4D42) { // 'BM' signature in little-endian
		fclose(pFile);
		return nullptr;
	}

	fread(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	fseek(pFile, static_cast<long>(bitmapFileHeader.bfOffBits), SEEK_SET);

	const auto ImageSize = pBitmapInfoHeader->biSizeImage * 2;
	bitmapImage = new(std::nothrow) std::uint8_t[ImageSize];
	if (bitmapImage == nullptr) {
		fclose(pFile);
		return nullptr;
	}

	memset(bitmapImage, 0, sizeof(std::uint8_t) * ImageSize);


	fread(bitmapImage, pBitmapInfoHeader->biSizeImage, 1, pFile);
	if (bitmapImage == nullptr) {
		delete[] bitmapImage;
		fclose(pFile);
		return nullptr;
	}

	//const auto sub = ImageSize;
	std::uint8_t* temp = new(std::nothrow) std::uint8_t[ImageSize];
	if (temp == nullptr) {
		delete[] bitmapImage;
		fclose(pFile);
		return nullptr;
	}
	
	
	memset(temp, 0, sizeof(std::uint8_t) * ImageSize);


	for (std::size_t i = 0, j = 0; i < (ImageSize/2) && j + 2 < ImageSize && i + 3 < ImageSize; i += 4, j += 3) {
		temp[i] = bitmapImage[j];
		temp[i + 1] = bitmapImage[j + 1];
		temp[i + 2] = bitmapImage[j + 2];
	}


	memcpy(bitmapImage, temp, sizeof(std::uint8_t) * ImageSize);

	delete[] temp;
	fclose(pFile);

	return bitmapImage;
}


#endif

#ifdef _NOT_SURE_CODE
#define _NOT_SURE_CODE
std::uint8_t* LoadBitmapFile(const char* fileName, BITMAPINFOHEADER* bitmapInfoHeader)
{
	std::uint8_t* bitmapData = nullptr;
	std::uint8_t* temp = nullptr;
	FILE* file;
	fopen_s(&file, fileName, "rb");

	if (file)
	{
		// Read the bitmap file header
		BITMAPFILEHEADER bitmapFileHeader;
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file);

		// Verify that this is a bitmap file
		if (bitmapFileHeader.bfType == 'MB')
		{
			// Read the bitmap info header
			fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

			// Calculate the bitmap data offset and size
			std::uint32_t dataSize = bitmapInfoHeader->biSizeImage * 2;
			if (dataSize == 0)
			{
				_ASSERT("!dataSize cannot be zero");
				//	dataSize = (bitmapInfoHeader->biWidth * bitmapInfoHeader->biBitCount + 7) / 8 * bitmapInfoHeader->biHeight;
			}
			uint32_t dataOffset = bitmapFileHeader.bfOffBits;

			// Allocate memory for the bitmap file data
			bitmapData = new uint8_t[dataSize];
			if (bitmapData == nullptr)
			{
				delete[] bitmapData;
				fclose(file);
				return nullptr;
			}

			// Read the bitmap file data
			fseek(file, dataOffset, SEEK_SET);
			fread(bitmapData, 1, dataSize, file);


			temp = new(std::nothrow) std::uint8_t[dataSize];
			if (temp == nullptr)
			{
				delete[] temp;
				fclose(file);
				return nullptr;
			}



			//ZeroMemory(temp, sizeof(std::uint8_t) * dataSize);
			for (std::size_t i = 0, j = 0; i < (bitmapInfoHeader->biSizeImage) && j + 2 < dataSize && i + 3 < dataSize; i += 4, j += 3) {
				temp[i] = bitmapData[j];
				temp[i + 1] = bitmapData[j + 1];
				temp[i + 2] = bitmapData[j + 2];
			}

		{
			std::size_t i = 0;
			std::size_t j = 0;
			while (i < (bitmapInfoHeader->biSizeImage) && j + 2 < dataSize && i + 3 < dataSize) {
				temp[i] = bitmapData[j];
				temp[i + 1] = bitmapData[j + 1];
				temp[i + 2] = bitmapData[j + 2];
				i += 4;
				j += 3;
			}
		}
	



			ZeroMemory(bitmapData, sizeof(std::uint8_t) * dataSize);

			memcpy(bitmapData, temp, sizeof(std::uint8_t) * dataSize);

			for (std::size_t i = 0; i < dataSize ; i++) { // (bitmapInfoHeader->biSizeImage * 2); i++) {
			//	bitmapData[i] = temp[i];

			}
			delete[] temp;
		}
		fclose(file);


			

	}

	

	return bitmapData;
}

#endif


#endif // _MY_BITMAP_FILE_H_


std::shared_ptr<std::uint8_t> LoadBitmapFile(const char* filename, BITMAPINFOHEADER * pBitmapInfoHeader) {
	FILE* pFile;
	BITMAPFILEHEADER bitmapFileHeader;
	std::shared_ptr<std::uint8_t> bitmapImage;

	errno_t err = fopen_s(&pFile, filename, "rb");
	if (err != 0 || pFile == nullptr) {
		return nullptr;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
	if (bitmapFileHeader.bfType != 0x4D42) { // 'BM' signature in little-endian
		fclose(pFile);
		return nullptr;
	}

	fread(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	fseek(pFile, static_cast<long>(bitmapFileHeader.bfOffBits), SEEK_SET);

	const auto ImageSize = pBitmapInfoHeader->biSizeImage * 2;
	bitmapImage = std::shared_ptr<std::uint8_t>(new(std::nothrow) std::uint8_t[ImageSize], std::default_delete<std::uint8_t[]>());
	if (bitmapImage == nullptr) {
		fclose(pFile);
		return nullptr;
	}

	memset(bitmapImage.get(), 0, sizeof(std::uint8_t) * ImageSize);
	fread(bitmapImage.get(), pBitmapInfoHeader->biSizeImage, 1, pFile);
	if (bitmapImage == nullptr) {
		fclose(pFile);
		return nullptr;
	}

	std::shared_ptr<std::uint8_t> temp(new(std::nothrow) std::uint8_t[ImageSize], std::default_delete<std::uint8_t[]>());
	if (temp == nullptr) {
		fclose(pFile);
		return nullptr;
	}

	memset(temp.get(), 0, sizeof(std::uint8_t) * ImageSize);

	for (std::size_t i = 0, j = 0; i < (ImageSize / 2) && j + 2 < ImageSize && i + 3 < ImageSize; i += 4, j += 3) {
		temp.get()[i] = bitmapImage.get()[j];
		temp.get()[i + 1] = bitmapImage.get()[j + 1];
		temp.get()[i + 2] = bitmapImage.get()[j + 2];
	}

	memcpy(bitmapImage.get(), temp.get(), sizeof(std::uint8_t) * ImageSize);
	fclose(pFile);

	return bitmapImage;
}

