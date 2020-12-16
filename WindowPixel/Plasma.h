#pragma once
#include "framework.h"
#pragma comment(lib, "WinMM.lib")


struct SDL_Surface;

class Plasma 
{
public:
	Plasma();
	Plasma(SDL_Surface* Scr); // : scr(Scr) {};
	~Plasma(void);
	void SomeFunction5(HWND hwnd, win32_offscreen_buffer& surfdata);
	void Foo();
	void BuildMem();

	void ReleaseMem();

	SDL_Surface* scr;


	

};

static SDL_Surface* scr;

