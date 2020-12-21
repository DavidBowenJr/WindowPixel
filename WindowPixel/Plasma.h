#pragma once
#include "framework.h"
#include "share.h" // contains structs to hold injectors
#pragma comment(lib, "WinMM.lib")


struct SDL_Surface;

class CustomRunner;

class Plasma 
{
public:

	win32_offscreen_buffer Buffer;

	Plasma(olved__buffer& buffer);
	Plasma();
	~Plasma(void);
	void SomeFunction5(HWND& rHWnd, win32_offscreen_buffer& rSurfData, CustomRunner& rDraw);
	
#if 0
	void SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata);
#endif
	
	void Foo();
	

	olved__buffer olvedBuffer;


};



