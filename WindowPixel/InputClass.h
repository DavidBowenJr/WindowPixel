#pragma once


// https://bell0bytes.eu/directinput/


#ifndef  _INPUTCLASS_H
#define _INPUTCLASS_H

#define DIRECTINPUT_VERSION 0X0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

static	BOOL hasEnhanced;
static	inline BOOL CALLBACK DIEnumKbdCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

static inline  BOOL CALLBACK DIEnumJoyCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

class InputClass
{

	std::vector<unsigned int> nAxes;
public:
	LONG jlx = 0;
	LONG jly = 0;
	LONG jlz = 0;
	LONG JHat = 0;
	LONG jSlider1 = 0;
	LONG jSlider2 = 0;
	BYTE JButton[32];

public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsJoyEtc(); // etcetera  so on.

	bool GetJoyCapabilities(); // singlar for now.
	bool SetupJoyParameters();


	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool ReadJoyStick();
	void ProcessInput();

public:




	IDirectInput8W* m_directInput;
 	IDirectInputDevice8W* m_joystick;



	IDirectInputDevice8W* m_keyboard;
	IDirectInputDevice8W* m_mouse;





	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	DIJOYSTATE  m_joyState;

	// Joystick Properties
	DIDEVCAPS   m_joyCapabilities; // IDirectInput8::GetCapabilities method:

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

public:
	GUID KeyboardGuid;
	GUID JoystickGuid;



};

#endif