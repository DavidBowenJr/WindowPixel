#pragma once
#include "framework.h"

//https://docs.microsoft.com/en-us/windows/win32/inputdev/about-raw-input
//https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
class RawInputApi
{
public:
	RAWINPUTDEVICE Rid[4];

	RawInputApi();
	~RawInputApi();

	// Registering for Raw Input
	// An application specifies the raw input from game controllers (both game pads and joysticks)
	// and all devices off the telephony usage page except answering machines

	 /// <summary>
	 /// for raw input an application must register the devices it wants to get data from.
	 /// </summary>
	 void RegisterRawInput();

	 HRESULT WM_INPUT_PROC(WPARAM wParam, LPARAM lParam);

//	 Human Interface Devices(HIDs).

};



