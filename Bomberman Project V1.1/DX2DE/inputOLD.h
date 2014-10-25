////-------------------------------------
////input.h
////-------------------------------------
//#ifndef _Input_H_
//#define _Input_H_
//
//#define DIRECTINPUT_VERSION 0x0800
//
////Linking Librarys
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//
//#include<Windowsx.h>
//#include<dinput.h>
//#include<xnamath.h>
//
//class Input
//{
//public:
//	Input();
//	Input(const Input&);
//	~Input();
//
//	bool Initialize(HINSTANCE, HWND, int, int);
//	bool Update();
//
//	bool IsEscapePressed();
//	void GetMousePosition(int&, int&);
//	XMFLOAT2 GetMousePosition();
//
//	unsigned int GetKey();
//	bool KeyDown(unsigned int);
//	bool MouseDown(unsigned int);
//	bool MouseDownOnce(unsigned int);
//
//	static int LEFT_MOUSE;
//	static int RIGHT_MOUSE;
//
//private:
//	//Methods
//	void Shutdown();
//	bool ReadKeyboard();
//	bool ReadMouse();
//	void ProcessInput();
//
//	//Variables
//	IDirectInput8* m_pDirectInput;
//	IDirectInputDevice8* m_pKeyboard;
//	IDirectInputDevice8* m_pMouse;
//
//	unsigned char m_keyboardState[256];
//	DIMOUSESTATE m_mouseState;
//
//	HWND m_hwnd;
//
//	int m_screenWidth, m_screenHeight;
//	int m_mouseX, m_mouseY;
//
//	enum MouseState { MOUSE_UP, MOUSE_DOWN };
//	MouseState e_mouse;
//
//};
//
//#endif