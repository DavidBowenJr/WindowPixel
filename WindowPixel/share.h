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


// Share with Plasma
struct olved__buffer
{
	uint32* palette;
	uint32** plasma;
	uint32** buffer;
	uint32_t Width;
	uint32_t Height;
};
