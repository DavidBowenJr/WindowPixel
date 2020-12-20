

#include "CustomRunner.h"
#include "Scratch.h"
#include "Plasma.h"

#include "ImageSource.h"
#include "colorstruct.h"
#include <mmsystem.h> // timeGetTime

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
			TextureBuffer[0].Memory = bitmapData;
			TextureBuffer[0].Width = bitmapInfoHeader.biWidth;
			TextureBuffer[0].Height = bitmapInfoHeader.biHeight;
			TextureBuffer[0].BytesPerPixel = bitmapInfoHeader.biBitCount / 8;
			TextureBuffer[0].Pitch = bitmapInfoHeader.biWidth * (bitmapInfoHeader.biBitCount / 8);
			TextureBuffer[0].Pitch = TextureBuffer[0].Pitch;

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
		SAFE_DELETE(TextureBuffer[0].Memory);

		/*
		if (this->pplasma != nullptr)
		{
			delete pplasma; pplasma = nullptr;
		}


		if (scratch != nullptr)
		{
			delete scratch; scratch = nullptr;
		}


		if (TextureBuffer[0].Memory != NULL)
		{
			free(TextureBuffer[0].Memory);
		}
		*/

		/// <summary>
		/// buffer that will be sent to the stretch blit function.
		/// </summary>
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
			// Maybe Consult us where this happens.

			//std::wstring wst; wst.append()
			MessageBox(NULL, TEXT("NAG...!Potential Problem trying to lock an all ready lock Context. ")
				, TEXT("Error"), MB_OK);
		}

		return HDC(this->hdc);
	}





	void CustomRunner::Win32ResizeDibSection(uint32_t Width, uint32_t Height)
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

	void CustomRunner::Win32UpdateWindow(HDC hdc_param, uint32_t WindowWidth, uint32_t WindowHeight)
	{
		if (Buffer.Memory == NULL) _ASSERT(L"Bad");
		int previousmode = SetStretchBltMode(hdc_param, MAXSTRETCHBLTMODE);
		int wh = SetICMMode(hdc_param, ICM_ON); if (wh == wh) {};
		if (previousmode == previousmode) {};
		StretchDIBits(hdc_param, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory, (const BITMAPINFO*)&Buffer.Info, (UINT)DIB_RGB_COLORS, (DWORD)SRCCOPY);

	}
	void CustomRunner::Win32UpdateWindow()
	{

		/*
		{
			 HDC ___dc = GetDC(hWnd);
			RECT rt;
			GetClientRect(hWnd, &rt);
			uint32_t canvasWidth = rt.right - rt.left;
			uint32_t canvasHeight = rt.bottom - rt.top;
			if (this)
			{
				this->Win32UpdateWindow(___dc, canvasWidth, canvasHeight);
			}
			ReleaseDC(hWnd, ___dc);
		}
		*/

		{
		
		//	SafeReleaseDC();
		//this->hdc =	
			SafeGetDC();
	
			RECT rt;
			GetClientRect( hWnd, &rt);
			uint32_t canvasWidth = rt.right - rt.left;
			uint32_t canvasHeight = rt.bottom - rt.top;
			if (this)
			{
				this->Win32UpdateWindow( hdc, canvasWidth, canvasHeight);
			}
			//ReleaseDC(___hWnd, ___dc);
			SafeReleaseDC();

		}





	}
	/*
	void CustomRunner::Win32UpdateWindow(HDC hdc_param, uint32_t WindowWidth, uint32_t WindowHeight)
	{
		if (Buffer.Memory == NULL) _ASSERT(L"Bad");
		int previousmode = SetStretchBltMode(hdc_param, MAXSTRETCHBLTMODE);
		int wh = SetICMMode(hdc_param, ICM_ON); if (wh == wh) {};
		if (previousmode == previousmode) {};
		StretchDIBits(hdc_param, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory, (const BITMAPINFO*)&Buffer.Info, (UINT)DIB_RGB_COLORS, (DWORD)SRCCOPY);

	}
	*/



#if 0
	void CustomRunner::Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer Buffer)
	{


		if (Buffer.Memory == NULL) _ASSERTE(L"Bad");

		int previousmode = SetStretchBltMode(hdc, MAXSTRETCHBLTMODE); // COLORONCOLOR
		int wh = SetICMMode(hdc, ICM_ON);
		if (wh == wh) {};
		if (previousmode == previousmode) {};
		StretchDIBits(hdc, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory, (const BITMAPINFO*)&Buffer.Info, (UINT)DIB_RGB_COLORS, (DWORD)SRCCOPY);



		this->Buffer.BytesPerPixel = Buffer.BytesPerPixel;
		this->Buffer.Height = Buffer.Height;
		memcpy(&this->Buffer.Info, &Buffer.Info, sizeof(BITMAPINFO));


		if (this->Buffer.Memory == NULL && Buffer.Memory != NULL)
			this->Buffer.Memory = Buffer.Memory;
		this->Buffer.Pitch = Buffer.Pitch;
		this->Buffer.Width = Buffer.Width;
		this->Buffer.Height = Buffer.Height;


	}
#endif


	/*
	void CustomRunner::Win32UpdateWindow(HDC hdc, RECT* cR, int x, int y, int Width, int Height)
	{
		UNREFERENCED_PARAMETER(Height);
		UNREFERENCED_PARAMETER(Width);
		UNREFERENCED_PARAMETER(x);
		UNREFERENCED_PARAMETER(y);

		// get screen coordinates
		// int maxX = GetSystemMetrics(SM_CXSCREEN);
		// int maxY = GetSystemMetrics(SM_CYSCREEN);

		bool bSetMappingMode = false;
		if (bSetMappingMode)
		{
			// set mapping mode, window and viewport extents
			// LOTS CAN BE DONE  HERE...

			 SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, 50, 50, NULL);
			SetViewportExtEx(hdc, 100, 100, NULL);
			SetViewportOrgEx(hdc, 50, 10, NULL);
		}

		LONG WinW = cR->right - cR->left;
		LONG WinH = cR->bottom - cR->top;
		StretchDIBits(hdc,
			0, 0,(int) BitmapWidth , (int) BitmapHeight,
			0, 0, (int)WinW, (int) WinH,
			BitmapMemory, &BitmapInfo,
			(UINT)DIB_RGB_COLORS, (DWORD)SRCCOPY);
	}
	*/
	// FIXME.
	HWND CustomRunner::myPaint(HWND hWnd)
	{
		RECT CR;
		GetClientRect(hWnd, &CR);

		this->hWnd = hWnd;

		// PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &this->ps);

		//int x = this->ps.rcPaint.left;
		//int y = this->ps.rcPaint.top;
		uint32_t WindowWidth = this->ps.rcPaint.right - this->ps.rcPaint.left;
		uint32_t WindowHeight = this->ps.rcPaint.bottom - this->ps.rcPaint.top;

		//if(Buffer.Memory != NULL)
		//Try to get contained.
		this->Win32UpdateWindow(hDC, WindowWidth, WindowHeight);
	//	this->Win32UpdateWindow(hDC, WindowWidth, WindowHeight, Buffer);


		//this->Win32UpdateWindow(hDC, &CR, x, y, Wt, Ht);

		EndPaint(hWnd, &this->ps);
		return hWnd;
	}

	void CustomRunner::ClearBuffer()
	{
		//uint8_t* row = (uint8_t*)BitmapMemory; // get the first 8 bits from a 32 bit buffer in memory


	}

	void CustomRunner::Render() { this->scratch->APP(*this); }

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
		RenderWeirdGradient(0, 0, custRun.Buffer, custRun.TextureBuffer[0]);
	}

	/// <summary>
	///  Just a test more param needed PutPixel keeps us in bounds.
	/// </summary>
	/// <param name="destBuffer"></param>
	/// <param name="sourceBuffer"></param>
	void CustomRunner::CopyImage(win32_offscreen_buffer& destBuffer, win32_offscreen_buffer& sourceBuffer)
	{

		for (uint32_t j = 0; j < sourceBuffer.Width; j++)
			for (uint32_t i = 0; i < sourceBuffer.Height; i++)
			{
				//	COLORREF color = CustomRunner::GetPixel(sourceBuffer.Memory, sourceBuffer.Width, sourceBuffer.Height, j, i);
					// PutPixel(destBuffer.Memory, destBuffer.Width, destBuffer.Height, j, i, color);
				COLORREF color = CustomRunner::mGetPixel(this->Buffer, j, i);
				PutPixel(this->Buffer, j, i, color);
			}
	}

	void CustomRunner::FlipHorizontal(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source)
	{
		uint32_t sy = 0; // y and pitch for Source
		uint32_t dy = 0; // y and pitch for Destination

		uint32_t* gps = (uint32*)Source.Memory; // Base Address  for our source memory region
		uint32_t* gpd = (uint32*)Destination.Memory; // Base Address for our destination memory region

		for (uint32_t Y = 0; Y < Source.Height; Y++)
		{
			// Compute Row...
			sy = (Source.Width * Y);
			dy = (Destination.Width * Y);
			for (uint32_t X = 0; X < Source.Width; X++)
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

		for (uint32_t Y = 0; Y < Source.Height; ++Y)
		{
			sy = (Source.Width * (Source.Height - Y));
			dy = (Destination.Width * Y);
			for (uint32_t X = 0; X < Source.Width; ++X)
			{
				uint32* ps = gps;
				uint32* pd = gpd;
				ps += (sy + X);
				pd += (dy + X);
				*pd = *ps;
			}
		}
	}

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

	void CustomRunner::PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr)
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

	void CustomRunner::PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, int x, int y, COLORREF bgr)
	{
		if (BitmapMemory == NULL) return;

		if (y < (signed int)BitmapHeight && y > -1)
			if (x < (signed int)BitmapWidth && x > -1)
			{
				uint32_t* p32b = (uint32_t*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)&bgr;
				*p32b = (uint32_t)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::PutPixel(win32_offscreen_buffer& Destination, uint32_t x, uint32_t y, COLORREF bgr)
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

	void CustomRunner::SetPixel(uint32_t x, uint32_t y, COLORREF color)
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
				SetPixel(y, x, color);
			}
			else
			{
				SetPixel(x, y, color);
			}

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}

	}

	void CustomRunner::WuDrawLine(void* bm, uint32_t bw, uint32_t bh, float x0, float y0, float x1, float y1)
	{
		//this->DrawRect(bm, bw, bh);
		if (bm == NULL) return;

		for (uint32_t i = (uint32_t)y0; i < y0 + y1; i++)
			for (uint32_t j = (uint32_t)x0; j < (uint32_t)x0 + x1; j++)
				this->PutPixel(bm, bw, bh, j, i, (0xFFFFFFFF));
	}

	void CustomRunner::DrawRect(void* bm, uint32_t bw, uint32_t bh)
	{
		if (bm == NULL) return;

		for (uint32_t i = 0; i < bw / 4; i++)
			for (uint32_t j = 0; j < bh / 4; j++)
			{
				this->PutPixel(bm, bw, bh, i, j, (COLORREF)0XFFFF0000);
			}
	}

	void CustomRunner::DrawRect()
	{
		if (this->Buffer.Memory == NULL) return;

		for (uint32_t i = 0; i < Buffer.Width / 4; i++)
			for (uint32_t j = 0; j < Buffer.Height / 4; j++)
			{
				//	this->PutPixel(Buffer.Memory, Buffer.Width, Buffer.Height, i, j, (COLORREF)0XFFFF0000);
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
		SetPixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y),(COLORREF) xlbuffer[y][x]);
	}



#endif



	}

	void CustomRunner::PutPixelBackOrder(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF gbr)
	{
		if (BitmapMemory == NULL) return;

		UNREFERENCED_PARAMETER(gbr);
		if (y < BitmapHeight) // && y > -1)
			if (x < BitmapWidth) // && x > -1)
			{
				uint32_t* p32b = (uint32_t*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				BYTE r = GetRValue(*p32b);
				BYTE g = GetGValue(*p32b);
				BYTE b = GetBValue(*p32b);
				*p32b = RGB(b, g, r);
				*p32b = ~*p32b;
			}

	}

	COLORREF CustomRunner::mGetPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y)
	{
		if (BitmapMemory == NULL) return COLORREF(0);

		if (y < BitmapHeight) // && y > -1)
			if (x < BitmapWidth) // && x > -1)
			{
				uint32_t* p32b = (uint32_t*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)p32b;
				return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}

		return COLORREF(0);
	}

	COLORREF CustomRunner::mGetPixel(win32_offscreen_buffer& sourceBuffer, uint32_t x, uint32_t y)
	{
		if (sourceBuffer.Memory == NULL) return COLORREF(0);

		if (y < sourceBuffer.Height)
			if (x < sourceBuffer.Width)
			{
				uint32_t* p32b = (uint32_t*)sourceBuffer.Memory;
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

	// Just a test
	//void CustomRunner::PlasmaXXXX() { this->pplasma->SomeFunction5(this->hWnd, this->Buffer, *this); }

