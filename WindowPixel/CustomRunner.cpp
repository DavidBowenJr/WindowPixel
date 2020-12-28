

#include "CustomRunner.h"
#include "RawInputApi.h" // hopefully keyboard mouse and joystick
#include "Scratch.h"
#include "Plasma.h"

#include "ImageSource.h"
#include "colorstruct.h"
#include <mmsystem.h> // timeGetTime

#include <chrono>
#include <thread>
#include <limits.h>
#include "SaveBitmap.h"





	CustomRunner::CustomRunner()
	{
		this->rfCntHdc = 0;

		OutputDebugString(TEXT("\n ..CR..Con.. \n"));

		Buffer.Memory = NULL;

		LRESULT Result = 0;
		
		wmId = 0;
		
		//HDC hdc = NULL;
		//HWND hWnd = NULL;
		
		Buffer.BytesPerPixel = 4;
		
		UNREFERENCED_PARAMETER(hWnd);
		UNREFERENCED_PARAMETER(Result);
		UNREFERENCED_PARAMETER(hdc);

		{
			BITMAPINFOHEADER bitmapInfoHeader;
			uint8_t* bitmapData;

			// Should add error handling here it pass so far.


			// There are ways to post build and get our image data into our debug or ext
			// but for now compile then place into the debug or release...
			// VERY SIMPLE TEST.....
			// Will Place else where but for now simple test.
			bitmapData = LoadBitmapFile((char*)("photo3.bmp"), &bitmapInfoHeader);
			bitmapInfoHeader.biBitCount = 32;
			memcpy(&TextureBuffer[0].Info, &bitmapInfoHeader, sizeof(BITMAPINFO));

			//TextureBuffer[0].Memory = bitmapData;

			// TextureBuffer[0].Memory = new uint8[sizeof(uint32) * (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight)];
			TextureBuffer[0].Memory = new uint32[bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight];
			// Or 


			// We know for now a component is 8 bits long... 4 components is 32 bits total...
			memcpy( TextureBuffer[0].Memory,  bitmapData, sizeof(uint32) * (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight));// bitmapInfoHeader.biSizeImage + (bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight)); // bitmapData;
			SAFE_DELETE(bitmapData);

			TextureBuffer[0].Width = bitmapInfoHeader.biWidth;
			TextureBuffer[0].Height = bitmapInfoHeader.biHeight;
			TextureBuffer[0].BytesPerPixel = bitmapInfoHeader.biBitCount / 8;
			TextureBuffer[0].Pitch = bitmapInfoHeader.biWidth * (bitmapInfoHeader.biBitCount / 8);
			TextureBuffer[0].Pitch = TextureBuffer[0].Pitch;

			ZeroMemory(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

			if (TextureBuffer[0].Width > 0)
			{
				OutputDebugString(std::to_wstring(TextureBuffer[0].Width).c_str());
			}
		}
	}


	CustomRunner::~CustomRunner()
	{

		DeleteObject(hfont);

		// Our BackBuffer for say maybe.
		// better name but for now bfn.
		DeleteObject( hBitmapBackBuffer   );
		DeleteDC(this->memBackSurface);





		SafeReleaseDC();

		SAFE_DELETE(pplasma);
		SAFE_DELETE(scratch);

		//free(TextureBuffer[0].Memory);
		SAFE_DELETE(TextureBuffer[0].Memory);

		if (Buffer.Memory != NULL)
			if (VirtualFree(
				Buffer.Memory,       // Base address of block
				0,             // Bytes of committed pages  // Decommit the pages <MEM_RELEASE>
				MEM_RELEASE))
			{
				OutputDebugString(_T("\n Virtual Free Bitmap MEMORY RELEASED"));
			}
		OutputDebugString(_T("\n Class CustomRunner  ~ Called.\n"));
	}


	void CustomRunner::SafeReleaseDC()
	{
		rfCntHdc = rfCntHdc;
		if (this->rfCntHdc >= 1)
		{
			ReleaseDC(this->hWnd, this->hdc);
			rfCntHdc -= 1;
		}
	}


	HDC CustomRunner::SafeGetDC()
	{
		if (this->rfCntHdc == 0) // Good
		{
			rfCntHdc += 1;
			this->hdc = GetDC(hWnd);
			return this->hdc;
		}
		else
		{
			MessageBox(NULL, TEXT("NAG...!Potential Problem trying to lock an all ready lock Context. "), TEXT("Error"), MB_OK);
		}
		return HDC(this->hdc);
	}


	void CustomRunner::Win32ResizeDibSection(uint32 Width, uint32 Height)
	{
		if (Buffer.Memory)
		{
			VirtualFree(Buffer.Memory, 0, MEM_RELEASE);
		}

		Buffer.Width = Width;
		Buffer.Height = Height;
		Buffer.BytesPerPixel = 4; //?

		Buffer.Info.bmiHeader.biSize = sizeof(Buffer.Info.bmiHeader);
		Buffer.Info.bmiHeader.biWidth = (LONG)Buffer.Width;
		Buffer.Info.bmiHeader.biHeight = -(signed int)Buffer.Height;
		Buffer.Info.bmiHeader.biPlanes = 1;
		Buffer.Info.bmiHeader.biBitCount = 32;
		Buffer.Info.bmiHeader.biCompression = BI_RGB;

		SIZE_T BitmapMemorySize = (Buffer.Width * Buffer.Height) * Buffer.BytesPerPixel;
		Buffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
		Buffer.Pitch = Width * Buffer.BytesPerPixel;
	}



	void CustomRunner::TestSomeMouse()
	{
	//	BOOL isMousePressent = ::GetSystemMetrics(SM_MOUSEPRESENT);
	//	BOOL isMouseWheelPressent = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
//INT nButtonCount = ::GetSystemMetrics(SM_CMOUSEBUTTONS);

		// Press or Release of a mouse button
		// double click of a mouse button
		// movement of the mouse.
		// ignore events not in client area of this application
		//https://flylib.com/books/en/4.348.1.16/1/
		
		// [Message]				[Sent When]
		// WM_LBUTTONDOWN		|	left mouse button is pressed
		// WM_LBUTTONUP			|	left mouse button is released
		// WM_LBUTTONDBCKLK		|	left mouse button is double-clicked
		// WM_MBUTTONDOWN		|	middle mouse button is pressed
		// WM_MBUTTONUP			|	middle mouse button is released
		// WM_MBUTTONDBCLK		|	middle mouse button is double-clicked
		// WM_RBUTTONDOWN		|	right  mouse button is pressed
		// WM_RBUTTONUP			|	right  mouse button is released
		// WM_RBUTTONDBLCLK		|	right  mouse button is double-clicked
		// WM_MOUSEMOVE			|	cursor is moved over the window's client area
		// https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input




		


	}

	void CustomRunner::CreateAFont(HDC &hdc_param)
	{
		RECT Rect;
		if (GetClientRect(this->hWnd, &Rect))
		{

			CHOOSEFONT cf;
			LOGFONT lf;


			// Initialize members of the CHOOSEFONT structure.

			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.hwndOwner = (HWND)NULL;
			cf.hDC = (HDC)NULL;
			cf.lpLogFont = &lf;
			cf.iPointSize = 0;
			cf.Flags = CF_SCREENFONTS;
			cf.rgbColors = RGB(0, 0, 0);
			cf.lCustData = 0L;
			cf.lpfnHook = (LPCFHOOKPROC)NULL;
			cf.lpTemplateName = (LPCWSTR)NULL;
			cf.hInstance = (HINSTANCE)NULL;
			cf.lpszStyle = (LPWSTR)NULL;
			cf.nFontType = SCREEN_FONTTYPE;
			cf.nSizeMin = 0;
			cf.nSizeMax = 0;

			// Display the CHOOSEFONT common-dialog box.

			ChooseFont(&cf);

			// Create a logical font based on the user's
			// selection and return a handle identifying
			// that font.



			hfont = CreateFontIndirect(cf.lpLogFont);
			SelectObject(hdc_param, hfont);

		}


	}



	void CustomRunner::TestSomeGDIProcedureTwo(HDC& hdc_param, uint32& WindowWidth, uint32& WindowHeight)
	{
		COLORREF old_fcolor, old_bcolor; // old foreground text color, old background text color
		INT old_tmode; // old text transparency mode

		// Clear The  Memmory image Screen
		SelectObject( hdc_param, this->hBitmapBackBuffer);
		FillRect(  hdc_param, &this->ps.rcPaint, (HBRUSH)(COLOR_ACTIVEBORDER)); 

		////////////////////////////////////////
		// Set foreground color to some color | AND Save the old one
		old_fcolor = SetTextColor(hdc_param, RGB(255, 255, 255));

		// Set the background color to black | AND Save the old one
		old_bcolor = SetBkMode(hdc_param, RGB(0, 0, 0));

		// Set the transparency mode to Transparent | AND Save the old one
		old_tmode = SetBkMode(hdc_param, TRANSPARENT);

		RECT clientRect;
		if (GetClientRect(hWnd, &clientRect)) {

			TextOut(hdc_param, 120, 130, L"Hello", (int)strlen("Hello"));

		}

		// now restore everything
		SetTextColor (hdc_param,    old_fcolor);
		SetBkColor   (hdc_param,    old_bcolor);
		SetBkMode    (hdc_param,    old_tmode);
		//hdc_param = memBackSurface;
	}

	// PROTO CODE
	void CustomRunner::TestSomeGDIProcedure(HDC& hdc_param, uint32& WindowWidth, uint32& WindowHeight)
	{
		std::wstring wstr;

		// Getting the GDC Text on top of our memory ...Draw ... text last.
		COLORREF old_fcolor, // old foreground text color
			old_bcolor; // old background text color
		int old_tmode; // old text transparency mode

		
		////////////////////////////////////////
		// Clear Screen
		SelectObject(this->memBackSurface, this->hBitmapBackBuffer);

		//        COLOR_ACTIVEBORDER equals 10   COLOR_WINDOW = 5
		FillRect(this->memBackSurface, &this->ps.rcPaint, (HBRUSH)(COLOR_ACTIVEBORDER)); //         (COLOR_WINDOW + 5));
		

		// set the foreground color to green and save old one
		old_fcolor = SetTextColor(hdc_param, RGB(255, 255, 255)); // RGB(0, 0, 255));

		// set the background color to black and save old one
		old_bcolor = SetBkColor(hdc_param, RGB(0, 0, 0));

		// finally set the transparency mode to transparent
		old_tmode = SetBkMode(hdc_param, TRANSPARENT);

		///////////////////////////////////////////////////////

		
		RECT Rect;

		// Spawn off the font this system has and let user decide .... 
	// Im useing the deconstrutor to DeleteOjbect for now... 
	// gona ask once for now.. 
		static BOOL spawnCreateAFontOnce = TRUE;

		if (spawnCreateAFontOnce)
		{  // Magic Font creation 

		if (GetClientRect(this->hWnd, &Rect))
		{

			CHOOSEFONT cf;
			LOGFONT lf;


			// Initialize members of the CHOOSEFONT structure.

			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.hwndOwner = (HWND)NULL;
			cf.hDC = (HDC)NULL;
			cf.lpLogFont = &lf;
			cf.iPointSize = 0;
			cf.Flags = CF_SCREENFONTS;
			cf.rgbColors = RGB(0, 0, 0);
			cf.lCustData = 0L;
			cf.lpfnHook = (LPCFHOOKPROC)NULL;
			cf.lpTemplateName = (LPCWSTR)NULL;
			cf.hInstance = (HINSTANCE)NULL;
			cf.lpszStyle = (LPWSTR)NULL;
			cf.nFontType = SCREEN_FONTTYPE;
			cf.nSizeMin = 0;
			cf.nSizeMax = 0;

			// Display the CHOOSEFONT common-dialog box.

			ChooseFont(&cf);

			// Create a logical font based on the user's
			// selection and return a handle identifying
			// that font.



			hfont = CreateFontIndirect(cf.lpLogFont);
			SelectObject(hdc_param, hfont);

		}

	
		//	this->CreateAFont(hdc_param);
			spawnCreateAFontOnce = FALSE;
		}

		
		
		SelectObject(hdc_param, hfont);

		///////////////////////////////////////////////////////


		wstr.append(L" This is a test ");

		// SelectObject(hdc_param, GetStockObject(SYSTEM_FIXED_FONT));
		
		 //  https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-drawtext
	//	 DrawText(hdc_param, L"HI THERE", sizeof("HI THERE"), &Rect, DT_CALCRECT);

		/*
				ZeroMemory(wbuff, sizeof(WCHAR) * BUF_SIZE);
				wsprintf(wbuff, wstr.c_str(), wstr.size());
				TextOut(hdc_param, 120, 50, wbuff, BUF_SIZE);
		*/

		
		static	long double ld = 100.00001;
		ld += 0.01;
		wstr.append(std::to_wstring(ld));
		wstr.append(L"\t: counting away. ");
	

		SetBkColor(hdc_param, RGB(255, 255, 255));

	//	 SelectObject(hdc_param, GetStockObject(SYSTEM_FIXED_FONT));
	
		 
		 TextOut(hdc_param, 120, 50, wstr.c_str(), static_cast<int>(wstr.size()));
		wstr.clear();


	//	SelectObject(hdc_param, GetStockObject(SYSTEM_FIXED_FONT));


		// draw some text at (20,30)
		TextOut(hdc_param, 120, 130, L"Hello", (int)strlen("Hello"));
		


	//	DeleteObject(hfont);  /// test.
	

		// now restore everything
		SetTextColor(hdc_param, old_fcolor);
		SetBkColor(hdc_param, old_bcolor);
		SetBkMode(hdc_param, old_tmode);



		HBRUSH	hOldbrush2 = (HBRUSH)SelectObject(hdc_param, GetStockObject(HOLLOW_BRUSH));

		//https: //docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/2af5f7e4-9abb-4fb4-b428-d739c8b23829

	//	HPEN white_dashed_pen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 255, 255));
	//	SelectObject(hdc_param, white_dashed_pen);


		old_bcolor = SetBkMode(hdc_param, OPAQUE);
		SetBkColor(hdc_param,RGB(0, 0, 0));


		// Cursor should go here
		POINT p = { 100, 100 };                                //    this->getLocalCursor(hdc_param);

		RoundRect(hdc_param, 1 + p.x, 1 + p.y, 220, 240, 15, 13);
		//	DeleteObject(white_dashed_pen);


		//	MoveToEx(hdc_param, 10, 10, NULL);
			//	LineTo(hdc_param, 50, 60);
		MoveToEx(hdc_param, 20, 10, NULL);
		LineTo(hdc_param, 30, 20);
		LineTo(hdc_param, 10, 20);
		LineTo(hdc_param, 20, 10);

		Ellipse(hdc_param, p.x + 10, p.y + 10, p.x + 30, p.y + 30);
		Ellipse(hdc_param, 250, 175, 350, 225);

		// create the polygon shown in the figure

#if 0

		POINT p0 = { 10, 30 };
		POINT p1 = { 20,  0 };
		POINT p2 = { 30, 32 };
		POINT p3 = { 40, 31 };
		POINT p4 = { 35, 40 };
		POINT p5 = { 20, 40 };
		POINT p6 = { 15, 45 };

		HBRUSH	hOldbrush = (HBRUSH)SelectObject(hdc_param, GetStockObject( WHITE_BRUSH)); //      DKGRAY_BRUSH));
		POINT poly[7] = { p0.x, p0.y, p1.x, p1.y, p2.x, p2.y,
		p3.x, p3.y, p4.x, p4.y, p5.x, p5.y, p6.x, p6.y };
		// assume hdc is valid, and pen and brush are selected into
		
		// graphics device context
		Polygon(hdc_param, poly, 7);
#endif


	}

	void CustomRunner::TestSaveFeature(HDC hdc_param)
	{
		RECT Rect;
		if (GetClientRect(this->hWnd, &Rect))
		{

			int maxX = Rect.right - Rect.left;
			int maxY = Rect.bottom - Rect.top;
			HDC memdc = CreateCompatibleDC(hdc_param);
			HBITMAP hbit = CreateCompatibleBitmap(hdc_param, maxX, maxY);
			INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
			INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);
			SelectObject(memdc, hbit);
			BitBlt(memdc, 0, 0, Rect.right, Rect.bottom, hdc_param, x, y, SRCCOPY);
			if (mSaveBitmap((LPWSTR)L"SavedBitmapmem.bmp", this->hWnd, memdc)) {}
			
			DeleteObject(hbit);
			
			ReleaseDC(this->hWnd, memdc);


			if (mSaveBitmap((LPWSTR)L"SavedBitmaplocal.bmp", this->hWnd, hdc_param)) {}

		}

	}

	
	void CustomRunner::Win32UpdateWindow(HDC& hdc_param, uint32& WindowWidth, uint32& WindowHeight)
	{
		
		if (GetDeviceCaps(hdc_param, RASTERCAPS))
		{
			if (Buffer.Memory == NULL) _ASSERT(L"Bad");
			int previousmode = SetStretchBltMode(hdc_param, MAXSTRETCHBLTMODE);
			int wh = SetICMMode(hdc_param, ICM_ON); if (wh == wh) {};
			if (previousmode == previousmode) {};
			StretchDIBits(hdc_param, 0, 0, WindowWidth, WindowHeight, 0, 0, Buffer.Width, Buffer.Height, Buffer.Memory, (const BITMAPINFO*)&Buffer.Info, (UINT)DIB_RGB_COLORS,(DWORD)SRCCOPY);
		}
	}




	void CustomRunner::Win32UpdateWindow()
	{
		{
			static BOOL runone = TRUE;
			
			RECT rect;
			if (GetClientRect(this->hWnd, &rect))
			{
				uint32 mX = rect.right - rect.left;
				uint32 mY = rect.bottom - rect.top;
				
				if(runone)
				{
					runone = !runone;
					// IF CHANGES SIZE DEAL WITH LATTER
					
					this->memBackSurface = CreateCompatibleDC(this->hdc);
					this->hBitmapBackBuffer = CreateCompatibleBitmap(this->hdc, mX, mY);
				}
				
				if (this->memBackSurface)
				{
					
					SelectObject(this->memBackSurface, this->hBitmapBackBuffer);
					TestSomeGDIProcedure(this->memBackSurface, mX, mY);
					
					if (this->hdc)
					{
						SafeGetDC();
						

						SelectObject(this->hdc, this->hBitmapBackBuffer);
						Win32UpdateWindow(this->hdc, mX, mY);

						BitBlt(this->hdc, 0, 0, rect.right, rect.bottom, this->memBackSurface, 0, 0, SRCPAINT);

						// uncomment still works.
					//	this->TestSaveFeature(hdc);

						SafeReleaseDC();
					}
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(150));


	}


	BOOL CustomRunner::mSaveBitmap(LPWSTR wPath, HWND hWnd, HDC hdc)
	{
		return 	 SaveBitmap((LPWSTR)wPath, hWnd, hdc);
	}



	HWND CustomRunner::myPaint()
	{
		this->hWnd = this->myPaint(this->hWnd);
		return this->hWnd;
	}

	HWND CustomRunner::myPaint(HWND hWnd)
	{

// Very good read somewhat old but good ...

//	http://www.yaldex.com/games-programming/0672323699_ch03lev1sec3.html


		// BOOL ValidateRect(HWND hWnd; // handle of window
	    // CONST RECT *lpRect); // address of validation rectangle coordinates

		// handle of window along with a region you want to be validated in lpRect.
		// PAINTSTRUCT ps; // used in WM_PAINT
		// HDC hdc; // handle to a device context
		// RECT rect; // rectangle of window


#if 1
		SafeReleaseDC();

		this->hdc = BeginPaint(hWnd, &this->ps);
	
		this->Win32UpdateWindow();

		EndPaint(hWnd, &ps);

		ReleaseDC(hWnd, hdc);
#endif


		// This is good also  but testing some theory from Yaldex...
#if 0
		SafeReleaseDC();
		
		RECT CR;
		GetClientRect(hWnd, &CR);
		this->hWnd = hWnd;
		
		this->hdc = BeginPaint(hWnd, &this->ps);              
		this->rfCntHdc = 1;
		this->Win32UpdateWindow();

		EndPaint(hWnd, &this->ps);
		ReleaseDC(hWnd, hdc);
#endif

		return hWnd;
	}

	void CustomRunner::ClearBuffer()
	{
		//uint8_t* row = (uint8_t*)BitmapMemory; // get the first 8 bits from a 32 bit buffer in memory
	}

	void CustomRunner::Render() { this->scratch->APP(*this); }


	void CustomRunner::AnotherWrap(CustomRunner& custRun)
	{
		/*
		win32_offscreen_buffer* Bf = &custRun.Buffer;
		win32_offscreen_buffer* Gd = &custRun.TextureBuffer[0];

		 RenderWeirdGradient(0, 0,  *Bf, *Gd);
		 */

		 /*
		 win32_offscreen_buffer Bf = custRun.Buffer;
		 win32_offscreen_buffer Gd = custRun.TextureBuffer[0];

		 RenderWeirdGradient(0, 0, Bf, Gd);
		 */

#ifdef STRANGE_RENDER
		RenderWeirdGradient(0, 0, custRun.Buffer, custRun.TextureBuffer[0]);
		
#endif
		Funn();


	}

	void CustomRunner::CopyImage(win32_offscreen_buffer& destBuffer, win32_offscreen_buffer& sourceBuffer)
	{

		for (uint32 j = 0; j < sourceBuffer.Width; j++)
			for (uint32 i = 0; i < sourceBuffer.Height; i++)
			{
				//	COLORREF color = CustomRunner::GetPixel(sourceBuffer.Memory, sourceBuffer.Width, sourceBuffer.Height, j, i);
					// PutPixel(destBuffer.Memory, destBuffer.Width, destBuffer.Height, j, i, color);
				COLORREF color = CustomRunner::mGetPixel(this->Buffer, j, i);
				PutPixel(this->Buffer, j, i, color);
			}
	}

	void CustomRunner::FlipHorizontal(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source)
	{
		uint32 sy = 0; // y and pitch for Source
		uint32 dy = 0; // y and pitch for Destination

		uint32* gps = (uint32*)Source.Memory; // Base Address  for our source memory region
		uint32* gpd = (uint32*)Destination.Memory; // Base Address for our destination memory region

		for (uint32_t Y = 0; Y < Source.Height; Y++)
		{
			// Compute Row...
			sy = (Source.Width * Y);
			dy = (Destination.Width * Y);
			for (uint32 X = 0; X < Source.Width; X++)
			{
				// Compute Row Times Column
				// On the Source we would like to read from it's end to it's start to flip horizontal
				uint32* ps = gps;
				ps += (sy + (Source.Width - X));

				// Now calculate the destination in forward order
				uint32* pd = gpd;
				pd += dy + X;

				// We have our address at the new possition we dereference our pointers.
				// Pass by value. from our souce cell to our destination cell
				*pd = *ps;
			}
		}
	}

	void CustomRunner::FlipVertical(win32_offscreen_buffer& Destination, win32_offscreen_buffer& Source)
	{
		uint32 sy = 0;
		uint32 dy = 0;
		uint32* gps = (uint32*)Source.Memory;
		uint32* gpd = (uint32*)Destination.Memory;

		for (uint32 Y = 0; Y < Source.Height; ++Y)
		{
			sy = (Source.Width * (Source.Height - Y));
			dy = (Destination.Width * Y);
			for (uint32 X = 0; X < Source.Width; ++X)
			{
				uint32* ps = gps;
				uint32* pd = gpd;
				ps += (sy + X);
				pd += (dy + X);
				*pd = *ps;
			}
		}
	}


	void CustomRunner::Funn()
	{
		static int inc = 0;
		for(unsigned int y = 0; y < Buffer.Height; y++)
			for (unsigned int x = 0; x < Buffer.Width; x++)
			{
				COLORREF color = RGB( (BYTE) y ,(BYTE) x  , (BYTE)  x + y );
				//COLORREF color = RGB((BYTE)x, (BYTE)x + y, (BYTE)y);
				mSetPixel(x, y, color);
				inc++;
			}
	}

	void CustomRunner::PutPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y, COLORREF bgr)
	{
		if (BitmapMemory == NULL) return;

		if (y < BitmapHeight)  // && y > -1)
			if (x < BitmapWidth) // && x > -1)
			{
				unsigned int* p32b = (unsigned int*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)&bgr;
				*p32b = (unsigned int)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::PutPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, int x, int y, COLORREF bgr)
	{
		if (BitmapMemory == NULL) return;

		if (y < (signed int)BitmapHeight && y > -1)
			if (x < (signed int)BitmapWidth && x > -1)
			{
				uint32_t* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)&bgr;
				*p32b = (uint32)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::PutPixel(win32_offscreen_buffer& Destination, uint32 x, uint32 y, COLORREF bgr)
	{
		if (Destination.Memory == NULL) return;

		if (y < Destination.Height)
			if (x < Destination.Width)
			{
				uint32* p32b = (uint32*)Destination.Memory;
				p32b += (Destination.Width * y) + x;
				uint8* p8b = (uint8*)&bgr;
				*p32b = (uint32)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
	}

	void CustomRunner::mSetPixel(uint32 x, uint32 y, COLORREF color)
	{
		this->PutPixel(this->Buffer, x, y, color);
	}

	// Rosettacode.org line. Bresenham's line
	void CustomRunner::Line(float x1, float y1, float x2, float y2,  const COLORREF& color)
	{
		// Bresenham's line algorithm
		const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
		if (steep)
		{
			std::swap(x1, y1);
			std::swap(x2, y2);
		}

		if (x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float dx = x2 - x1;
		const float dy = fabs(y2 - y1);

		float error = dx / 2.0f;
		const int ystep = (y1 < y2) ? 1 : -1;
		int y = (int)y1;

		const int maxX = (int)x2;

		for (int x = (int)x1; x <= maxX; x++)
		{
			if (steep)
			{
				mSetPixel(y, x, color);
			}
			else
			{
				mSetPixel(x, y, color);
			}

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}

	}

	void CustomRunner::WuDrawLine(void* bm, uint32 bw, uint32 bh, float x0, float y0, float x1, float y1)
	{
		//this->DrawRect(bm, bw, bh);
		if (bm == NULL) return;

		for (uint32 i = (uint32)y0; i < y0 + y1; i++)
			for (uint32 j = (uint32)x0; j < (uint32)x0 + x1; j++)
				this->PutPixel(bm, bw, bh, j, i, (0xFFFFFFFF));
	}

	void CustomRunner::DrawRect(void* bm, uint32 bw, uint32 bh)
	{
		if (bm == NULL) return;
		for (uint32 i = 0; i < bw / 4; i++)
			for (uint32 j = 0; j < bh / 4; j++)
			{
				this->PutPixel(bm, bw, bh, i, j, (COLORREF)0XFFFF0000);
			}
	}

	void CustomRunner::DrawRect()
	{
		if (this->Buffer.Memory == NULL) return;
		for (uint32 i = 0; i < Buffer.Width / 4; i++)
			for (uint32 j = 0; j < Buffer.Height / 4; j++)
			{
				this->PutPixel(this->Buffer, i, j, (COLORREF)0Xffff0000);
			}
	}

	void CustomRunner::Foo()
	{
		uint32_t w = this->Buffer.Width;
		uint32_t h = this->Buffer.Height;
#if 0
		for(int y = 0; y < h; y++)
			for (int x = 256; x < w; x++)
			{
				// int color = int(128.0 + (128.0 * sin(x / 8.0)));
				// int color = int(128.0 + (128.0 * sin((x + y) / 8.0)));
				 int color = int(128.0 + (128.0 * sin(sqrt((x - w / 2.0) * (x - w / 2.0) + (y - h / 2.0) * (y - h / 2.0)) / 8.0)));
			//	int color = int(128.0 + (128.0 * sin(x / 8.0))+ 128.0 + (128.0 * sin(y / 8.0))) / 2;
				
				SetPixel(x, y, RGB(color, color, color));
			}
#endif

#if 1

	//	https://lodev.org/cgtutor/plasma.html

#define screenWidth  256
#define screenHeight 256

		w = screenWidth;
		h = screenHeight;



		// Y-ccordinate first because we use horizontal scanlines
static		uint32 xplasma[screenHeight][screenWidth];
static		uint32 xlbuffer[screenHeight][screenWidth];
static		uint32 xpalette[256];

// generate the palette
ColorRGB colorRGB;
for (int x = 0; x < 256; x++)
{
	//use HSVtoRGB to vary the Hue of the color through the palette
	colorRGB = HSVtoRGB(ColorHSV( static_cast<uint8>(x), 255, 255));
	xpalette[x] = RGBtoINT(colorRGB);
}

// generate the plasma once
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		
		// the plasma buffer is a sum of sines
		int color = int( 128.0 + (128.0 * sin(static_cast<int>(x) / 16.0)) + 128.0 + (128.0 * sin(static_cast<int>(y) / 16.0)) ) / 2;
		xplasma[y][x] = (uint32) color;
	}

int paletteShift;

//start the animation loop, it rotates the palette
// the parameter to shif the palette varies with time
paletteShift = int( timeGetTime() / 10.0);

//draw every pixel again with the shifted palette color
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		xlbuffer[y][x] = xpalette[(xplasma[y][x] + paletteShift) % 256];
	}

//make everything visible
for (size_t y = 0; y < h; y++)
	for (size_t x = 0; x < w; x++)
	{
		mSetPixel(static_cast<uint32_t>(x), static_cast<uint32_t>(y),(COLORREF) xlbuffer[y][x]);
	}



#endif



	}

	void CustomRunner::PutPixelBackOrder(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y, COLORREF gbr)
	{
		if (BitmapMemory == NULL) return;
		UNREFERENCED_PARAMETER(gbr);
		if (y < BitmapHeight)
			if (x < BitmapWidth)
			{
				uint32* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				BYTE r = GetRValue(*p32b);
				BYTE g = GetGValue(*p32b);
				BYTE b = GetBValue(*p32b);
				*p32b = RGB(b, g, r);
				*p32b = ~*p32b;
			}
	}

	COLORREF CustomRunner::mGetPixel(void* BitmapMemory, uint32 BitmapWidth, uint32 BitmapHeight, uint32 x, uint32 y)
	{
		if (BitmapMemory == NULL) return COLORREF(0);
		if (y < BitmapHeight)
			if (x < BitmapWidth) 
			{
				uint32* p32b = (uint32*)BitmapMemory;
				p32b += (BitmapWidth * y) + x;
				unsigned char* p8b = (unsigned char*)p32b;
				return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
		return COLORREF(0);
	}

	COLORREF CustomRunner::mGetPixel(win32_offscreen_buffer& sourceBuffer, uint32 x, uint32 y)
	{
		if (sourceBuffer.Memory == NULL) return COLORREF(0);
		if (y < sourceBuffer.Height)
			if (x < sourceBuffer.Width)
			{
				uint32* p32b = (uint32*)sourceBuffer.Memory;
				p32b += (sourceBuffer.Width * y) + x;
				uint8b* p8b = (uint8b*)p32b;
				return (COLORREF)*p8b << 16 | *(p8b + 1) << 8 | *(p8b + 2);
			}
		return COLORREF(0);
	}

	/////////////////////////////////////////////////////////////////////////

#if 0
	// Temporaryly removed trying to see what's going on in raster. maybe refresh rate.

	// Had to change My GetPixel to mGetPixel as to not conflict with Windows.
	COLORREF CustomRunner::getColorAtCursor(void) {
		POINT p;
		COLORREF color;
		HDC hDC;
		BOOL b;

		/* Get the device context for the screen */
		hDC = GetDC(NULL);
		if (hDC == NULL) return CLR_INVALID;

		/* Get the current cursor position */
		b = GetCursorPos(&p);
		if (!b)
			return CLR_INVALID;

		/* Retrieve the color at that position */
		color = GetPixel(hDC, p.x, p.y);

		/* Release the device context again */
		ReleaseDC(GetDesktopWindow(), hDC);

		return color;
	}

	POINT CustomRunner::getLocalCursor(void) {
		POINT p;
		HDC hdc;
		hdc = GetDC(this->hWnd);
		if (hdc == NULL) return { ~0,~0 }; 
		BOOL b = GetCursorPos(&p);
		if (!b) return { ~0,~0 };
		return p;
	}

	POINT CustomRunner::getLocalCursor(HDC hdc)
	{
		POINT p;
		GetCursorPos(&p);
		return p;
	}
#endif

	// Just a test
	//void CustomRunner::PlasmaXXXX() { this->pplasma->SomeFunction5(this->hWnd, this->Buffer, *this); }

