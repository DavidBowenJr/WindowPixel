#include "Scratch.h"
#include "CustomRunner.h"

Scratch::Scratch()
{
	OutputDebugString(TEXT("CONSTRUCTOR CALLED FOR SCRATCH SURFACE ....."));
}
Scratch::~Scratch()
{
	OutputDebugString(TEXT("DECONSTRUCTOR CALLED FOR SCRATCH SURFACE ...."));
}

void Scratch::APP(CustomRunner& customRunner)
{

	int x = 0;
	int y = 0;
	POINT p;
	BOOL  b;

	
	if(customRunner.hdc == NULL) customRunner.hdc  = GetDC(customRunner.hWnd);
	if (customRunner.hdc == NULL) return; 

	/* Get the current cursor position */
	b = GetCursorPos(&p);
	//GetMouseMovePointsEx();
	// b = FALSE;
	if (!b) {
		ReleaseDC(customRunner.hWnd, customRunner.hdc); //GetDesktopWindow(), customRunner.hdc);
	return;
	}

	
	y =  -p.x;
	x =  -p.y;
	//GetPixel(customRunner.hdc, p.y, p.x); // wingdi

	

	ReleaseDC(customRunner.hWnd, customRunner.hdc); //GetDesktopWindow(), customRunner.hdc); // wingdi
	

	/*
	LONG st = 0;
	LONG lg = ~0;
	OutputDebugString(std::to_wstring( lg   ).c_str());
	*/



customRunner.
RenderWeirdGradient(x, y);

	{
	
	customRunner.
	CopyImage(
		customRunner.
		Buffer,
		customRunner.
		TextureBuffer[0]);

	customRunner.
		FlipHorizontal(
			customRunner.
			Buffer,
			customRunner.
			TextureBuffer[0]);

	customRunner.
	FlipVertical(
		customRunner.
		Buffer, 
		customRunner.
		TextureBuffer[0]);
	


#ifdef __USE_TEST_PLASMA
#define __USE_TEST_PLASMA
	Plasma plasma(scr);
	plasma.SomeFunction5(hWnd, GlobalBackBuffer);

#endif;




	}


	// customRunner.RenderWeirdGradient(x, y);

	// this will be remove latter just a test...
	//customRunner.RenderWeirdGradient(x, y, customRunner.Buffer, customRunner.TextureBuffer[0] );

}

