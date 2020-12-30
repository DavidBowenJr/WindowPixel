#include "framework.h"

#include "InputClass.h"

//  http://www.rastertek.com/dx11tut13.html



InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	
	// Store the screen size with will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result)) { return false; }

	// Initialize the direct input interface for the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format. In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) { return false; }

	// Set the cooperative level of the keyborad to not share with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) { return false;  }

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result)) { return false;  }

////////////////////////////////////////////////////////////////////////////////////////////

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}


	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}
//////////////////////////////////////////////////////////////////////////

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_Joystick, &m_joystick, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_joystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the joystick to share with other programs.
	result = m_joystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE  );
	if (FAILED(result))
	{
		return false;
	}

	this->GetJoyCapabilities();


	// Acquire the joystick
	result = m_joystick->Acquire();
	if (FAILED(result))
	{
		return false;
	}



	return true;

}

void InputClass::Shutdown()
{
	// Release the joystick
	if (m_joystick)
	{
		m_joystick->Unacquire();
		m_joystick->Release();
		m_joystick = 0;
	}

	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Read the current state of the joystick.
	result = ReadJoyStick();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_H] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsJoyExt()
{

	this->ReadJoyStick();

	

	// Still have to get general info about this.
	//OutputDebugString(L" We Have : ");
	//OutputDebugString( std::to_wstring(nAxes[0]).c_str());
	//OutputDebugString(L" | number of Axis ");


	//OutputDebugString(L"\n The number of Buttons on this joystick : ");
	//OutputDebugString(std::to_wstring(this->m_joyCapabilities.dwButtons).c_str());
	//OutputDebugString(L"\n");

	//OutputDebugString(L"");
	//OutputDebugString(L"\n");

	//OutputDebugString(std::to_wstring(m_joyState.rgbButtons[0]).c_str() );
	if (m_joyState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	if (m_joyCapabilities.dwAxes == 2)
	{
		this->jlx = m_joyState.lX;
		this->jly = m_joyState.lY;
	}
	if (m_joyCapabilities.dwAxes == 3)
	{
		this->jlx = m_joyState.lX;
		this->jly = m_joyState.lY;
		this->jlz = m_joyState.lZ; 
	}

	return false;
}

bool InputClass :: GetJoyCapabilities()
{
	this->m_joystick->Unacquire();
	//this->SetupJoyParameters();

	// For now im just doing a single joystick we can enumerate but kiss for now
	this->m_joyCapabilities.dwSize = sizeof(DIDEVCAPS);
	HRESULT hResult = m_joystick->GetCapabilities(&m_joyCapabilities);
	if (FAILED(hResult))
	{
		 throw "Critical error: Unable to get game controller capabilities!";
	}
	else
	{
		
		nAxes.push_back(m_joyCapabilities.dwAxes);
		
		//www
		this->m_joystick->Acquire();
		
		if (this->SetupJoyParameters()) {};


	}

}

bool InputClass::SetupJoyParameters()
{
	this->m_joystick->Unacquire();
	// Set the range of the joystick axes tp[-1000, + 1000]
	DIPROPRANGE directInputPropertyRange;

	ZeroMemory(&directInputPropertyRange, sizeof(DIPROPRANGE));
	directInputPropertyRange.diph.dwSize = sizeof(DIPROPRANGE);
	directInputPropertyRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	directInputPropertyRange.diph.dwHow = DIPH_BYOFFSET;
	directInputPropertyRange.lMin = -10;
	directInputPropertyRange.lMax = +10;

	directInputPropertyRange.diph.dwObj = DIJOFS_X;
	this->m_joystick->SetProperty(DIPROP_RANGE, &directInputPropertyRange.diph);

	directInputPropertyRange.diph.dwObj = DIJOFS_Y;
	this->m_joystick->SetProperty(DIPROP_RANGE, &directInputPropertyRange.diph);


	


	//note Supports 3 then Z

	// Set the dead zone for the joystick axes (because many joysticks are not perfectly calibrated to be zero when centered).
	
#if 0
	DIPROPDWORD dipdw;
	ZeroMemory(&dipdw, sizeof(DIPROPDWORD));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 1000; // 1000 = 10%

	dipdw.diph.dwObj = DIJOFS_X; // Set the x-axis deadzone
	this->m_joystick->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	dipdw.diph.dwObj = DIJOFS_Y; // Set the y-axis deadzone
	this->m_joystick->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
#endif



	this->m_joystick->Acquire();

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool InputClass::ReadJoyStick()
{
	HRESULT result;

	// Read the joystick device.
	result = m_joystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&m_joyState);
	if (FAILED(result))
	{
		// If the joy lost focuc or was not acqured then try to get contral back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_joystick->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void InputClass::ProcessInput()
{

	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}
