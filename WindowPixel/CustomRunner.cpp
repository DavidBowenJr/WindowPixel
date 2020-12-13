#include "CustomRunner.h"
#include "Scratch.h"


CustomRunner::CustomRunner()
{
	Buffer.Memory = NULL;
	

	LRESULT Result = 0;
	wmId = 0;
	HDC hdc = NULL;
	HWND hWnd = NULL;
	Buffer.BytesPerPixel = 4;
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Result);
	UNREFERENCED_PARAMETER(hdc);

	scratch =  new Scratch();

	{
		BITMAPINFOHEADER bitmapInfoHeader;
		uint8_t* bitmapData;
		
		// Should add error handling here it pass so far.


		// There are ways to post build and get our image data into our debug or ext
		// but for now compile then place into the debug or release...
		// VERY SIMPLE TEST.....
		// Will Place else where but for now simple test.
		bitmapData = LoadBitmapFile( (char*)("photo3.bmp") , &bitmapInfoHeader);
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
		OutputDebugString( std::to_wstring(TextureBuffer[0].Width).c_str() );
			
		}

		// Our other texture memory surface ext.
		// free(GlobalTextureBuffer[1].Memory);
		// free(TextureBuffer[0].Memory);



	}



}

CustomRunner::~CustomRunner()
{

	


	if (scratch != nullptr)
	{
		delete scratch; scratch = nullptr;
	}


if (TextureBuffer[0].Memory != NULL)
	{
	free(TextureBuffer[0].Memory); 
	}

	/// <summary>
	/// buffer that will be sent to the stretch blit function.
	/// </summary>
	if(Buffer.Memory != NULL)
	if (VirtualFree(
		Buffer.Memory,       // Base address of block
		0,             // Bytes of committed pages  // Decommit the pages <MEM_RELEASE>
		MEM_RELEASE))
	{
		OutputDebugString(_T(" Virtual Free Bitmap MEMORY RELEASED"));
}
	
	//;  


	OutputDebugString(_T(" Class CustomRunner  ~ Called.\n"));
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






//void CustomRunner::Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer* Buffer)
void CustomRunner::Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer Buffer)
{
	

	if (Buffer.Memory == NULL) _ASSERTE(L"Bad");
	 
	int previousmode = SetStretchBltMode(hdc, MAXSTRETCHBLTMODE); // COLORONCOLOR
	int wh = SetICMMode(hdc, ICM_ON);
	if (wh == wh) {};
	if (previousmode == previousmode) {};
	StretchDIBits(hdc, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory,(const BITMAPINFO*) &Buffer.Info,(UINT) DIB_RGB_COLORS, (DWORD) SRCCOPY);
	
	

	this->Buffer.BytesPerPixel = Buffer.BytesPerPixel;
	this->Buffer.Height = Buffer.Height;
	memcpy( &this->Buffer.Info, &Buffer.Info, sizeof(BITMAPINFO));
	

	if (this->Buffer.Memory == NULL && Buffer.Memory != NULL)
	this->Buffer.Memory = Buffer.Memory;
	this->Buffer.Pitch = Buffer.Pitch;
	this->Buffer.Width = Buffer.Width;
	this->Buffer.Height = Buffer.Height;
	

}

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
    uint32_t WindowWidth  = this->ps.rcPaint.right - this->ps.rcPaint.left;
	uint32_t WindowHeight = this->ps.rcPaint.bottom - this->ps.rcPaint.top;

	//if(Buffer.Memory != NULL)
	this->Win32UpdateWindow(hDC, WindowWidth, WindowHeight, Buffer);
	
	
	//this->Win32UpdateWindow(hDC, &CR, x, y, Wt, Ht);

	EndPaint(hWnd, &this->ps);
	return hWnd;
}

void CustomRunner::ClearBuffer()
{
	//uint8_t* row = (uint8_t*)BitmapMemory; // get the first 8 bits from a 32 bit buffer in memory


}

void CustomRunner::Render()
{
	this->scratch->APP( *this);
}

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

	for( uint32_t j = 0; j < sourceBuffer.Width; j++)
		for ( uint32_t i = 0; i < sourceBuffer.Height; i++)
		{
			COLORREF color = CustomRunner::GetPixel(sourceBuffer.Memory, sourceBuffer.Width, sourceBuffer.Height, j, i);
			PutPixel(destBuffer.Memory, destBuffer.Width, destBuffer.Height, j, i, color);

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
	win32_offscreen_buffer *B = &TextureBuffer;
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
					uint8_t Blue =  (BYTE)(YOffset + X);
					uint8_t Green = (BYTE)(XOffset + YOffset + X + Y);
					uint8_t Red =   (BYTE)(XOffset + Y);
					*Pixel++ = (uint32_t)  (Red << 16) | (Green << 8) | (Blue) ;
				}
				Row += Pitch;
			}

}

void CustomRunner::PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr)
{
	if (BitmapMemory == NULL) return;

	if( y < BitmapHeight)  // && y > -1)
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






void CustomRunner::WuDrawLine(void* bm, uint32_t bw, uint32_t bh, float x0, float y0, float x1, float y1)
{
	 //this->DrawRect(bm, bw, bh);
	if (bm == NULL) return;

	for (uint32_t  i = (uint32_t) y0;  i < y0+y1; i++)
		for (uint32_t j = (uint32_t) x0; j < (uint32_t) x0 + x1; j++)
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

	for(uint32_t i = 0; i < Buffer.Width / 4; i++)
		for (uint32_t j = 0; j < Buffer.Height / 4; j++)
		{
			this->PutPixel(Buffer.Memory, Buffer.Width, Buffer.Height, i, j, (COLORREF)0XFFFF0000);
		}
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

COLORREF CustomRunner::GetPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y)
{
	if (BitmapMemory == NULL) return COLORREF(0);

	if( y < BitmapHeight) // && y > -1)
		if (x < BitmapWidth) // && x > -1)
		{
			uint32_t* p32b = (uint32_t*)BitmapMemory;
			p32b += (BitmapWidth * y) + x;
			unsigned char* p8b = (unsigned char*)p32b;
			return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
		}

	return COLORREF(0);
}




