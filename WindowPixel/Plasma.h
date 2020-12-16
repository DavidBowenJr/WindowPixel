#pragma once
#include "framework.h"
#include "share.h" // contains structs to hold injectors
#pragma comment(lib, "WinMM.lib")


struct SDL_Surface;

class Plasma 
{
public:
	Plasma(olved__buffer& buffer);
	Plasma();
	~Plasma(void);
	void SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata);
	void Foo();
	void BuildMem();

	void ReleaseMem();

	olved__buffer olvedBuffer;


	//SDL_Surface* scr;


	

};

//static SDL_Surface* scr;

