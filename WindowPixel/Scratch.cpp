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

static	int x = 0;
	int y = 0;


	x += 5;

	customRunner.RenderWeirdGradient(x, y);
	// Bind Texture[0] with our main Render Image.
//	static bool doOnce = false;
//	if (!doOnce)
	{
		customRunner.CopyImage(customRunner.Buffer, customRunner.TextureBuffer[0]);
		customRunner.FlipHorizontal(customRunner.Buffer, customRunner.TextureBuffer[0]);
		customRunner.FlipVertical(customRunner.Buffer, customRunner.TextureBuffer[0]);
	//	doOnce = false;
	}


	// customRunner.RenderWeirdGradient(x, y);

	// this will be remove latter just a test...
	//customRunner.RenderWeirdGradient(x, y, customRunner.Buffer, customRunner.TextureBuffer[0] );

}

