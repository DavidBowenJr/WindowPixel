#include "Scratch.h"
#include "Plasma.h"
#include "CustomRunner.h"

Scratch::Scratch()  { OutputDebugString(TEXT("CONSTRUCTOR CALLED FOR SCRATCH SURFACE  .....")); }
Scratch::~Scratch() { OutputDebugString(TEXT("DECONSTRUCTOR CALLED FOR SCRATCH SURFACE ....")); }



void Scratch::APP(CustomRunner& customRunner) {
	int x = 0;
	int y = 0;

	POINT p;
	BOOL  b;
	
	 customRunner.SafeGetDC();
	//customRunner.SafeGetDC();


	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b) {customRunner.SafeReleaseDC(); return; }

	y = -p.x;
	x = -p.y;
	
	customRunner.SafeReleaseDC();  // wingdi
	


	       //     customRunner.Funn();



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


	 //////////////////////////////////////////    customRunner.pplasma->SomeFunction5(customRunner.hWnd, customRunner.Buffer, customRunner);
	
		 // This one is wraped in customRunner.
		

		OutputDebugStringW(L"After  SCRATCH WORKING \n");
		OutputDebugStringW(std::to_wstring(customRunner.inputclass->jlx).c_str());
		OutputDebugStringW(L"\t SCRATCH FIN \n");

	
		// for a test if user pressed the zero button then request shut down of windows.
		if (customRunner.inputclass->m_joyState.rgbButtons[0] & 0x80)
		{
			DestroyWindow(customRunner.hWnd);
			return;
		}


		customRunner.Line(0, 0, 300, 300, RGB(0, 255, 0));
		
		
		customRunner.PlasmaXXXX();

	//	customRunner.BowCurve2D( customRunner.Buffer);
	
	//	customRunner.Foo();



	}

	
	



}




