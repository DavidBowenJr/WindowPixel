#pragma once


// https://bell0bytes.eu/directinput/


#ifndef  _INPUTCLASS_H
#define _INPUTCLASS_H

#define DIRECTINPUT_VERSION 0X0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>


class InputClass
{

	// Example to retrieve the capabilities of the game controller:
	// We are mostly interested in the dwAxes and swButtons members,
	// They specify the number of axis and buttons available on the game controller.
	// maybe the hat if one exzist.

	// Container std::vector
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
	bool IsJoyExt();

	bool GetJoyCapabilities(); // singlar for now.
	bool SetupJoyParameters();


	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool ReadJoyStick();
	void ProcessInput();

private:
	IDirectInput8W* m_directInput;
	IDirectInputDevice8W* m_keyboard;
	IDirectInputDevice8W* m_mouse;

	IDirectInputDevice8W* m_joystick;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	DIJOYSTATE  m_joyState;

	// Joystick Properties
	DIDEVCAPS   m_joyCapabilities; // IDirectInput8::GetCapabilities method:

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

};

#endif

