#pragma once
#include "framework.h"


class Scratch;

class CustomRunner
{

public:
	CustomRunner();
	~CustomRunner();


	int wmId;
	int wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	LRESULT Result; // = 0;
	HWND hWnd;

	void* BitmapMemory;
	BITMAPINFO BitmapInfo;
	uint32_t BitmapWidth;
	uint32_t BitmapHeight;

	Scratch* scratch;
private:
	int BytesPerPixel;  // = 4;
public:


	void Win32ResizeDibSection(uint32_t Width, uint32_t Height);

	void Win32UpdateWindow(HDC hdc, RECT* cR, int x, int y, int Width, int Height);

	HWND myPaint(HWND hWnd);

	void ClearBuffer();

	void Render();

	void RenderWeirdGradient(uint32_t XOffset, uint32_t YOffset);

	void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF bgr);

	void PutPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, int x, int y, COLORREF bgr);

	void DrawRect();
	
	void PutPixelBackOrder(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y, COLORREF gbr);

	COLORREF GetPixel(void* BitmapMemory, uint32_t BitmapWidth, uint32_t BitmapHeight, uint32_t x, uint32_t y);

};

