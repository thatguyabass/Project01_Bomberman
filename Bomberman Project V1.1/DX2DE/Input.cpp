//--------------------------------------------------------------------------
//Input.cpp
//--------------------------------------------------------------------------
//

#include "Input.h"

Input::Input()
{

}

Input::~Input()
{

}

//Initialize the Global Variables
bool Input::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_hwnd = hwnd;

	return true;
}

void Input::Update()
{
	UpdateMousePos();
	ReadInput();
}

//Letter
int Input::m_currentLetter = 0;
int Input::m_previousLetter = 0;
int Input::m_lastLetter = 0;
int Input::m_downLetter = 0;
int Input::m_upLetter = 0;

//Number
int Input::m_currentNumber = 0;
int Input::m_previousNumber = 0;
int Input::m_lastNumber = 0;
int Input::m_downNumber = 0;
int Input::m_upNumber = 0;

//Special
int Input::m_currentSpecial = 0;
int Input::m_previousSpecial = 0;
int Input::m_lastSpecial = 0;
int Input::m_downSpecial = 0;
int Input::m_upSpecial = 0;

//Mouse
int Input::m_currentMouse = 0;
int Input::m_previousMouse = 0;
int Input::m_lastMouse = 0;
int Input::m_downMouse = 0;
int Input::m_upMouse = 0;

void Input::ReadInput()
{
	//Reset the current Keys variables
	m_currentLetter = 0;
	m_currentNumber = 0;
	m_currentSpecial = 0;
	m_currentMouse = 0;

	//Get the currently used letter keys
	if(GetAsyncKeyState('A'))			m_currentLetter |= KEY_A;
	if(GetAsyncKeyState('B'))			m_currentLetter |= KEY_B;
	if(GetAsyncKeyState('C'))			m_currentLetter |= KEY_C;
	if(GetAsyncKeyState('D'))			m_currentLetter |= KEY_D;
	if(GetAsyncKeyState('E'))			m_currentLetter |= KEY_E;
	if(GetAsyncKeyState('F'))			m_currentLetter |= KEY_F;
	if(GetAsyncKeyState('G'))			m_currentLetter |= KEY_G;
	if(GetAsyncKeyState('H'))			m_currentLetter |= KEY_H;
	if(GetAsyncKeyState('I'))			m_currentLetter |= KEY_I;
	if(GetAsyncKeyState('J'))			m_currentLetter |= KEY_J;
	if(GetAsyncKeyState('K'))			m_currentLetter |= KEY_K;
	if(GetAsyncKeyState('L'))			m_currentLetter |= KEY_L;
	if(GetAsyncKeyState('M'))			m_currentLetter |= KEY_M;
	if(GetAsyncKeyState('N'))			m_currentLetter |= KEY_N;
	if(GetAsyncKeyState('O'))			m_currentLetter |= KEY_O;
	if(GetAsyncKeyState('P'))			m_currentLetter |= KEY_P;
	if(GetAsyncKeyState('Q'))			m_currentLetter |= KEY_Q;
	if(GetAsyncKeyState('R'))			m_currentLetter |= KEY_R;
	if(GetAsyncKeyState('S'))			m_currentLetter |= KEY_S;
	if(GetAsyncKeyState('T'))			m_currentLetter |= KEY_T;
	if(GetAsyncKeyState('U'))			m_currentLetter |= KEY_U;
	if(GetAsyncKeyState('V'))			m_currentLetter |= KEY_V;
	if(GetAsyncKeyState('W'))			m_currentLetter |= KEY_W;
	if(GetAsyncKeyState('X'))			m_currentLetter |= KEY_X;
	if(GetAsyncKeyState('Y'))			m_currentLetter |= KEY_Y;
	if(GetAsyncKeyState('Z'))			m_currentLetter |= KEY_Z;
	
	//Get the currently used Special Keys
	if(GetAsyncKeyState(VK_RETURN))		m_currentSpecial |= KEY_ENTER;
	if(GetAsyncKeyState(VK_SPACE))		m_currentSpecial |= KEY_SPACE;
	if(GetAsyncKeyState(VK_BACK))		m_currentSpecial |= KEY_BACK;
	if(GetAsyncKeyState(VK_ESCAPE))		m_currentSpecial |= KEY_ESCAPE;
	if(GetAsyncKeyState(VK_LSHIFT))		m_currentSpecial |= KEY_LEFT_SHIFT;
	if(GetAsyncKeyState(VK_RSHIFT))		m_currentSpecial |= KEY_RIGHT_SHIFT;
	if(GetAsyncKeyState(VK_UP))			m_currentSpecial |= KEY_UP;
	if(GetAsyncKeyState(VK_DOWN))		m_currentSpecial |= KEY_DOWN;
	if(GetAsyncKeyState(VK_RIGHT))		m_currentSpecial |= KEY_RIGHT;
	if(GetAsyncKeyState(VK_LEFT))		m_currentSpecial |= KEY_LEFT;

	//Get the Currently used Number Keys
	if(GetAsyncKeyState('1') || GetAsyncKeyState(VK_NUMPAD1))	m_currentNumber |= KEY_1;
	if(GetAsyncKeyState('2') || GetAsyncKeyState(VK_NUMPAD2))	m_currentNumber |= KEY_2;
	if(GetAsyncKeyState('3') || GetAsyncKeyState(VK_NUMPAD3))	m_currentNumber |= KEY_3;
	if(GetAsyncKeyState('4') || GetAsyncKeyState(VK_NUMPAD4))	m_currentNumber |= KEY_4;
	if(GetAsyncKeyState('5') || GetAsyncKeyState(VK_NUMPAD5))	m_currentNumber |= KEY_5;
	if(GetAsyncKeyState('6') || GetAsyncKeyState(VK_NUMPAD6))	m_currentNumber |= KEY_6;
	if(GetAsyncKeyState('7') || GetAsyncKeyState(VK_NUMPAD7))	m_currentNumber |= KEY_7;
	if(GetAsyncKeyState('8') || GetAsyncKeyState(VK_NUMPAD8))	m_currentNumber |= KEY_8;
	if(GetAsyncKeyState('9') || GetAsyncKeyState(VK_NUMPAD9))	m_currentNumber |= KEY_9;
	if(GetAsyncKeyState('0') || GetAsyncKeyState(VK_NUMPAD0))	m_currentNumber |= KEY_0;

	//Get the Currently Pressed Mouse Buttons
	if(GetAsyncKeyState(VK_LBUTTON))	m_currentMouse |= MOUSE_LEFT;
	if(GetAsyncKeyState(VK_RBUTTON))	m_currentMouse |= MOUSE_RIGHT;
	if(GetAsyncKeyState(VK_MBUTTON))	m_currentMouse |= MOUSE_MID;

	//Calculate using Bitmask to determine which keys are currently pressed and in which state
	m_lastLetter = m_currentLetter ^ m_previousLetter;
	m_downLetter = m_currentLetter & m_lastLetter;
	m_upLetter = ~m_currentLetter & m_lastLetter; 

	m_lastNumber = m_currentNumber ^ m_previousNumber;
	m_downNumber = m_currentNumber & m_lastNumber;
	m_upNumber = ~m_currentNumber & m_lastNumber;

	m_lastSpecial = m_currentSpecial ^ m_previousSpecial;
	m_downSpecial = m_currentSpecial & m_lastSpecial;
	m_upSpecial = ~m_currentSpecial & m_lastSpecial;

	m_lastMouse = m_currentMouse ^ m_previousMouse;
	m_downMouse = m_currentMouse & m_lastMouse;
	m_upMouse = ~m_currentMouse & m_lastMouse; 

	//Set the previous keys
	m_previousLetter = m_currentLetter;
	m_previousNumber = m_currentNumber;
	m_previousSpecial = m_currentSpecial;
	m_previousMouse = m_currentMouse;
}

//Letters
int Input::GetCurrentKeys() { return m_currentLetter; }
int Input::GetLastKeys() { return m_lastLetter; }
int Input::GetDownKeys() { return m_downLetter; }
int Input::GetUpKeys() { return m_upLetter; }

//Numbers
int Input::GetCurrentNumber() { return m_currentNumber; }
int Input::GetLastNumber() { return m_lastNumber; }
int Input::GetDownNumber() { return m_downNumber; }
int Input::GetUpNumber() { return m_upNumber; }

//Special
int Input::GetCurrentSpecial() { return m_currentSpecial; }
int Input::GetLastSpecial() { return m_lastSpecial; }
int Input::GetDownSpecial() { return m_downSpecial; }
int Input::GetUpSpecial() { return m_upSpecial; }

//Mouse
int Input::GetCurrentMouse() { return m_currentMouse; }
int Input::GetLastMouse() { return m_lastMouse; }
int Input::GetDownMouse() { return m_downMouse; }
int Input::GetUpMouse() { return m_upMouse; }

//Global Variables
HWND Input::m_hwnd;
int Input::m_screenWidth = 0;
int Input::m_screenHeight = 0;
int Input::m_mouseX = 0;
int Input::m_mouseY = 0;

void Input::UpdateMousePos()
{
	//Create and find position the point in the current application window 
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_hwnd, &p);
	
	//Store the Position for use
	m_mouseX = p.x + (m_screenWidth / -2);
	m_mouseY = -p.y + (m_screenHeight / 2); //Y needs to be reversed to be rendered correctly 
}

XMFLOAT2 Input::GetMousePosition()
{
	return XMFLOAT2(m_mouseX, m_mouseY);
}
void Input::GetMousePosition(int& x, int& y)
{
	x = m_mouseX;
	y = m_mouseY;
}

char Input::GetCurrentInput()
{
	//Askii offset
	int offset = 0;

	//is shift being held
	if(!(m_currentSpecial & KEY_LEFT_SHIFT))
		offset = 32;

	//Get the current input and return the value of the key
	if(m_downLetter & KEY_A)	return ('A' + offset);
	if(m_downLetter & KEY_B)	return ('B' + offset);
	if(m_downLetter & KEY_C)	return ('C' + offset);
	if(m_downLetter & KEY_D)	return ('D' + offset);
	if(m_downLetter & KEY_E)	return ('E' + offset);
	if(m_downLetter & KEY_F)	return ('F' + offset);
	if(m_downLetter & KEY_G)	return ('G' + offset);
	if(m_downLetter & KEY_H)	return ('H' + offset);
	if(m_downLetter & KEY_I)	return ('I' + offset);
	if(m_downLetter & KEY_J)	return ('J' + offset);
	if(m_downLetter & KEY_K)	return ('K' + offset);
	if(m_downLetter & KEY_L)	return ('L' + offset);
	if(m_downLetter & KEY_M)	return ('M' + offset);
	if(m_downLetter & KEY_N)	return ('N' + offset);
	if(m_downLetter & KEY_O)	return ('O' + offset);
	if(m_downLetter & KEY_P)	return ('P' + offset);
	if(m_downLetter & KEY_Q)	return ('Q' + offset);
	if(m_downLetter & KEY_R)	return ('R' + offset);
	if(m_downLetter & KEY_S)	return ('S' + offset);
	if(m_downLetter & KEY_T)	return ('T' + offset);
	if(m_downLetter & KEY_U)	return ('U' + offset);
	if(m_downLetter & KEY_V)	return ('V' + offset);
	if(m_downLetter & KEY_W)	return ('W' + offset);
	if(m_downLetter & KEY_X)	return ('X' + offset);
	if(m_downLetter & KEY_Y)	return ('Y' + offset);
	if(m_downLetter & KEY_Z)	return ('Z' + offset);
	
	//Get the current number input + Special key values
	char number = GetCurrentNumberInput();
	if(number != '\0') return number;

	return '\0';
}

char Input::GetCurrentNumberInput()
{
	if(m_downSpecial & KEY_SPACE)	return (' ');
	if(m_downSpecial & KEY_BACK)	return ('\b');
	
	if(m_downNumber & KEY_1)	return ('1');
	if(m_downNumber & KEY_2)	return ('2');
	if(m_downNumber & KEY_3)	return ('3');
	if(m_downNumber & KEY_4)	return ('4');
	if(m_downNumber & KEY_5)	return ('5');
	if(m_downNumber & KEY_6)	return ('6');
	if(m_downNumber & KEY_7)	return ('7');
	if(m_downNumber & KEY_8)	return ('8');
	if(m_downNumber & KEY_9)	return ('9');
	if(m_downNumber & KEY_0)	return ('0');

	return '\0';
}