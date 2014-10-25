//----------------------------------------------------------------
//Global.cpp
//----------------------------------------------------------------
//

#include "Global.h"

Global::Global()
{

}

Global::~Global()
{

}

Cursor Global::m_cursor = Cursor();;
Cursor* Global::GetCursor(){ return &m_cursor; }

bool Global::m_godMode = false;
void Global::SetGodMode(bool value) { m_godMode = value; }
bool Global::GetGodMode() { return m_godMode; }

//Screensize Access Methods
int Global::m_screenWidth = 0;
int Global::m_screenHeight = 0;
void Global::SetScreenWidth(int width) { m_screenWidth = width; }
void Global::SetScreenHeight(int height) { m_screenHeight = height; }
int Global::GetScreenWidth() { return m_screenWidth; }
int Global::GetScreenHeight() { return m_screenHeight; }

//HWND Access Methods
HWND Global::m_hwnd;
void Global::SetHWND(HWND hwnd) { m_hwnd = hwnd; }
HWND& Global::GetHWND() { return m_hwnd; }