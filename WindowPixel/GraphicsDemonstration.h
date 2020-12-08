#pragma once
#include "framework.h"

class GraphicsDemonstration
{
public:
	GraphicsDemonstration();
	~GraphicsDemonstration();
	/* place inside of WM_CREATE */
	void WindowMessageCreate(HWND);

	/* DO COOL STUFF */
	void ClearBackGround(HWND hWnd);
	void CoolStuff(HWND hWnd);
	void CoolStuff2(HWND hWnd);
	void CoolStuff3(HWND hWnd);
	void Paint(HWND hWnd);
	void destroy(HWND hWnd);
	/* screen dimensions */
	int maxX;
	int maxY;
	HDC hdc;
	HWND hWnd; /* should be initalized with Create and Paint */
	/* compatible memory image device old school */
	HDC memdc;
	/* handle of compatible bitmap */
	HBITMAP hbit;
	/* handle of brush and old brush */
	HBRUSH hbrush;
	HBRUSH hOldbrush;

	/* create pens */
	HPEN hRedPen;
	HPEN hGreenPen;
	HPEN hBluePen;
	HPEN hYellowPen;
	HPEN hOldPen; /* handle of old pen */
};

