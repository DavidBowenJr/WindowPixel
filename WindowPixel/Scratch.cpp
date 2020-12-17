#include "Scratch.h"
#include "CustomRunner.h"

Scratch::Scratch()  { OutputDebugString(TEXT("CONSTRUCTOR CALLED FOR SCRATCH SURFACE  .....")); }
Scratch::~Scratch() { OutputDebugString(TEXT("DECONSTRUCTOR CALLED FOR SCRATCH SURFACE ....")); }

void Scratch::APP(CustomRunner& customRunner) {
	int x = 0;
	int y = 0;

	POINT p;
	BOOL  b;
	
	if (customRunner.hdc == NULL) customRunner.hdc  = GetDC(customRunner.hWnd);
	if (customRunner.hdc == NULL) return; 

	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b) { ReleaseDC(customRunner.hWnd, customRunner.hdc); return; }

	y = -p.x;
	x = -p.y;
	
	ReleaseDC(customRunner.hWnd, customRunner.hdc);  // wingdi
	
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

		customRunner.PlasmaXXXX();

		customRunner.Line(0, 0, 300, 300, RGB(255, 0, 255));

	}
}

