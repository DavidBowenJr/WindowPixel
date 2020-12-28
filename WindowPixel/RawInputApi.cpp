#include "RawInputApi.h"
#include <strsafe.h>
RawInputApi::RawInputApi()
{

}
RawInputApi::~RawInputApi()
{
}

void RawInputApi::RegisterRawInput()
{
	if (RegisterRawInputDevices(Rid, 4, sizeof(Rid[0])) == FALSE) {
		// REGISTRATION FAILED. Call GetLastError for the cause of the error.
	}

}

HRESULT RawInputApi::WM_INPUT_PROC(WPARAM wParam, LPARAM lParam)
{
	HRESULT hResult = 0;
	UINT dwSize;
	WCHAR szTempOutput[1024];

	ZeroMemory(szTempOutput, sizeof(WCHAR) * 1024);

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL) 	return LRESULT{ 0 };

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
			raw->data.keyboard.MakeCode,
			raw->data.keyboard.Flags,
			raw->data.keyboard.Reserved,
			raw->data.keyboard.ExtraInformation,
			raw->data.keyboard.Message,
			raw->data.keyboard.VKey);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		OutputDebugString(szTempOutput);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
			raw->data.mouse.usFlags,
			raw->data.mouse.ulButtons,
			raw->data.mouse.usButtonFlags,
			raw->data.mouse.usButtonData,
			raw->data.mouse.ulRawButtons,
			raw->data.mouse.lLastX,
			raw->data.mouse.lLastY,
			raw->data.mouse.ulExtraInformation);

		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		OutputDebugString(szTempOutput);
	}

	delete[] lpb;

	return hResult;
}








