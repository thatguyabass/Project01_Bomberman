////--------------------------
////Input.cpp
////--------------------------
////Define the Input Class 
//
//#include "input.h"
//
//Input::Input()
//{
//	m_pDirectInput = 0;
//	m_pKeyboard = 0;
//	m_pMouse = 0;
//
//	e_mouse = MOUSE_UP;
//}
//
//Input::Input(const Input& src)
//{
//
//}
//
//Input::~Input()
//{
//	Shutdown();
//}
//
//int Input::LEFT_MOUSE = 0;
//int Input::RIGHT_MOUSE = 1;
//
//bool Input::Initialize(HINSTANCE instance, HWND hwnd, int screenWidth, int screenHeight)
//{
//	HRESULT result;
//
//	//Store Screen Size, Used for calculating mouse position
//	m_screenWidth = screenWidth;
//	m_screenHeight = screenHeight;
//
//	//Initialize the mouse position
//	m_mouseX = 0;
//	m_mouseY = 0;
//
//	//RAWINPUT
//
//	//Initialize the main direct input interface
//	result = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
//	if(FAILED(result))
//		return false;
//
//	//Initialize the direct input interface for the keyboard
//	result = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
//	if(FAILED(result))
//		return false;
//
//	//Set the Keyboard Data format. use the predefined data format because it is a keyboard 
//	result = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
//	if(FAILED(result))
//		return false;
//
//	//Set the cooperative level of the keyboard to not share with other programs
//	result = m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//	if(FAILED(result))
//		return false;
//
//	//Now Acquire the keyboard
//	result = m_pKeyboard->Acquire();
//	if(FAILED(result))
//		return false;
//
//	//Initialize the direct input interface for the mouse
//	result = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
//	if(FAILED(result))
//		return false;
//
//	//Set the Mouse Data format
//	result = m_pMouse->SetDataFormat(&c_dfDIMouse);
//	if(FAILED(result))
//		return false;
//
//	//Set the cooperative level of the mouse to share with other programs
//	result = m_pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//	if(FAILED(result))
//		return false;
//
//	//Acquire the mouse
//	result = m_pMouse->Acquire();
//	if(FAILED(result))
//		return false;
//
//	m_hwnd = hwnd;
//
//	return true;
//}
//
//void Input::Shutdown()
//{
//	//Release the Keyboard
//	if(m_pKeyboard)
//	{
//		m_pKeyboard->Unacquire();
//		m_pKeyboard->Release();
//		m_pKeyboard = 0;
//	}
//
//	//Release the Mouse
//	{
//		m_pMouse->Unacquire();
//		m_pMouse->Release();
//		m_pMouse = 0;
//	}
//
//	//Release Direct Input
//	if(m_pDirectInput)
//	{
//		m_pDirectInput->Release();
//		m_pDirectInput = 0;
//	}
//}
//
//bool Input::Update()
//{
//	bool result;
//
//	//Read the current State of the Keyboard
//	result = ReadKeyboard();
//	if(!result)
//		return false;
//
//	//Read the currecnt State of the mouse
//	result = ReadMouse();
//	if(!result)
//		return false;
//
//	//Process the changes 
//	ProcessInput();
//
//	return true;
//}
//
//bool Input::ReadKeyboard()
//{
//	HRESULT result;
//
//	//Read the Keyboard Device
//	result = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
//	if(FAILED(result))
//	{
//		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
//			m_pKeyboard->Acquire();
//		else
//			return false;
//	}
//
//	return true;
//}
//
//bool Input::ReadMouse()
//{
//	HRESULT result;
//
//	//Read the Mouse Device
//	result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
//	if(FAILED(result))
//	{
//		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
//			m_pMouse->Acquire();
//		else
//			return false;
//	}
//
//	return true;
//}
//
//void Input::ProcessInput()
//{
//	//Update the location of the mouse cursor based on the change of the mouse location during the frame
//	POINT p;
//	GetCursorPos(&p);
//	ScreenToClient(m_hwnd, &p);
//	
//	m_mouseX = p.x + (m_screenWidth / -2);
//	m_mouseY = -p.y + (m_screenHeight / 2);
//}
//
//bool Input::IsEscapePressed()
//{
//	//Do a bitwise and on the keyboard state to check if the escape key is currently pressed
//	if(m_keyboardState[DIK_ESCAPE] & 0x80)
//		return true;
//		
//	return false;
//}
//
//void Input::GetMousePosition(int& x, int& y)
//{
//	x = m_mouseX;
//	y = m_mouseY;
//}
//XMFLOAT2 Input::GetMousePosition()
//{
//	return XMFLOAT2(m_mouseX, m_mouseY);
//}
//unsigned int Input::GetKey()
//{
//	for(int c = 0; c < 256; c++)
//	{
//
//		if(KeyDown(c))
//			return c;
//	}
//
//	return -1;
//}
//
//bool Input::KeyDown(unsigned int key)
//{
//	//if a key is pressed then save that state in the key array
//	if(m_keyboardState[key] & 0x80)
//		return true;
//
//	return false;
//}
//
//bool Input::MouseDown(unsigned int mouse)
//{
//	if(m_mouseState.rgbButtons[mouse] & 0x80)
//		return true;
//
//	return false;
//}
//bool Input::MouseDownOnce(unsigned int mouse)
//{
//	if(m_mouseState.rgbButtons[mouse] & 0x80)
//	{
//		if(e_mouse == MOUSE_UP)
//			return true;
//		else
//			return false;
//	}
//
//	e_mouse = MOUSE_DOWN;
//
//	return false;
//}