#pragma once
#include "framework.h"
#include <memory>
//uint8_t* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
std::shared_ptr<std::uint8_t> LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
