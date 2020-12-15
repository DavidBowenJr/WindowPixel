#include "Scratch.h"
#include "CustomRunner.h"

Scratch::Scratch()
{

}
Scratch::~Scratch()
{
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
	
	if (!b) return;
	y =  -p.x;
	x =  -p.y;
	//GetPixel(customRunner.hdc, p.y, p.x); // wingdi

	

	ReleaseDC(GetDesktopWindow(), customRunner.hdc); // wingdi

 





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
	//	doOnce = false;
	}


	// customRunner.RenderWeirdGradient(x, y);

	// this will be remove latter just a test...
	//customRunner.RenderWeirdGradient(x, y, customRunner.Buffer, customRunner.TextureBuffer[0] );

}

