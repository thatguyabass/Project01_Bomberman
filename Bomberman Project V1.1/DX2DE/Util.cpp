//---------------------------------------------------------
//Util.cpp
//---------------------------------------------------------
//

#include "Util.h"

Util::Util()
{
	m_flags.back = false;
	m_flags.exit = false;
}
Util::~Util()
{

}
Util::Vec2 Util::m_aspectRatio;
Util::Vec2 Util::GetAspectRatio()
{
	return m_aspectRatio;
}
void Util::SetAspectRatio(int screenWidth, int screenHeight) 
{
	m_aspectRatio.x = (float)screenWidth / (float)IDEAL_WIDTH;
	m_aspectRatio.y = (float)screenHeight / (float)IDEAL_HEIGHT;
}

void Util::ReadFolder(vector<string>& str, wstring directory)
{
	WIN32_FIND_DATA data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Find the first file in the directory.
	hFind = FindFirstFile(directory.c_str(), &data);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do
		{
			wstring ws = data.cFileName;
			string s(ws.begin(), ws.end());
			str.push_back(s);

		}while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

Util::GLOBAL_GAME_STATE Util::e_globalGame = MENU;
Util::GLOBAL_START_MENU_STATE Util::e_globalMenu = CLOSED;

void Util::ToggleMenu()
{
	switch(e_globalMenu)
	{
	case CLOSED: e_globalMenu = OPEN; break;
	case OPEN:
	default: e_globalMenu = CLOSED; break;
	}
}

Util::MenuFlags Util::m_flags = Util::MenuFlags();
Util::MenuFlags& Util::GetFlags()
{
	return m_flags;
}

void Util::ResetMenuFlags()
{
	m_flags.back = false;
}