#include "framework.h"

#include "InputClass.h"

//  http://www.rastertek.com/dx11tut13.html

static int enum_index = 0;
static	BOOL hasEnhanced;
static inline BOOL CALLBACK DIEnumKbdCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

static inline  BOOL CALLBACK DIEnumJoyCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

//static IDirectInput8W* gm_directInput;
//static IDirectInputDevice8W* gm_joystick;
// we could  say  gm_directInput = this->m_directInput
//  ...EM
//  this->m_directInput = gm_directInput 

// But lets not lets take advantage of a pointer holding another pointers address....
// We will let the class do clean up we should our pp_gm_ as if they where private......

// These are some of the globals im using to help Create a device in an emulator 
// Treating as if Pointer To Pointer to store the local address  m_directInput and m_joystick  so to make a little simpler
static LPDIRECTINPUT8W* pp_gm_directInput;
static  GUID* pp_KeyboardGuid[4] ;

static LPDIRECTINPUTDEVICE8W* pp_gm_keyboard[2];
static LPDIRECTINPUTDEVICE8W* pp_gm_joystick;

// g_didiKeyboard;
static DIDEVICEINSTANCE g_didiKeyboard[2];

static DIDEVICEINSTANCE g_joyInstance; // strings

InputClass::InputClass()
{
	m_directInput = 0;
	m_joystick = 0;
	m_keyboard[0] = 0;
	m_keyboard[1] = 0;
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
	HRESULT rst;

	// Store the screen size with will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;


	// Alias with pointer to pointer relay communication.
	pp_gm_directInput = &m_directInput;
	pp_gm_joystick = &m_joystick;

	//pp_KeyboardGuid = &KeyboardGuid;
	pp_KeyboardGuid[0] = &KeyboardGuid[0];
	pp_KeyboardGuid[1] = &KeyboardGuid[1];


	//g_didiKeyboard[0] = this->m_didiKeyboard[0];
	//g_didiKeyboard[1] = this->m_didiKeyboard[1];




	pp_gm_keyboard[0] = &m_keyboard[0];
	pp_gm_keyboard[1] = &m_keyboard[1];


	if (FAILED(rst = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL))) { return false; }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//LPCDIDEVICEINSTANCE g_joyInstance = NULL;
	if (FAILED(rst = m_directInput->EnumDevices(DI8DEVTYPE_KEYBOARD, (LPDIENUMDEVICESCALLBACKW)&DIEnumKbdCallback, (void**)&KeyboardGuid[0], DIEDFL_ATTACHEDONLY))) { return false; }

	this->m_didiKeyboard[0] = InputClass::FillDirectInputDeviceInstance(m_didiKeyboard[0], &g_didiKeyboard[0]);






	//if (FAILED(rst = m_directInput->CreateDevice(g_didiKeyboard.guidInstance, &m_keyboard[0], NULL))) { return false; }

	//  if (FAILED( rst = m_directInput->CreateDevice( KeyboardGuid[0], &m_keyboard[0], NULL) )) { return false; }

	
	
	if (FAILED(	rst = m_keyboard[0]->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE) )) { return false; }

	if (FAILED( rst = m_keyboard[0]->SetDataFormat(&c_dfDIKeyboard) )) { return false; }

	if (FAILED( rst = m_keyboard[0]->Acquire() )) { if (FAILED(rst = m_keyboard[0]->Poll())) { return false; } }
	
	{
		CHAR czstr[260];
		wsprintfA(czstr, "\n First Instance Name %s ", m_didiKeyboard[0].tszInstanceName);
		std::string wstr; wstr.append(czstr);
		
		wsprintfA(czstr, "\t First guid Product  %d \n", m_didiKeyboard[0].guidProduct);
		wstr.append(czstr);
		OutputDebugStringA(wstr.c_str());
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	if (FAILED(rst = m_directInput->EnumDevices(DI8DEVTYPE_KEYBOARD, (LPDIENUMDEVICESCALLBACKW)&DIEnumKbdCallback, (void**)&KeyboardGuid[1], DIEDFL_ATTACHEDONLY))) { return false; }

	//m_didiKeyboard[1] = InputClass::FillDirectInputDeviceInstance(m_didiKeyboard[1], &g_didiKeyboard[1]);

//	if (FAILED(rst = m_directInput->CreateDevice(KeyboardGuid[1], &m_keyboard[1], NULL))) { return false; }
	// 
	
	m_didiKeyboard[1] = FillDirectInputDeviceInstance(m_didiKeyboard[1], &g_didiKeyboard[1]);

	if (FAILED(rst = m_keyboard[1]->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) { return false; }

	if (FAILED(rst = m_keyboard[1]->SetDataFormat(&c_dfDIKeyboard))) { return false; }

	if (FAILED(rst = m_keyboard[1]->Acquire())) { if (FAILED(rst = m_keyboard[1]->Poll())) { return false; } }

//	m_didiKeyboard[1] = FillDirectInputDeviceInstance(m_didiKeyboard[1], &g_didiKeyboard[1]);

	{
		CHAR czstr[260];
		wsprintfA(czstr, "\n Second Instance Name %s ", m_didiKeyboard[1].tszInstanceName);
		std::string wstr; wstr.append(czstr);

		wsprintfA(czstr, "\t Second guid Product  %d \n\n", m_didiKeyboard[1].guidProduct);
		wstr.append(czstr);
		OutputDebugStringA(wstr.c_str());
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	
	



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (FAILED( rst = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL) )) { return false; } 

	if (FAILED( rst = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) { return false; }

	if (FAILED( rst = m_mouse->SetDataFormat(&c_dfDIMouse) )) { return false; }

	if (FAILED( rst = m_mouse->Acquire() )) { if (FAILED(rst = m_mouse->Poll())) { return false; } }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
	if (FAILED( rst = m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACKW) &DIEnumJoyCallback, &JoystickGuid, DIEDFL_ATTACHEDONLY) )) { return false; }
	
	if (FAILED( rst = m_joystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE) )) { return false; }
	
	if (FAILED( rst = m_joystick->SetDataFormat(&c_dfDIJoystick) )) { return false; }

	GetJoyCapabilities();

	if (FAILED(rst = m_joystick->Acquire() )) { if (FAILED(rst = m_joystick->Poll() )) { return false; } }

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
	if (m_keyboard[0])
	{
		m_keyboard[0]->Unacquire();
		m_keyboard[0]->Release();
		m_keyboard[0] = 0;
	}
	if (m_keyboard[1])
	{
		m_keyboard[1]->Unacquire();
		m_keyboard[1]->Release();
		m_keyboard[1] = 0;
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

	if (m_keyboardState2[DIK_H] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsJoyEtc()
{




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
	
	
	/*
	if (m_joyState.rgbButtons[0] & 0x80)
	{
		return true;
	}
	*/

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
		this->JHat = m_joyState.rgdwPOV[0];
		this->jSlider1 = m_joyState.rglSlider[0];
		//	this->jSlider2 = m_joyState.rglSlider[1];	
	}

	memcpy(JButton, m_joyState.rgbButtons, sizeof(BYTE) * 32);


	return false;
}

bool InputClass::GetJoyCapabilities()
{
	m_joystick->Unacquire();
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

		//this->
			m_joystick->Acquire();

		if (this->SetupJoyParameters()) {};
	}
}

bool InputClass::SetupJoyParameters()
{
		m_joystick->Unacquire();
	// Set the range of the joystick axes tp[-1000, + 1000]
	DIPROPRANGE directInputPropertyRange;

	ZeroMemory(&directInputPropertyRange, sizeof(DIPROPRANGE));
	directInputPropertyRange.diph.dwSize = sizeof(DIPROPRANGE);
	directInputPropertyRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	directInputPropertyRange.diph.dwHow = DIPH_BYOFFSET;
	directInputPropertyRange.lMin = -10;
	directInputPropertyRange.lMax = +10;

	directInputPropertyRange.diph.dwObj = DIJOFS_X;
	m_joystick->SetProperty(DIPROP_RANGE, &directInputPropertyRange.diph);


	// we can also set independent Axis
	directInputPropertyRange.lMin = -100;
	directInputPropertyRange.lMax = +100;

	directInputPropertyRange.diph.dwObj = DIJOFS_Y;
	m_joystick->SetProperty(DIPROP_RANGE, &directInputPropertyRange.diph);

	directInputPropertyRange.lMin = -1000;
	directInputPropertyRange.lMax = +1000;
	directInputPropertyRange.diph.dwObj = DIJOFS_Z;
	m_joystick->SetProperty(DIPROP_RANGE, &directInputPropertyRange.diph);

	//note Supports 3 then Z
	// Set the dead zone for the joystick axes (because many joysticks are not perfectly calibrated to be zero when centered).

#if 1
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



	
	m_joystick->Acquire();

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
	result = m_keyboard[0]->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	{
		CHAR czstr[260];
		wsprintfA(czstr, "\n  Instance Name %s ", m_didiKeyboard[0].tszInstanceName);
		std::string wstr; wstr.append(czstr);

		wsprintfA(czstr, "\t  guid Product  %d \n\n", m_didiKeyboard[0].guidProduct);
		wstr.append(czstr);

		wsprintfA(czstr, "\t  guid Instance  %d \n\n", m_didiKeyboard[0].guidInstance);
		wstr.append(czstr);

		OutputDebugStringA(wstr.c_str());
	}
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			if (!m_keyboard[0]->Acquire()) { m_keyboard[0]->Poll(); }
		}
		else
		{
			return false;
		}	
	}
	

	////////////////////////////////////////////////
#if 0
	result = m_keyboard[1]->GetDeviceState(sizeof(m_keyboardState2), (LPVOID)&m_keyboardState2);
	

//result =	m_keyboard[1]->GetDeviceInfo((LPDIDEVICEINSTANCEW) &m_didiKeyboard[1]);
		//	OutputDebugString(TEXT(m_didiKeyboard[1].tszInstanceName));

			{
				CHAR czstr[260];
				wsprintf(czstr, "\n Second Instance Name %s ", m_didiKeyboard[1].tszInstanceName);
				std::string wstr; wstr.append(czstr);

				wsprintf(czstr, "\t Second guid Product  %d \n\n", m_didiKeyboard[1].guidProduct);
				wstr.append(czstr);
				
				wsprintf(czstr, "\t Second guid Instance  %d \n\n", m_didiKeyboard[0].guidInstance);
				wstr.append(czstr);

				OutputDebugString(wstr.c_str());
			}

	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard[1]->Acquire();
		}
		else
		{		
			return false;
		}
	}
	OutputDebugString(TEXT(m_didiKeyboard[1].tszInstanceName));
#endif

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

DIDEVICEINSTANCE InputClass::FillDirectInputDeviceInstance(DIDEVICEINSTANCE& didiKeyboard, LPCDIDEVICEINSTANCE lpddi)
{
	ZeroMemory(&didiKeyboard, sizeof(DIDEVICEINSTANCE));
	memcpy(&didiKeyboard, lpddi, sizeof(DIDEVICEINSTANCE));         ///lpddi));
	memcpy(&didiKeyboard.dwDevType, &lpddi->dwDevType, sizeof(DWORD));
	memcpy(&didiKeyboard.dwSize, &lpddi->dwSize, sizeof(DWORD));
	memcpy(&didiKeyboard.guidFFDriver, &lpddi->guidFFDriver, sizeof(GUID));
	memcpy(&didiKeyboard.guidInstance, &lpddi->guidInstance, sizeof(GUID));
	memcpy(&didiKeyboard.guidProduct, &lpddi->guidProduct, sizeof(GUID));
	memcpy(&didiKeyboard.tszProductName, lpddi->tszProductName, sizeof(CHAR) * MAX_PATH);
	memcpy(&didiKeyboard.tszInstanceName, lpddi->tszInstanceName, sizeof(CHAR) * MAX_PATH);
	memcpy(&didiKeyboard.wUsage, &lpddi->wUsage, sizeof(WORD));
	memcpy(&didiKeyboard.wUsagePage, &lpddi->wUsagePage, sizeof(WORD));

	return didiKeyboard;
}

BOOL CALLBACK DIEnumKbdCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	*(GUID*)pvRef =  lpddi->guidInstance;
	//hasEnhanced = FALSE;
	if (GET_DIDEVICE_SUBTYPE(lpddi->dwDevType) == DI8DEVTYPEKEYBOARD_PCENH)
	{
		g_didiKeyboard[enum_index] = InputClass::FillDirectInputDeviceInstance(g_didiKeyboard[enum_index], lpddi);

		/*
		ZeroMemory(&g_didiKeyboard[enum_index], sizeof(DIDEVICEINSTANCE));     //   g_didiKeyboard[enum_index]));
		memcpy(&g_didiKeyboard[enum_index], lpddi, sizeof(DIDEVICEINSTANCE)); //              lpddi));
		memcpy(&g_didiKeyboard[enum_index].dwDevType, &lpddi->dwDevType, sizeof(DWORD));
		memcpy(&g_didiKeyboard[enum_index].dwSize, &lpddi->dwSize, sizeof(DWORD));
		memcpy(&g_didiKeyboard[enum_index].guidFFDriver, &lpddi->guidFFDriver, sizeof(GUID));
		memcpy(&g_didiKeyboard[enum_index].guidInstance, &lpddi->guidInstance, sizeof(GUID));
		memcpy(&g_didiKeyboard[enum_index].guidProduct, &lpddi->guidProduct, sizeof(GUID));
		memcpy(&g_didiKeyboard[enum_index].tszProductName, lpddi->tszProductName, sizeof(WCHAR) * MAX_PATH);
		memcpy(&g_didiKeyboard[enum_index].tszInstanceName, lpddi->tszInstanceName, sizeof(WCHAR)* MAX_PATH);
		memcpy(&g_didiKeyboard[enum_index].wUsage, &lpddi->wUsage, sizeof(WORD));
		memcpy(&g_didiKeyboard[enum_index].wUsagePage, &lpddi->wUsagePage, sizeof(WORD));
		*/
		
		 (*pp_gm_directInput)->CreateDevice( lpddi->guidInstance , &*pp_gm_keyboard[enum_index], NULL);
		 enum_index++;
	//	hasEnhanced = TRUE;
		return  DIENUM_STOP;
	}
	return  DIENUM_STOP; // DIENUM_CONTINUE;
}
         
BOOL  CALLBACK DIEnumJoyCallback(  LPCDIDEVICEINSTANCE  lpddi, LPVOID pvRef)
{
	HRESULT hr = S_OK;
	(*pp_gm_directInput)->CreateDevice(lpddi->guidInstance, &*pp_gm_joystick, NULL);

	// this->m_directInput->CreateDevice(lpddi->guidInstance, &this->m_joystick, NULL); // Similar but we can't access unless global so we op for pointer to pointer from our global to Address our InputClass.... 
	if (FAILED(hr))
		return DIENUM_CONTINUE;

	return DIENUM_STOP;
}


