//--------------------------------------------------------------------------------
//Global.h
//--------------------------------------------------------------------------------
//This class will act as a Container for all the Global Objects

#ifndef _GLOBAL_BOMBERMAN_OBJECTS_H_
#define _GLOBAL_BOMBERMAN_OBJECTS_H_

#include "Cursor.h"

static class Global
{
public:
	Global();
	~Global();

	static Cursor* GetCursor();

	static void SetGodMode(bool);
	static bool GetGodMode();

	//Screen size Access Methods
	static void SetScreenWidth(int);
	static void SetScreenHeight(int);
	static int GetScreenWidth();
	static int GetScreenHeight();

	//HWND access methods
	static void SetHWND(HWND);
	static HWND& GetHWND();

private:
	//Variables
	static Cursor m_cursor;

	static bool m_godMode;

	static int m_screenWidth;
	static int m_screenHeight;

	static HWND m_hwnd;

};

#endif//_GLOBAL_BOMBERMAN_OBJECTS_H_