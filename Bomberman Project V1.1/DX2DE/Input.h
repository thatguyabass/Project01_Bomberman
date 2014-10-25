//-----------------------------------------------------------------------
//Input.h
//-----------------------------------------------------------------------
//This class will handle the input for every key used in the game
//This will also be a generic Win32 Input class that should be able to be ported into any C++ program

#ifndef _WIN_32_INPUT_H_
#define _WIN_32_INPUT_H_

#include<Windows.h>
#include<xnamath.h>
using namespace std;

//---------------------------------------------------------------------------------------

//Mask the enum values to simulate Binary which will be used to determine if the key has been pressed down, Held or released
enum Keys
{
	//LETTER KEYS
	KEY_A =			0x00000001,
	KEY_B =			0x00000002,
	KEY_C =			0x00000004,
	KEY_D =			0x00000008,

	KEY_E =			0x00000010,
	KEY_F =			0x00000020,
	KEY_G =			0x00000040,
	KEY_H =			0x00000080,
	
	KEY_I =			0x00000100,
	KEY_J =			0x00000200,
	KEY_K =			0x00000400,
	KEY_L =			0x00000800,
	
	KEY_M =			0x00001000,
	KEY_N =			0x00002000,
	KEY_O =			0x00004000,
	KEY_P =			0x00008000,
	
	KEY_Q =			0x00010000,
	KEY_R =			0x00020000,
	KEY_S =			0x00040000,
	KEY_T =			0x00080000,
	
	KEY_U =			0x00100000,
	KEY_V =			0x00200000,
	KEY_W =			0x00400000,
	KEY_X =			0x00800000,
	
	KEY_Y =			0x01000000,
	KEY_Z =			0x02000000,

	//NUMBER KEYS
	KEY_1 =			0x00000001,
	KEY_2 =			0x00000002,
	KEY_3 =			0x00000004,
	KEY_4 =			0x00000008,

	KEY_5 =			0x00000010,
	KEY_6 =			0x00000020,
	KEY_7 =			0x00000040,
	KEY_8 =			0x00000080,
	
	KEY_9 =			0x00000100,
	KEY_0 =			0x00000200,

	//SPECIAL KEYS
	KEY_ENTER =		0x00000001,
	KEY_SPACE =		0x00000002,
	KEY_BACK =		0x00000004,
	KEY_ESCAPE =	0x00000008,

	KEY_LEFT_SHIFT =0x00000010,
	KEY_RIGHT_SHIFT=0x00000020,
	KEY_UP =		0x00000040,
	KEY_DOWN =		0x00000080,

	KEY_RIGHT =		0x00000100,
	KEY_LEFT =		0x00000200
};

//Mask the Enum value to simulate binary
enum Mouse
{
	MOUSE_LEFT =	0x00000001,
	MOUSE_MID =		0x00000002,
	MOUSE_RIGHT =	0x00000004,
};

static class Input
{
public:
	Input();
	~Input();

	static bool Initialize(int, int, HWND);

	//Access Methods for Letter Keys
	static int GetCurrentKeys();
	static int GetLastKeys();
	static int GetDownKeys();
	static int GetUpKeys();

	//Access Methods for Number Keys - Includes Num pad keys
	static int GetCurrentNumber();
	static int GetLastNumber();
	static int GetDownNumber();
	static int GetUpNumber();

	//Access Methods for Special Keys - Enter, Escape, Space
	static int GetCurrentSpecial();
	static int GetLastSpecial();
	static int GetDownSpecial();
	static int GetUpSpecial();

	//Access Methods for the Mouse buttons
	static int GetCurrentMouse();
	static int GetLastMouse();
	static int GetDownMouse();
	static int GetUpMouse();

	//Mouse Position Access Methods
	static void UpdateMousePos();
	static XMFLOAT2 GetMousePosition();
	static void GetMousePosition(int& x, int& y);

	//Get the current Key presses
	static void Update();

	//Get the character from the key pressed
	static char GetCurrentInput(); 
	static char GetCurrentNumberInput();

private:
	//Methods
	//Read the keys to determine if they have been pressed
	static void ReadInput();
	
	//Variables
	//Letter
	static int m_currentLetter;
	static int m_previousLetter;
	static int m_lastLetter;
	static int m_upLetter;
	static int m_downLetter;

	//Number
	static int m_currentNumber;
	static int m_previousNumber;
	static int m_lastNumber;
	static int m_upNumber;
	static int m_downNumber;

	//Special
	static int m_currentSpecial;
	static int m_previousSpecial;
	static int m_lastSpecial;
	static int m_upSpecial;
	static int m_downSpecial;

	//Mouse
	static int m_previousMouse;
	static int m_currentMouse;
	static int m_lastMouse;
	static int m_downMouse;
	static int m_upMouse;

	//Global Variables
	static HWND m_hwnd;
	static int m_screenWidth;
	static int m_screenHeight;
	static int m_mouseX;
	static int m_mouseY;

};

#endif//_WIN_32_INPUT_H_