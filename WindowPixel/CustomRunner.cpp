#include "CustomRunner.h"
#include "Scratch.h"


CustomRunner::CustomRunner()
{
	LRESULT Result = 0;
	wmId = 0;
	HDC hdc = NULL;
	HWND hWnd = NULL;
	BytesPerPixel = 4;
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Result);
	UNREFERENCED_PARAMETER(hdc);

	scratch =  new Scratch();

}

CustomRunner::~CustomRunner()
{

	if (scratch != nullptr)
	{
		delete scratch; scratch = nullptr;
	}

	if (VirtualFree(
		BitmapMemory,       // Base address of block
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
	if (BitmapMemory)
	{
		VirtualFree(BitmapMemory, 0, MEM_RELEASE);
	}

	BitmapWidth = Width;
	BitmapHeight = Height;

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = (LONG)BitmapWidth;
	BitmapInfo.bmiHeader.biHeight = -(signed int)BitmapHeight;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	SIZE_T BitmapMemorySize = (BitmapWidth * BitmapHeight) * BytesPerPixel;
	BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void CustomRunner::Win32UpdateWindow(HDC hdc, RECT* cR, int x, int y, int Width, int Height)
{
	UNREFERENCED_PARAMETER(Height);
	UNREFERENCED_PARAMETER(Width);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);

	/* get screen coordinates */
	// int maxX = GetSystemMetrics(SM_CXSCREEN);
	// int maxY = GetSystemMetrics(SM_CYSCREEN);

	bool bSetMappingMode = false;
	if (bSetMappingMode)
	{
		/* set mapping mode, window and viewport extents */
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

HWND CustomRunner::myPaint(HWND hWnd)
{
	RECT CR;
	GetClientRect(hWnd, &CR);
	// PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &this->ps);

	int x = this->ps.rcPaint.left;
	int y = this->ps.rcPaint.top;
	int Ht = this->ps.rcPaint.bottom - this->ps.rcPaint.top;
	int Wt = this->ps.rcPaint.right - this->ps.rcPaint.left;
	this->Win32UpdateWindow(hDC, &CR, x, y, Wt, Ht);
	EndPaint(hWnd, &this->ps);
	return hWnd;
}

void CustomRunner::ClearBuffer()
{
	uint8_t* row = (uint8_t*)BitmapMemory; // get the first 8 bits from a 32 bit buffer in memory


}

void CustomRunner::Render()
{
	this->scratch->APP( *this);
}

/* more or less from HMH but it's very usefull */
void CustomRunner::RenderWeirdGradient(uint32_t XOffset, uint32_t YOffset)
{
	uint32_t w = BitmapWidth;
	uint32_t h = BitmapHeight;
	UNREFERENCED_PARAMETER(h);

	uint32_t Pitch = w * BytesPerPixel;
	uint8* Row = (uint8*)BitmapMemory;
	for (uint32_t Y = 0; Y < BitmapHeight; ++Y)
	{
		uint32* Pixel = (uint32*)Row;
		for (uint32_t X = 0; X < BitmapWidth; ++X)
		{
			uint8 Blue = (BYTE)(YOffset + X);
			uint8 Green = (BYTE)(XOffset + YOffset + X + Y);
			uint8 Red = (BYTE)(XOffset + Y);
			*Pixel++ = (uint32_t)((Red << 16) | (Green << 8) | Blue);
		}
		Row += Pitch;
	}
}

void CustomRunner::PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr)
{
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
	if (y < (signed int)BitmapHeight && y > -1)
		if (x < (signed int)BitmapWidth && x > -1)
		{
			uint32_t* p32b = (uint32_t*)BitmapMemory;
			p32b += (BitmapWidth * y) + x;
			unsigned char* p8b = (unsigned char*)&bgr;
			*p32b = (uint32_t)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
		}
}


void CustomRunner::DrawRect()
{
	for(uint32_t i = 0; i < BitmapWidth / 4; i++)
		for (uint32_t j = 0; j < BitmapHeight / 4; j++)
		{
			this->PutPixel(BitmapMemory, BitmapWidth, BitmapHeight, i, j, (COLORREF)0XFFFF0000);
		}
}

void CustomRunner::PutPixelBackOrder(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF gbr)
{
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
