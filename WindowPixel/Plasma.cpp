
#include "CustomRunner.h"
#include "share.h"
#include "colorstruct.h"


#include "PartsOfSDL.h"

#include <mmsystem.h> // timeGetTime

#include "Plasma.h"

 // for our timeGetTime

//static SDL_Surface* scr;


Plasma::Plasma() {
	OutputDebugString(TEXT("Plasma CC "));



}

void Plasma::BuildMem()
{
	
}

void Plasma::ReleaseMem()
{

}




Plasma::Plasma(SDL_Surface* Scr) : scr(Scr) {
	OutputDebugString(TEXT("Plasma CC<------------------>Initializer list "));
};


Plasma::~Plasma()
{
	OutputDebugString(TEXT("Plasma De Con ........................................... "));


	//ReleaseMem();

}

// Any way sorta got this going from LODEV
// But I need to move the allocator outside somewhres and pass it in through parameters 
// that would include the destruction of the memory...
// OtherWise it call a constructor and deconstructor every time...
// Not sure how to go abouth this problem.... 
// Other than that..... Has been modifyed just a touch.
void Plasma::SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata)
{
 #define plasma_screenWidth  256
 #define plasma_screenHeight 256


	
	{
		uint32* palette = new uint32[256];
		uint32** plasma = new uint32 * [plasma_screenHeight];
		for (uint32 i(0); i < plasma_screenHeight; i++) plasma[i] = new uint32[plasma_screenWidth];

		uint32** buffer = new uint32 * [plasma_screenHeight];
		for (uint32 i(0); i < plasma_screenHeight; i++) buffer[i] = new uint32[plasma_screenWidth];


		int h = 256;
		int w = 256;

#define TIME_WRAP_VALUE (~(DWORD)0)

		// The first (low-resolution) ticks value of the applicaiton
		static DWORD start = timeGetTime();
		DWORD now; DWORD ticks;
		now = timeGetTime();
		if (now < start) { ticks = (TIME_WRAP_VALUE - start) + now; }
		else { ticks = (now - start); }

		bool doOnce = false;
		if (!doOnce)
		{
			doOnce = !doOnce;
			ColorRGB colorRGB;

			for (int x = 0; x < 256; x++)
			{
				colorRGB.r = int(128.0 + 128 * sin(3.1415 * x / 16.0));
				colorRGB.g = int(128.0 + 128 * sin(3.1415 * x / 128.0));
				colorRGB.b = 0;
				palette[x] = RGBtoINT(colorRGB);
			}

			// generate the plasma once
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w; x++)
				{
					int color = int(
						128.0 + (128.0 * sin(x / 16.0))
						+ 128.0 + (128.0 * sin(y / 32.0))
						+ 128.0 + (128.0 * sin(sqrt(double((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0))) / 8.0))
						+ 128.0 + (128.0 * sin(sqrt(double(x * x + y * y)) / 8.0))
						) / 4;
					plasma[y][x] = color;
				}
		}

		// Once everything is generated, the main loop starts. There isn't much math involved anymore
		int paletteShift;

		CustomRunner draw;

		// start the animation loop, it rotates the palette
		{
			paletteShift = int(ticks * 10);

			// draw every pixel again, with the shifted palette color
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w; x++)
				{
					buffer[y][x] = palette[(plasma[y][x] + paletteShift) % 256];
				}

			// Draw the result buffer to the screen
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w; x++)
				{
					ColorRGB cRGB = INTtoRGB((int)buffer[y][x]);
					COLORREF rgbref = RGB(cRGB.r, cRGB.g, cRGB.b);
					draw.PutPixel(surfdata, x, y, rgbref);

				}
		}

		for (uint32 i(0); i < plasma_screenHeight; i++) delete[] buffer[i];
		delete[] buffer;

		for (uint32 i(0); i < plasma_screenHeight; i++) delete[] plasma[i];
		delete[] plasma;

		delete[] palette;

	} 

}

void Plasma::Foo()
{
}
