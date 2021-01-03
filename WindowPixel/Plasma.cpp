
#include "CustomRunner.h"
#include "share.h"
#include "colorstruct.h"


#include "PartsOfSDL.h"

#include <mmsystem.h> // timeGetTime

#include "Plasma.h"


Plasma::Plasma(olved__buffer& olvedBuffer) {

#define plasma_screenWidth  256
#define plasma_screenHeight 256


	uint32* palette = new uint32[256];
	uint32** plasma = new uint32 * [plasma_screenHeight];
	for (uint32 i(0); i < plasma_screenHeight; i++) plasma[i] = new uint32[plasma_screenWidth];

	uint32** buffer = new uint32 * [plasma_screenHeight];
	for (uint32 i(0); i < plasma_screenHeight; i++) buffer[i] = new uint32[plasma_screenWidth];

this->olvedBuffer.buffer  =	olvedBuffer.buffer  = buffer;
this->olvedBuffer.palette =	olvedBuffer.palette = palette;
this->olvedBuffer.plasma  =	olvedBuffer.plasma  = plasma;
this->olvedBuffer.Width   =	olvedBuffer.Width   = plasma_screenWidth;
this->olvedBuffer.Height  =	olvedBuffer.Height  = plasma_screenHeight;

}

Plasma::Plasma() {
	OutputDebugString(TEXT("Plasma CC "));
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

void Plasma::SomeFunction5(HWND& rHWnd, win32_offscreen_buffer& rSurfdata, CustomRunner& rDraw )
{
#define plasma_screenWidth  256
#define plasma_screenHeight 256

	this->olvedBuffer.Width = this->olvedBuffer.Width;
	uint32* palette = olvedBuffer.palette;
	uint32** plasma = olvedBuffer.plasma;
	uint32** buffer = olvedBuffer.buffer;

	int h = olvedBuffer.Width;
	int w = olvedBuffer.Height;

#define TIME_WRAP_VALUE (~(DWORD)0)
	static DWORD start = timeGetTime();
	DWORD now; DWORD ticks;
	now = timeGetTime();
	if (now < start) { ticks = (TIME_WRAP_VALUE - start) + now; }
	else { ticks = (now - start); }

	bool doOnce = false;
	if (!doOnce)
	{
	//	doOnce = !doOnce;
		ColorRGB colorRGB;

		for (int x = 0; x < 256; x++)
		{
			colorRGB.r = int(128.0 + 128 * sin(3.1415 * x / 16.0));
			colorRGB.g = int(128.0 + 128 * sin(3.1415 * x / 128.0));
			colorRGB.b = 0;
			palette[x] = RGBtoINT(colorRGB);
		}

		// Generate the plasma once
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
			{
				int color = int(
					128.0 + (128.0 * sin(x / 16.0)) +
					128.0 + (128.0 * sin(y / 32.0)) +
					128.0 + (128.0 * sin(sqrt(double((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0))) / 8.0)) +
					128.0 + (128.0 * sin(sqrt(double(x * x + x * y)) / 8.0))) / 4;
				plasma[y][x] = color;
			}
	}

	// Once everything is generated, the main loop starts. There isn't much math involved anymore
	int paletteShift;

	// What Happens When we place in CustomRunner <---- Prehaps pass thought by paramters ....
	// Now it is passed by parameters.
	// CustomRunner draw;
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
				//draw.PutPixel(*surfdata, x, y, rgbref);
				//draw->PutPixel(draw->Buffer , x, y, rgbref);
			//	rDraw.PutPixel(rDraw.Buffer, x, y, rgbref);
				
				rDraw.PutPixel(rSurfdata, x, y, rgbref);
			}
	}

}

#if 0
void Plasma::SomeFunction5(HWND* pHWnd, win32_offscreen_buffer* surfdata)
{
#define plasma_screenWidth  256
#define plasma_screenHeight 256

	this->olvedBuffer.Width = this->olvedBuffer.Width;
	uint32*  palette = olvedBuffer.palette;
	uint32** plasma = olvedBuffer.plasma;
	uint32** buffer = olvedBuffer.buffer;

	int h = olvedBuffer.Width;
	int w = olvedBuffer.Height;

#define TIME_WRAP_VALUE (~(DWORD)0)
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

		// Generate the plasma once
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
			{
				int color = int(
					128.0 + (128.0 * sin(x / 16.0)) +
					128.0 + (128.0 * sin(y / 32.0)) +
					128.0 + (128.0 * sin(sqrt(double((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0))) / 8.0)) +
					128.0 + (128.0 * sin(sqrt(double(x * x + x * y)) / 8.0))) / 4;
				plasma[y][x] = color;
			}
	}

	// Once everything is generated, the main loop starts. There isn't much math involved anymore
	int paletteShift;

	// What Happens When we place in CustomRunner <---- Prehaps pass thought by paramters ....
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
				draw.PutPixel(*surfdata, x, y, rgbref);
			}
	}


}

#endif


#if 0
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

#endif

void Plasma::FxFireEffect(HWND& rHwnd, win32_offscreen_buffer& surfdata, CustomRunner& rDraw)
{


static	const uint32 sW = 640;
static	const uint32 sH = 128;
	int h = sH;
	int w = sW;

	//uint32* palette = new uint32[256];
	static uint32 palette[256];
	//uint32** buffer = new uint32* [h];
	//for (uint32 i(0); i < h; i++) buffer[i] = new uint32[w];

//	uint32** fire = new uint32* [h];
//	for (uint32 i(0); i < h; i++) fire[i] = new uint32[w];

	static uint32  buffer[sW][sH];
	static uint32    fire[sW][sH];



	//this->olvedBuffer.Width = this->olvedBuffer.Width;
	//uint32* palette = olvedBuffer.palette;
	//uint32** fire = olvedBuffer.plasma;
	//uint32** buffer =  olvedBuffer.buffer;



	ColorRGB color;

	static BOOLEAN doPart1 = TRUE;

	if(doPart1)
	{
		doPart1 = !doPart1;
	// fire buffer
	for (uint32 y = 0; y < h; y++)
		for (uint32 x = 0; x < w; x++)
			fire[y][x] = uint32(0);

	// generate the palette
	for (uint32 x = 0; x < uint32(256); x++)
	{
		BYTE Blue = min((uint8)255, (uint8)(x * 2));

		color = HSLtoRGB(ColorHSL((BYTE)x / 3, (BYTE)255, (BYTE)(unsigned char)Blue));

		// set the palett to the calculated RGB value
		palette[x] = RGBtoINT(color);
	}
}





	// Start the loop (one frame per loop)
//         static 
bool done = false;
	int iter = 0;
	while (!done)
	{
		done = !done;
		iter++;
		if (iter > 128) done = true;
		// timing: set to maximum 50 millseconds per frame = 20 fps
		static DWORD start = timeGetTime();
		DWORD time = 1;
		DWORD now; DWORD ticks;
		now = timeGetTime();
		if (now < start) { ticks = (TIME_WRAP_VALUE - start) + now; }
		else { ticks = (now - start); }
		time = ticks / 1000;//* 0.000001;


		// Randomize the bottom row of the fire buffer
		for (uint32 x = 0; x < w; x++) fire[h - 1][x] = abs(32768 + rand()) % 256;

		// do the fire calculations for every pixel, from top to bottom
		for (uint32 y = 0; y < h - 1; y++)
			for (uint32 x = 0; x < w; x++)
			{
				fire[y][x] = ((
					
					fire[(y + 1) % h][(x - 1 + w) % w] + 
					fire[(y + 1) % h][(x)         % w] + 
					fire[(y + 1) % h][(x + 1)     % w] + 
					fire[(y + 2) % h][(x)         % w]
					) * 32) / 129;
			}

		// set the drawing buffer to the fire buffer, using the palette color
		for (uint32 y = 0; y < h; y++)
			for (uint32 x = 0; x < w; x++)
			{
				buffer[y][x] = palette[fire[y][x]];
			}





		// draw the buffer and redraw the screen

		for (uint32 y = 0; y < h; y++)
			for (uint32 x = 0; x < w; x++)
			{
				int color = buffer[y][x];

#if 0
				// HardCore
				ColorRGB cRGB = INTtoRGB(color);
				COLORREF rgbref = RGB(cRGB.r, cRGB.g, cRGB.b);
				rDraw.PutPixel(surfdata, x, y, rgbref);
#endif

#if 1
				// Very nice overlay
				ColorRGB c = ColorRGB(BYTE(color), BYTE(color * 2), BYTE(255 - color));
				COLORREF rgb = RGB(BYTE(c.r), BYTE(c.g), BYTE(c.b));
				rDraw.PutPixel(surfdata, x, y, rgb);
#endif

				//ColorRGB c = ColorRGB(static_cast<BYTE>(color), static_cast<BYTE>(color * 2), static_cast<BYTE>(255 - color));
				//ColorRGB c = ColorRGB(static_cast<BYTE>(color), static_cast<BYTE>(color * 2), static_cast<BYTE>(255 - color));
				//rDraw.PutPixel(surfdata, x, y, color);
			}
	}
	/*
	for (uint32 i(0); i < h; i++) delete[] buffer[i];
	delete[] buffer;

	for (uint32 i(0); i < h; i++) delete[] fire[i];
	delete[] fire;

	delete[] palette;
	*/


}


void Plasma::Foo()
{
}
