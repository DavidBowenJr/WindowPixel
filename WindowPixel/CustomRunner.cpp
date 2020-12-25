

#include "CustomRunner.h"
#include "Scratch.h"
#include "Plasma.h"

#include "ImageSource.h"
#include "colorstruct.h"
#include <mmsystem.h> // timeGetTime

#include <chrono>
#include <thread>
#include <limits.h>
#include "SaveBitmap.h"

	CustomRunner::CustomRunner()
	{
		this->rfCntHdc = 0;

		OutputDebugString(TEXT("\n ..CR..Con.. \n"));

		Buffer.Memory = NULL;

		LRESULT Result = 0;
		
		wmId = 0;
		
		//HDC hdc = NULL;
		//HWND hWnd = NULL;
		
		Buffer.BytesPerPixel = 4;
		
		UNREFERENCED_PARAMETER(hWnd);
		UNREFERENCED_PARAMETER(Result);
		UNREFERENCED_PARAMETER(hdc);

		{
			BITMAPINFOHEADER bitmapInfoHeader;
			uint8_t* bitmapData;

			// Should add error handling here it pass so far.


			// There are ways to post build and get our image data into our debug or ext
			// but for now compile then place into the debug or release...
			// VERY SIMPLE TEST.....
			// Will Place else where but for now simple test.
			bitmapData = LoadBitmapFile((char*)("photo3.bmp"), &bitmapInfoHeader);
			bitmapInfoHeader.biBitCount = 32;
			memcpy(&TextureBuffer[0].Info, &bitmapInfoHeader, sizeof(BITMAPINFO));

			//TextureBuffer[0].Memory = bitmapData;

			// TextureBuffer[0].Memory = new uint8[sizeof(uint32) * (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight)];
			TextureBuffer[0].Memory = new uint32[bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight];
			// Or 


			// We know for now a component is 8 bits long... 4 components is 32 bits total...
			memcpy( TextureBuffer[0].Memory,  bitmapData, sizeof(uint32) * (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight));// bitmapInfoHeader.biSizeImage + (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight)); // bitmapData;
			SAFE_DELETE(bitmapData);

			TextureBuffer[0].Width = bitmapInfoHeader.biWidth;
			TextureBuffer[0].Height = bitmapInfoHeader.biHeight;
			TextureBuffer[0].BytesPerPixel = bitmapInfoHeader.biBitCount / 8;
			TextureBuffer[0].Pitch = bitmapInfoHeader.biWidth * (bitmapInfoHeader.biBitCount / 8);
			TextureBuffer[0].Pitch = TextureBuffer[0].Pitch;

			ZeroMemory(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

			if (TextureBuffer[0].Width > 0)
			{
				OutputDebugString(std::to_wstring(TextureBuffer[0].Width).c_str());
			}
		}
	}


	CustomRunner::~CustomRunner()
	{
		SafeReleaseDC();

		SAFE_DELETE(pplasma);
		SAFE_DELETE(scratch);

		//free(TextureBuffer[0].Memory);
		SAFE_DELETE(TextureBuffer[0].Memory);

		if (Buffer.Memory != NULL)
			if (VirtualFree(
				Buffer.Memory,       // Base address of block
				0,             // Bytes of committed pages  // Decommit the pages <MEM_RELEASE>
				MEM_RELEASE))
			{
				OutputDebugString(_T("\n Virtual Free Bitmap MEMORY RELEASED"));
			}
		OutputDebugString(_T("\n Class CustomRunner  ~ Called.\n"));
	}


	void CustomRunner::SafeReleaseDC()
	{
		rfCntHdc = rfCntHdc;
		if (this->rfCntHdc >= 1)
		{
			ReleaseDC(this->hWnd, this->hdc);
			rfCntHdc -= 1;
		}
	}


	HDC CustomRunner::SafeGetDC()
	{
		if (this->rfCntHdc == 0) // Good
		{
			rfCntHdc += 1;
			this->hdc = GetDC(hWnd);
			return this->hdc;
		}
		else
		{
			MessageBox(NULL, TEXT("NAG...!Potential Problem trying to lock an all ready lock Context. "), TEXT("Error"), MB_OK);
		}
		return HDC(this->hdc);
	}


	void CustomRunner::Win32ResizeDibSection(uint32 Width, uint32 Height)
	{
		if (Buffer.Memory)
		{
			VirtualFree(Buffer.Memory, 0, MEM_RELEASE);
		}

		Buffer.Width = Width;
		Buffer.Height = Height;
		Buffer.BytesPerPixel = 4; //?

		Buffer.Info.bmiHeader.biSize = sizeof(Buffer.Info.bmiHeader);
		Buffer.Info.bmiHeader.biWidth = (LONG)Buffer.Width;
		Buffer.Info.bmiHeader.biHeight = -(signed int)Buffer.Height;
		Buffer.Info.bmiHeader.biPlanes = 1;
		Buffer.Info.bmiHeader.biBitCount = 32;
		Buffer.Info.bmiHeader.biCompression = BI_RGB;

		SIZE_T BitmapMemorySize = (Buffer.Width * Buffer.Height) * Buffer.BytesPerPixel;
		Buffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
		Buffer.Pitch = Width * Buffer.BytesPerPixel;
	}



	void CustomRunner::Win32UpdateWindow(HDC hdc_param, uint32 WindowWidth, uint32 WindowHeight)
	{

// Being a little silly here but we prove that we can Create A compatiable device that will hold a HDC memory device handle that can be used.
		
		// this is not really the way it will be used directly in context .. but it may give a general idea. 
		// some of the steps used. some not important. 
		// Doing a simple accumulated dither StrectchDiBits
		// Also we see at the bottom  PatPlit who is using the hdc memimage

		static int dither = 0;



		if (GetDeviceCaps(hdc_param, RASTERCAPS))
		{

			if (Buffer.Memory == NULL) _ASSERT(L"Bad");
				int previousmode = SetStretchBltMode(hdc_param, MAXSTRETCHBLTMODE);
				int wh = SetICMMode(hdc_param, ICM_ON); if (wh == wh) {};
				if (previousmode == previousmode) {};

				//if (dither >= 2)
				{
					StretchDIBits(hdc_param, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory, (const BITMAPINFO*)&Buffer.Info, (UINT)DIB_RGB_COLORS, (DWORD)SRCCOPY);
				
					dither = 0;
				}
			//	dither++;

		
		}

#if 1
		// Text Yes....
		// Getting the GDC Text on top of our memory ...Draw ... text last.
		COLORREF old_fcolor, // old foreground text color
			old_bcolor; // old background text color
		int old_tmode; // old text transparency mode

	
		// set the foreground color to green and save old one
		old_fcolor = SetTextColor(hdc_param, RGB(0, 255, 0));

		// set the background color to black and save old one
		old_bcolor = SetBkColor(hdc_param, RGB(0, 0, 0));

		// finally set the transparency mode to transparent
		old_tmode = SetBkMode(hdc_param, TRANSPARENT);

		// draw some text at (20,30)
		TextOut(hdc_param, 20, 30, L"Hello",(int) strlen("Hello"));

		
		// now restore everything
		SetTextColor(hdc_param, old_fcolor);
		SetBkColor(hdc_param, old_bcolor);
		SetBkMode(hdc_param, old_tmode);
		
		// Ok with this line we can say our rect is hollow ext... somewhat vague but  theres lot's that can be done now.. much more... old gdi  windows. very nice.
	HBRUSH	hOldbrush2 = (HBRUSH)SelectObject(hdc_param, GetStockObject(HOLLOW_BRUSH));
	
	//https: //docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/2af5f7e4-9abb-4fb4-b428-d739c8b23829
	
//	HPEN white_dashed_pen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 255, 255));
//	SelectObject(hdc_param, white_dashed_pen);


		old_bcolor = SetBkMode(hdc_param,OPAQUE);
		SetBkColor(hdc_param, RGB(0,0,0));
		
	POINT p =	this->getLocalCursor(hdc_param);

		RoundRect(hdc_param, 1+p.x, 1+p.y, 220, 240, 15, 13);
	//	DeleteObject(white_dashed_pen);


	//	MoveToEx(hdc_param, 10, 10, NULL);
		//	LineTo(hdc_param, 50, 60);
			MoveToEx(hdc_param, 20, 10, NULL);
			LineTo(hdc_param, 30, 20);
			LineTo(hdc_param, 10, 20);
			LineTo(hdc_param, 20, 10);

			Ellipse(hdc_param, p.x +10, p.y + 10, p.x +30, p.y + 30);
			Ellipse(hdc_param, 250, 175, 350, 225);
			
			// create the polygon shown in the figure

			POINT p0 = { 10, 30 };
			POINT p1 = { 20,  0 };
			POINT p2 = { 30, 32 };
			POINT p3 = { 40, 31 };
			POINT p4 = { 35, 40 };
			POINT p5 = { 20, 40 };
			POINT p6 = { 15, 45 };

			HBRUSH	hOldbrush = (HBRUSH)SelectObject(hdc_param, GetStockObject(DKGRAY_BRUSH));
			POINT poly[7] = { p0.x, p0.y, p1.x, p1.y, p2.x, p2.y,
			p3.x, p3.y, p4.x, p4.y, p5.x, p5.y, p6.x, p6.y };
			// assume hdc is valid, and pen and brush are selected into
// graphics device context
			Polygon(hdc_param, poly, 7);




#endif

#if 1

			//https://stackoverflow.com/questions/3291167/how-can-i-take-a-screenshot-in-a-windows-application


			/// <summary>
			/// FOR NOW IM ALLOWING THIS TO RUN WILD BUT IN A REAL APPLICATION WE WOULD HAVE A STATE THAT WOULD ALLOW THIS CODE BLOCK TO RUN ONCE EACH KEY STROKE.
			/// </summary>
			/// <param name="hdc_param"></param>
			/// <param name="WindowWidth"></param>
			/// <param name="WindowHeight"></param>
			
			RECT Rect;
			if (GetClientRect(this->hWnd, &Rect))                    // (GetWindowRect(this->hWnd, &Rect))
			{
				int maxX = Rect.right - Rect.bottom;
				int maxY = Rect.bottom - Rect.top;
				HDC memdc = CreateCompatibleDC(hdc_param);
				HBITMAP hbit = CreateCompatibleBitmap(hdc_param, maxX, maxY);
				HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				HPEN hOldPen = (HPEN)SelectObject(memdc, hRedPen);
				SelectObject(memdc, hOldPen);
				if (SaveBitmap((LPWSTR)L"SavedBitmap.bmp", this->hWnd, hdc_param))
				{
				}
				ReleaseDC(this->hWnd, memdc);
			}
#endif




	}

	void CustomRunner::Win32UpdateWindow()
	{
		// Pace the program down a bit so we have better cache hits maybe.
		// otherwise my harddrive ramps up ....
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		{	
			SafeGetDC();
			RECT rt;
			GetClientRect( hWnd, &rt);
			uint32_t canvasWidth = rt.right - rt.left;
			uint32_t canvasHeight = rt.bottom - rt.top;
			if (this)
			{
				this->Win32UpdateWindow( hdc, canvasWidth, canvasHeight);
			}
			SafeReleaseDC();

		}

	}


	HWND CustomRunner::myPaint()
	{
	this->hWnd = this->myPaint(this->hWnd);

		return this->hWnd;
	}


	


	HWND CustomRunner::myPaint(HWND hWnd)
	{

// Very good read somewhat old but good ...

//	http://www.yaldex.com/games-programming/0672323699_ch03lev1sec3.html


		// BOOL ValidateRect(HWND hWnd; // handle of window
	    // CONST RECT *lpRect); // address of validation rectangle coordinates

		// handle of window along with a region you want to be validated in lpRect.
		// PAINTSTRUCT ps; // used in WM_PAINT
		// HDC hdc; // handle to a device context
		// RECT rect; // rectangle of window
#if 0
		SafeReleaseDC();
		this->hWnd = hWnd;
		this->hdc = SafeGetDC();
		this->SafeReleaseDC();
		RECT rt;
		GetClientRect(hWnd, &rt);
		this->Win32UpdateWindow();
		ValidateRect(hWnd, &rt);
#endif

#if 1
		SafeReleaseDC();
		this->hWnd = hWnd;
		// invalidate the entire window
		InvalidateRect(hWnd, NULL, FALSE);
		this->hdc = BeginPaint(hWnd, &this->ps);
		this->Win32UpdateWindow();
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
#endif


		// This is good also  but testing some theory from Yaldex...
#if 0
		SafeReleaseDC();
		
		RECT CR;
		GetClientRect(hWnd, &CR);
		this->hWnd = hWnd;
		
		this->hdc = BeginPaint(hWnd, &this->ps);              

		this->Win32UpdateWindow();

		EndPaint(hWnd, &this->ps);
		ReleaseDC(hWnd, hdc);
#endif

		return hWnd;
	}


	void CustomRunner::ClearBuffer()
	{
		//uint8_t* row = (uint8_t*)BitmapMemory; // get the first 8 bits from a 32 bit buffer in memory


	}


	void CustomRunner::Render() { this->scratch->APP(*this); }


#ifdef STRANGE_RENDER

	/* more or less from HMH but it's very usefull */
	void CustomRunner::RenderWeirdGradient(uint32_t XOffset, uint32_t YOffset)
	{
		if (Buffer.Memory == NULL) return;

		uint32_t w = Buffer.Width;
		uint32_t h = Buffer.Height;
		UNREFERENCED_PARAMETER(h);

		uint32_t Pitch = w * Buffer.BytesPerPixel;
		uint8* Row = (uint8*)Buffer.Memory;
		for (uint32_t Y = 0; Y < Buffer.Height; ++Y)
		{
			uint32* Pixel = (uint32*)Row;
			for (uint32_t X = 0; X < Buffer.Width; ++X)
			{
				uint8 Blue = (BYTE)(YOffset + X);
				uint8 Green = (BYTE)(XOffset + YOffset + X + Y);
				uint8 Red = (BYTE)(XOffset + Y);
				*Pixel++ = (uint32_t)((Red << 16) | (Green << 8) | Blue);
			}
			Row += Pitch;
		}
	}

#endif



	/// <summary>
	/// Serves no particular purpose just a test.
	/// </summary>
	/// <param name="custRun"></param>
	void CustomRunner::AnotherWrap(CustomRunner& custRun)
	{
		/*
		win32_offscreen_buffer* Bf = &custRun.Buffer;
		win32_offscreen_buffer* Gd = &custRun.TextureBuffer[0];

		 RenderWeirdGradient(0, 0,  *Bf, *Gd);
		 */

		 /*
		 win32_offscreen_buffer Bf = custRun.Buffer;
		 win32_offscreen_buffer Gd = custRun.TextureBuffer[0];

		 RenderWeirdGradient(0, 0, Bf, Gd);
		 */

#ifdef STRANGE_RENDER
		RenderWeirdGradient(0, 0, custRun.Buffer, custRun.TextureBuffer[0]);
		
#endif
		Funn();


	}

	/// <summary>
	///  Just a test more param needed PutPixel keeps us in bounds.
	/// </summary>
	/// <param name="destBuffer"></param>
	/// <param name="sourceBuffer"></param>
	void CustomRunner::CopyImage(win32_offscreen_buffer& destBuffer, win32_offscreen_buffer& sourceBuffer)
	{

		for (uint32 j = 0; j < sourceBuffer.Width; j++)
			for (uint32 i = 0; i < sourceBuffer.Height; i++)
			{
				//	COLORREF color = CustomRunner::GetPixel(sourceBuffer.Memory, sourceBuffer.Width, sourceBuffer.Height, j, i);
					// PutPixel(destBuffer.Memory, destBuffer.Width, destBuffer.Height, j, i, color);
				COLORREF color = CustomRunner::mGetPixel(this->Buffer, j, i);
				PutPixel(this->Buffer, j, i, color);
			}
	}

	void CustomRunner::FlipHorizontal(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source)
	{
		uint32 sy = 0; // y and pitch for Source
		uint32 dy = 0; // y and pitch for Destination

		uint32* gps = (uint32*)Source.Memory; // Base Address  for our source memory region
		uint32* gpd = (uint32*)Destination.Memory; // Base Address for our destination memory region

		for (uint32_t Y = 0; Y < Source.Height; Y++)
		{
			// Compute Row...
			sy = (Source.Width * Y);
			dy = (Destination.Width * Y);
			for (uint32 X = 0; X < Source.Width; X++)
			{
				// Compute Row Times Column
				// On the Source we would like to read from it's end to it's start to flip horizontal
				uint32* ps = gps;
				ps += (sy + (Source.Width - X));

				// Now calculate the destination in forward order
				uint32* pd = gpd;
				pd += dy + X;

				// We have our address at the new possition we dereference our pointers.
				// Pass by value. from our souce cell to our destination cell
				*pd = *ps;
			}
		}
	}

	void CustomRunner::FlipVertical(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source)
	{
		uint32 sy = 0;
		uint32 dy = 0;
		uint32* gps = (uint32*)Source.Memory;
		uint32* gpd = (uint32*)Destination.Memory;

		for (uint32 Y = 0; Y < Source.Height; ++Y)
		{
			sy = (Source.Width * (Source.Height - Y));
			dy = (Destination.Width * Y);
			for (uint32 X = 0; X < Source.Width; ++X)
			{
				uint32* ps = gps;
				uint32* pd = gpd;
				ps += (sy + X);
				pd += (dy + X);
				*pd = *ps;
			}
		}
	}



	void CustomRunner::Funn()
	{
		static int inc = 0;
		for(unsigned int y = 0; y < Buffer.Height; y++)
			for (unsigned int x = 0; x < Buffer.Width; x++)
			{
				COLORREF color = RGB( (BYTE) y ,(BYTE) x  , (BYTE)  x + y );
				//COLORREF color = RGB((BYTE)x, (BYTE)x + y, (BYTE)y);
				mSetPixel(x, y, color);
				inc++;
			}
	}


#ifdef STRANGE_RENDER
	// This is just a test
	void CustomRunner::RenderWeirdGradient(uint32_t XOffset, uint32_t YOffset, win32_offscreen_buffer& TextureBuffer, win32_offscreen_buffer& GardBuffer)
	{
		if (TextureBuffer.Memory == NULL) return;
		win32_offscreen_buffer* B = &TextureBuffer;
		uint32_t w = B->Width;
		uint32_t h = B->Height;

		UNREFERENCED_PARAMETER(GardBuffer);

		uint32_t Pitch = w * B->BytesPerPixel;
		uint8_t* Row = (uint8_t*)B->Memory;
		for (uint32_t Y = 0; Y < B->Height; ++Y)
		{
			uint32* Pixel = (uint32*)Row;
			for (uint32_t X = 0; X < B->Width; ++X)
			{
				uint8_t Blue = (BYTE)(YOffset + X);
				uint8_t Green = (BYTE)(XOffset + YOffset + X + Y);
				uint8_t Red = (BYTE)(XOffset + Y);
				*Pixel++ = (uint32_t)(Red << 16) | (Green << 8) | (Blue);
			}
			Row += Pitch;
		}
	}
#endif




	void CustomRunner::PutPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y, COLORREF bgr)
	{
		if (BitmapMemory == NULL) return;

		if (y < BitmapHeight)  // && y > -1)
			if (x < BitmapWidth) // && x > -1)
			{
				unsigned int* p32b = (unsigned int*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)&bgr;
				*p32b = (unsigned int)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::PutPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, int x, int y, COLORREF bgr)
	{
		if (BitmapMemory == NULL) return;

		if (y < (signed int)BitmapHeight && y > -1)
			if (x < (signed int)BitmapWidth && x > -1)
			{
				uint32_t* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)&bgr;
				*p32b = (uint32)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::PutPixel(win32_offscreen_buffer& Destination, uint32 x, uint32 y, COLORREF bgr)
	{
		if (Destination.Memory == NULL) return;

		if (y < Destination.Height)
			if (x < Destination.Width)
			{
				uint32* p32b = (uint32*)Destination.Memory;
				p32b += (Destination.Width * y) + x;
				uint8* p8b = (uint8*)&bgr;
				*p32b = (uint32)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::mSetPixel(uint32 x, uint32 y, COLORREF color)
	{
		this->PutPixel(this->Buffer, x, y, color);
	}

	// Rosettacode.org line. Bresenham's line
	void CustomRunner::Line(float x1, float y1, float x2, float y2,  const COLORREF& color)
	{
		// Bresenham's line algorithm
		const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
		if (steep)
		{
			std::swap(x1, y1);
			std::swap(x2, y2);
		}

		if (x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float dx = x2 - x1;
		const float dy = fabs(y2 - y1);

		float error = dx / 2.0f;
		const int ystep = (y1 < y2) ? 1 : -1;
		int y = (int)y1;

		const int maxX = (int)x2;

		for (int x = (int)x1; x <= maxX; x++)
		{
			if (steep)
			{
				mSetPixel(y, x, color);
			}
			else
			{
				mSetPixel(x, y, color);
			}

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}

	}

	void CustomRunner::WuDrawLine(void* bm, uint32 bw, uint32 bh, float x0, float y0, float x1, float y1)
	{
		//this->DrawRect(bm, bw, bh);
		if (bm == NULL) return;

		for (uint32 i = (uint32)y0; i < y0 + y1; i++)
			for (uint32 j = (uint32)x0; j < (uint32)x0 + x1; j++)
				this->PutPixel(bm, bw, bh, j, i, (0xFFFFFFFF));
	}

	void CustomRunner::DrawRect(void* bm, uint32 bw, uint32 bh)
	{
		if (bm == NULL) return;
		for (uint32 i = 0; i < bw / 4; i++)
			for (uint32 j = 0; j < bh / 4; j++)
			{
				this->PutPixel(bm, bw, bh, i, j, (COLORREF)0XFFFF0000);
			}
	}

	void CustomRunner::DrawRect()
	{
		if (this->Buffer.Memory == NULL) return;
		for (uint32 i = 0; i < Buffer.Width / 4; i++)
			for (uint32 j = 0; j < Buffer.Height / 4; j++)
			{
				this->PutPixel(this->Buffer, i, j, (COLORREF)0Xffff0000);
			}
	}

	void CustomRunner::Foo()
	{
		uint32_t w = this->Buffer.Width;
		uint32_t h = this->Buffer.Height;
#if 0
		for(int y = 0; y < h; y++)
			for (int x = 256; x < w; x++)
			{
				// int color = int(128.0 + (128.0 * sin(x / 8.0)));
				// int color = int(128.0 + (128.0 * sin((x + y) / 8.0)));
				 int color = int(128.0 + (128.0 * sin(sqrt((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0)) / 8.0)));
			//	int color = int(128.0 + (128.0 * sin(x / 8.0))+ 128.0 + (128.0 * sin(y / 8.0))) / 2;
				
				SetPixel(x, y, RGB(color, color, color));
			}
#endif

#if 1

	//	https://lodev.org/cgtutor/plasma.html

#define screenWidth  256
#define screenHeight 256

		w = screenWidth;
		h = screenHeight;



		// Y-ccordinate first because we use horizontal scanlines
static		uint32 xplasma[screenHeight][screenWidth];
static		uint32 xlbuffer[screenHeight][screenWidth];
static		uint32 xpalette[256];

// generate the palette
ColorRGB colorRGB;
for (int x = 0; x < 256; x++)
{
	//use HSVtoRGB to vary the Hue of the color through the palette
	colorRGB = HSVtoRGB(ColorHSV( static_cast<uint8>(x), 255, 255));
	xpalette[x] = RGBtoINT(colorRGB);
}

// generate the plasma once
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		
		// the plasma buffer is a sum of sines
		int color = int( 128.0 + (128.0 * sin(static_cast<int>(x) / 16.0)) + 128.0 + (128.0 * sin(static_cast<int>(y) / 16.0)) ) / 2;
		xplasma[y][x] = (uint32) color;
	}

int paletteShift;

//start the animation loop, it rotates the palette
// the parameter to shif the palette varies with time
paletteShift = int( timeGetTime() / 10.0);

//draw every pixel again with the shifted palette color
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		xlbuffer[y][x] = xpalette[(xplasma[y][x] + paletteShift) % 256];
	}

//make everything visible
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		mSetPixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y),(COLORREF) xlbuffer[y][x]);
	}



#endif



	}

	void CustomRunner::PutPixelBackOrder(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y, COLORREF gbr)
	{
		if (BitmapMemory == NULL) return;
		UNREFERENCED_PARAMETER(gbr);
		if (y < BitmapHeight)
			if (x < BitmapWidth)
			{
				uint32* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				BYTE r = GetRValue(*p32b);
				BYTE g = GetGValue(*p32b);
				BYTE b = GetBValue(*p32b);
				*p32b = RGB(b, g, r);
				*p32b = ~*p32b;
			}
	}

	COLORREF CustomRunner::mGetPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y)
	{
		if (BitmapMemory == NULL) return COLORREF(0);
		if (y < BitmapHeight)
			if (x < BitmapWidth) 
			{
				uint32* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)p32b;
				return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
		return COLORREF(0);
	}

	COLORREF CustomRunner::mGetPixel(win32_offscreen_buffer& sourceBuffer, uint32 x, uint32 y)
	{
		if (sourceBuffer.Memory == NULL) return COLORREF(0);
		if (y < sourceBuffer.Height)
			if (x < sourceBuffer.Width)
			{
				uint32* p32b = (uint32*)sourceBuffer.Memory;
				p32b += (sourceBuffer.Width * y) + x;
				uint8b* p8b = (uint8b*)p32b;
				return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
		return COLORREF(0);
	}

	/////////////////////////////////////////////////////////////////////////

	// Had to change My GetPixel to mGetPixel as to not conflict with Windows.
	COLORREF CustomRunner::getColorAtCursor(void) {
		POINT p;
		COLORREF color;
		HDC hDC;
		BOOL b;

		/* Get the device context for the screen */
		hDC = GetDC(NULL);
		if (hDC == NULL) return CLR_INVALID;

		/* Get the current cursor position */
		b = GetCursorPos(&p);
		if (!b)
			return CLR_INVALID;

		/* Retrieve the color at that position */
		color = GetPixel(hDC, p.x, p.y);

		/* Release the device context again */
		ReleaseDC(GetDesktopWindow(), hDC);

		return color;
	}

	POINT CustomRunner::getLocalCursor(void) {
		POINT p;
		HDC hdc;
		hdc = GetDC(this->hWnd);
		if (hdc == NULL) return { ~0,~0 }; 
		BOOL b = GetCursorPos(&p);
		if (!b) return { ~0,~0 };
		return p;
	}

	POINT CustomRunner::getLocalCursor(HDC hdc)
	{
		POINT p;
		GetCursorPos(&p);
		return p;
	}


	// Just a test
	//void CustomRunner::PlasmaXXXX() { this->pplasma->SomeFunction5(this->hWnd, this->Buffer, *this); }

