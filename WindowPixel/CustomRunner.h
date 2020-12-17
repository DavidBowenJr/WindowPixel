#pragma once
#include "framework.h"
#include "share.h"
#include "ImageSource.h"

class Scratch;
class Plasma;



	class CustomRunner
	{
	
	public:
		CustomRunner();
		~CustomRunner();
		
		int wmId;
		int wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;
		LRESULT Result;
		HWND hWnd;
		HDC hdDesktopSpook;
		
		win32_offscreen_buffer Buffer;
		
		win32_offscreen_buffer TextureBuffer[4];
		
		Plasma*  pplasma;
		Scratch* scratch;
	private:
		//int BytesPerPixel;  // = 4;
	public:

		//void  Win32ResizeDibSection(win32_offscreen_buffer* Buffer, uint32_t Width, uint32_t Height);
		void Win32ResizeDibSection(uint32_t Width, uint32_t Height);



	//	void Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer Buffer);


		void Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight);

#if 0
		void Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer Buffer);
#endif

		//void Win32UpdateWindow(HDC hdc, uint32_t WindowWidth, uint32_t WindowHeight, win32_offscreen_buffer* Buffer);


		// void Win32UpdateWindow(HDC hdc, RECT* cR, int x, int y, int Width, int Height);

		HWND myPaint(HWND hWnd);

		void ClearBuffer();

		void Render();

		void RenderWeirdGradient(uint32_t XOffset, uint32_t YOffset);

		// This is just a test
		void RenderWeirdGradient(uint32_t x, uint32_t y, win32_offscreen_buffer& TextureBuffer, win32_offscreen_buffer& GardBuffer);


		void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr);




		void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, int x, int y, COLORREF bgr);

		void PutPixel(win32_offscreen_buffer& Destination, uint32_t x, uint32_t y, COLORREF bgr);



		void WuDrawLine(void* bm, uint32_t bw, uint32_t bh, float x0, float y0, float x1, float y1);

		void DrawRect(void* bm, uint32_t bw, uint32_t bh);

		void DrawRect();

		void PutPixelBackOrder(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF gbr);

		COLORREF mGetPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y);

		COLORREF mGetPixel(win32_offscreen_buffer& sourceBuffer, uint32_t x, uint32_t y);




		void AnotherWrap(CustomRunner& custRun);


		void CopyImage(win32_offscreen_buffer& destBuffer, win32_offscreen_buffer& sourceBuffer);

		void FlipHorizontal(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source);

		void FlipVertical(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source);


		COLORREF getColorAtCursor();

		POINT getLocalCursor();




	};



