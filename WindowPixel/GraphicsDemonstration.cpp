#include "GraphicsDemonstration.h"


GraphicsDemonstration::GraphicsDemonstration()
{
}

GraphicsDemonstration::~GraphicsDemonstration()
{
	OutputDebugString(L"Class GraphicsDemonstration ~Destructor Called.\n ");
	//InvalidateRect(hWnd, NULL, 1);

	//DeleteObject(hOldbrush);

	DeleteObject(hRedPen); /* delete pens */
	DeleteObject(hGreenPen);
	DeleteObject(hBluePen);
	DeleteObject(hYellowPen);
	DeleteDC(memdc);
}

void GraphicsDemonstration::WindowMessageCreate(HWND hWnd)
{
	/*
	int maxX;
	int maxY;
	HDC hdc;
	HDC memdc;
	HBITMAP hbit;
	HBRUSH hbrush;
	HPEN  hRedPen;
	HPEN  hGreenPen;
	HPEN  hBluePen;
	HPEN  hYellowPen;
	HPEN  hOldPen;
	*/
	/* get screen coordinates */
	maxX = GetSystemMetrics(SM_CXSCREEN);
	maxY = GetSystemMetrics(SM_CYSCREEN);
	/* make a compatible memory image device */
	hdc = GetDC(hWnd);
	memdc = CreateCompatibleDC(hdc);
	hbit = CreateCompatibleBitmap(hdc, maxX, maxY);
	SelectObject(memdc, hbit);
	hbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	SelectObject(memdc, hbrush);
	PatBlt(memdc, 0, 0, maxX, maxY, PATCOPY);

	hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	hBluePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	hYellowPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));

	/* save default pen */
	hOldPen = (HPEN)SelectObject(memdc, hRedPen);
	SelectObject(memdc, hOldPen);
	ReleaseDC(hWnd, hdc);

}

void GraphicsDemonstration::ClearBackGround(HWND hWnd)
{

	UNREFERENCED_PARAMETER(hWnd);

	//this->hWnd = hWnd;
			//RECT CR;
			//	ValidateRect(hWnd,&CR);
	PatBlt(memdc, 0, 0, maxX, maxY, PATCOPY);
	//InvalidateRect(hWnd, NULL, 0);
}

void GraphicsDemonstration::CoolStuff3(HWND hWnd)
{
	this->hWnd = hWnd;
	RECT CR;
	ValidateRect(hWnd, &CR);
	/* display, but don't fill */
	hOldbrush = (HBRUSH)SelectObject(memdc, GetStockObject(HOLLOW_BRUSH));

	/* draw some rectangles */
	Rectangle(memdc, 50, 50, 300, 300);
	RoundRect(memdc, 125, 125, 220, 240, 15, 13);

	/* use a red pen */
	SelectObject(memdc, hRedPen);
	Rectangle(memdc, 100, 100, 200, 200);
	SelectObject(memdc, hOldPen); /* return to the old pen */

	/* restore default brush */
	SelectObject(memdc, hOldbrush); /* return to the default pen */

	//DeleteObject(hOldbrush);

	InvalidateRect(hWnd, NULL, 0);
}

void GraphicsDemonstration::CoolStuff(HWND hWnd)
{
	this->hWnd = hWnd;
	RECT CR;
	ValidateRect(hWnd, &CR);

	//WINGDIAPI HBRUSH  WINAPI CreateSolidBrush( __in COLORREF color);
//	WINGDIAPI HGDIOBJ WINAPI GetStockObject( __in int i);


// Ellipses
/* make brush blue */
	hbrush = CreateSolidBrush(RGB(0, 0, 255));
	hOldbrush = (HBRUSH)SelectObject(memdc, hbrush);

	/* fill the ellipse with blue */
	Ellipse(memdc, 50, 200, 100, 280);
	Ellipse(memdc, 78, 25, 280, 100);

	/* create green brush */
	hbrush = CreateSolidBrush(RGB(0, 255, 0)); /* Green Brush */
	SelectObject(memdc, hRedPen);
	Ellipse(memdc, 100, 100, 200, 200);

	/* draw a pie slice */
	Pie(memdc, 200, 200, 340, 340, 255, 200, 200, 250);
	SelectObject(memdc, hOldPen);
	SelectObject(memdc, hOldbrush);
	// Don't really have to but did just show object is not owner getstockobjects are window.
	hOldbrush = (HBRUSH)SelectObject(memdc, GetStockObject(HOLLOW_BRUSH));
	DeleteObject(hbrush); /* delete the green brush */

	InvalidateRect(hWnd, NULL, 0);

}

void GraphicsDemonstration::CoolStuff2(HWND hWnd)
{
	this->hWnd = hWnd;

	RECT CR;

	ValidateRect(hWnd, &CR);

	SetPixel(memdc, 40, 14, RGB(0, 0, 0));
	SetPixel(memdc, 40, 15, RGB(0, 0, 0));

	LineTo(memdc, 100, 50);
	MoveToEx(memdc, 100, 50, NULL);

	/* change to green pen */
	hOldPen = (HPEN)SelectObject(memdc, hGreenPen);
	LineTo(memdc, 200, 100);

	/* change to yellow pen */
	SelectObject(memdc, hYellowPen);
	LineTo(memdc, 0, 200);

	/* change to blue pen */
	SelectObject(memdc, hBluePen);
	LineTo(memdc, 200, 200);

	/* change to red pen */
	SelectObject(memdc, hRedPen);
	LineTo(memdc, 0, 0);

	/* return to default pen */
	SelectObject(memdc, hOldPen);

	Arc(memdc, 0, 0, 300, 300, 0, 50, 200, 50);
	/* show intersecting lines that define arc */
	MoveToEx(memdc, 150, 150, NULL);

	LineTo(memdc, 0, 50);
	MoveToEx(memdc, 150, 150, NULL);
	LineTo(memdc, 200, 50);

	// Note the last paramter set to one would clear our screen
	//

	InvalidateRect(hWnd, NULL, 0);

}

void GraphicsDemonstration::Paint(HWND hWnd)
{
	this->hWnd = hWnd;
	PAINTSTRUCT ps;
	//ZeroMemory(&ps, sizeof(PAINTSTRUCT));
	hdc = BeginPaint(hWnd, &ps); /* get DC */

	bool bSetMappingMode = false;
	if (bSetMappingMode)
	{
		/* set mapping mode, window and viewport extents */

		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 200, 200, NULL);
		SetViewportExtEx(hdc, 10, 10, NULL);
		SetViewportOrgEx(hdc, 10, 10, NULL);
	}


	/* now , copy memory image onto screen */
	BitBlt(hdc, 0, 0, maxX, maxY, memdc, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps); /* release DC */
}

void GraphicsDemonstration::destroy(HWND hWnd)
{
	this->hWnd = hWnd;
	//Clean up could go here. ext.

}
