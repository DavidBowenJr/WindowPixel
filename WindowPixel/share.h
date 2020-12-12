#pragma once

struct win32_offscreen_buffer
{
	BITMAPINFO Info;
	void* Memory;
	uint32_t Width;
	uint32_t Height;
	uint32_t Pitch;
	uint32_t BytesPerPixel;

};
