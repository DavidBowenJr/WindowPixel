
#include "CustomRunner.h"
#include "share.h"
#include "colorstruct.h"


#include "PartsOfSDL.h"

#include <mmsystem.h> // timeGetTime

#include "Plasma.h"

 // for our timeGetTime

//static SDL_Surface* scr;


Plasma::Plasma(olved__buffer& olvedBuffer) {

#define plasma_screenWidth  256
#define plasma_screenHeight 256


	uint32* palette = new uint32[256];
	uint32** plasma = new uint32 * [plasma_screenHeight];
	for (uint32 i(0); i < plasma_screenHeight; i++) plasma[i] = new uint32[plasma_screenWidth];

	uint32** buffer = new uint32 * [plasma_screenHeight];
	for (uint32 i(0); i < plasma_screenHeight; i++) buffer[i] = new uint32[plasma_screenWidth];

this->olvedBuffer.buffer  =	olvedBuffer.buffer = buffer;
this->olvedBuffer.palette =	olvedBuffer.palette = palette;
this->olvedBuffer.plasma  =	olvedBuffer.plasma = plasma;
this->olvedBuffer.Width   =	olvedBuffer.Width = plasma_screenWidth;
this->olvedBuffer.Height  =	olvedBuffer.Height = plasma_screenHeight;

}

Plasma::Plasma() {
	OutputDebugString(TEXT("Plasma CC "));
}

void Plasma::BuildMem()
{
	
}

void Plasma::ReleaseMem()
{

}


Plasma::~Plasma()
{
	OutputDebugString(TEXT("Plasma De Con ........................................... "));

	/*
	olvedBuffer.buffer;
	olvedBuffer.palette;
	olvedBuffer.plasma;
	olvedBuffer.Width;
	olvedBuffer.Height;
	*/

	for (uint32 i(0); i < plasma_screenHeight; i++) delete[] olvedBuffer.buffer[i];    //buffer[i];
	delete[]  olvedBuffer.buffer;                //buffer;

	for (uint32 i(0); i < plasma_screenHeight; i++) delete[] olvedBuffer.plasma[i];                     // plasma[i];
	delete[] olvedBuffer.plasma;                  //plasma;

	delete[] olvedBuffer.palette;                  ///palette;



	//ReleaseMem();

}

// Any way sorta got this going from LODEV
// Pass initalization of Plasma to  WM_CREATE 
//  // Inject into class CustomRunner
// .... customRunner->pplasma = new Plasma(plasmaBuffer);
// The Plasma class for now will fill out the  olved__buffer& olvedBuffer
// lodev ol ved  loved solved.
// And when the program terminates will Call Plasma deconstructor at the end of the program and do clean up
// olvedBuffer is somewhat probed for the moment ....

void Plasma::SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata)
{
 #define plasma_screenWidth  256
 #define plasma_screenHeight 256





	
	{
		/*
		uint32* palette = new uint32[256];
		uint32** plasma = new uint32 * [plasma_screenHeight];
		for (uint32 i(0); i < plasma_screenHeight; i++) plasma[i] = new uint32[plasma_screenWidth];

		uint32** buffer = new uint32 * [plasma_screenHeight];
		for (uint32 i(0); i < plasma_screenHeight; i++) buffer[i] = new uint32[plasma_screenWidth];
		*/

	this->olvedBuffer.Width =	this->olvedBuffer.Width;

    uint32*  palette =  olvedBuffer.palette;
	uint32** plasma  =  olvedBuffer.plasma;
	uint32** buffer  =	olvedBuffer.buffer;
		
		
	int h =	olvedBuffer.Width;
	int w =	olvedBuffer.Height;



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

		/*
		for (uint32 i(0); i < plasma_screenHeight; i++) delete[] buffer[i];
		delete[] buffer;

		for (uint32 i(0); i < plasma_screenHeight; i++) delete[] plasma[i];
		delete[] plasma;

		delete[] palette;
		*/
	} 

}

void Plasma::Foo()
{
}
