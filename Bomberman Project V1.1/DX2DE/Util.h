//-----------------------------------------------------------------------
//Util.h
//-----------------------------------------------------------------------
//Define Util static variables

#ifndef _BOMBERMAN_Util_H_
#define _BOMBERMAN_Util_H_

#include<Windows.h>
#include<string>
#include<vector>
using namespace std;

//------------------------------------------------------------------------------------

#define ASPECT Util::GetAspectRatio()
#define FLAG_BACK Util::GetFlags().back
#define FLAG_EXIT Util::GetFlags().exit
#define _DEBUG_PATHFINDING false

static class Util
{
public:
	Util();
	~Util();

	static struct Vec2
	{
		float x;
		float y;
	};
	
	static Vec2 GetAspectRatio();
	static void SetAspectRatio(int, int);

	static void ReadFolder(vector<string>&, wstring);//Read the Files found in a Folder

	static enum GLOBAL_GAME_STATE { MENU, GAME, LEVEL_EDITOR };
	static enum GLOBAL_GAME_STATE e_globalGame;

	//--------------------------------------------------------------------------------
	//CLOSED = NOT IN VIEW
	//OPEN = IN VIEW
	//--------------------------------------------------------------------------------
	static enum GLOBAL_START_MENU_STATE { CLOSED, OPEN };
	static GLOBAL_START_MENU_STATE e_globalMenu;

	static void ToggleMenu();
	static struct MenuFlags
	{
		bool exit;
		bool back;
	};

	static MenuFlags& GetFlags();
	static void ResetMenuFlags();

private:
	//Variables
	static Vec2 m_aspectRatio;
	static const int IDEAL_WIDTH = 1280;
	static const int IDEAL_HEIGHT = 720;
	static MenuFlags m_flags;

};

#endif//_BOMBERMAN_Util_H_