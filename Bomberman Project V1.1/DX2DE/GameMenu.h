//---------------------------------------------------------------------------------
//GameMenu.h
//---------------------------------------------------------------------------------
//Define the Game menu. Child class to StartMenu

#ifndef _GAME_MENU_H_
#define _GAME_MENU_H_

#include "StartMenu.h"

class GameMenu : public StartMenu
{
public:
	GameMenu();
	~GameMenu();

	bool Initialize(ID3D11Device*, shared_ptr<Font>);

};

#endif//_GAME_MENU_H_