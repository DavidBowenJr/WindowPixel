#pragma once
#include "framework.h"
#include "share.h" // contains structs to hold injectors
#pragma comment(lib, "WinMM.lib")


struct SDL_Surface;

class CustomRunner;

class Plasma 
{
public:
	Plasma(olved__buffer& buffer);
	Plasma();
	~Plasma(void);
	void SomeFunction5(HWND& rHWnd, win32_offscreen_buffer& rSurfData, CustomRunner& rDraw);
	//void SomeFunction5(HWND* pHWnd, win32_offscreen_buffer* surfdata, CustomRunner* draw);
	//void SomeFunction5(HWND* pHWnd, win32_offscreen_buffer* surfdata);
#if 0
	void SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata);
#endif

	void Foo();
	void BuildMem();

	void ReleaseMem();

	olved__buffer olvedBuffer;


	//SDL_Surface* scr;


	

};

//static SDL_Surface* scr;

