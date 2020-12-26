#pragma once
#include "framework.h"
  #include "share.h"
#pragma comment(lib, "WinMM.lib")

#include <commdlg.h> // for LPCFHOOKPROC

class Plasma;
class Scratch;










#define BUF_SIZE 1024


	class CustomRunner 
	{
	
	public:
		CustomRunner();
		~CustomRunner();
		
		HFONT  hfont = NULL;
		HDC memBackSurface = NULL;


		int rfCntHdc;

		int wmId;
		int wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;
		LRESULT Result;
		HWND hWnd;
		HDC hdDesktopSpook;
		
		win32_offscreen_buffer Buffer;
		
		win32_offscreen_buffer TextureBuffer[4];
		WCHAR wbuff[BUF_SIZE];

		
		Plasma*  pplasma;
		Scratch* scratch;
	private:
		//int BytesPerPixel;  // = 4;
	public:

		void CreateAFont(HDC& hdc_param);


		void SafeReleaseDC();

		HDC SafeGetDC();

		void Win32ResizeDibSection(uint32 Width, uint32 Height);

		void Win32UpdateWindow(HDC &hdc, uint32& WindowWidth, uint32& WindowHeight);

		void Win32UpdateWindow();

		HWND myPaint();

		HWND myPaint(HWND hWnd);

		void ClearBuffer();

		void Render();


		void TestSomeMouse();


		void TestSomeGDIProcedure(HDC& hdc_param, uint32& WindowWidth, uint32& WindowHeight);

		void TestSaveFeature(HDC hdc_param);

		BOOL mSaveBitmap(LPWSTR wPath, HWND hWnd, HDC hdc);



		void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr);

		void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, int x, int y, COLORREF bgr);

		void PutPixel(win32_offscreen_buffer& Destination, uint32_t x, uint32_t y, COLORREF bgr);

		// RGB(r, g, b);
		void mSetPixel(uint32_t x, uint32_t y, COLORREF color);

		// http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
		void Line(float x1, float y1, float x2, float y2,  const COLORREF& color);

		void WuDrawLine(void* bm, uint32_t bw, uint32_t bh, float x0, float y0, float x1, float y1);

		void DrawRect(void* bm, uint32_t bw, uint32_t bh);

		void DrawRect();

		void Foo();

		//  float result = (255.0f-alpha)*value1+alpha*value2; return (int)result;


		void PutPixelBackOrder(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF gbr);

		COLORREF mGetPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y);

		COLORREF mGetPixel(win32_offscreen_buffer& sourceBuffer, uint32_t x, uint32_t y);

		void AnotherWrap(CustomRunner& custRun);

		void CopyImage(win32_offscreen_buffer& destBuffer, win32_offscreen_buffer& sourceBuffer);

		void FlipHorizontal(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source);

		void FlipVertical(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source);

	
		void Funn();


		COLORREF getColorAtCursor();

		POINT getLocalCursor();

		POINT getLocalCursor(HDC hdc);

		// Just a test
		//void PlasmaXXXX();

	};



