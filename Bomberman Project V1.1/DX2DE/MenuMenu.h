//---------------------------------------------------------------------------
//MenuMenu.h
//---------------------------------------------------------------------------
//Define the Menu menu. Child of the StartMenu class

#ifndef _MENU_MENU_H_
#define _MENU_MENU_H_

#include "StartMenu.h"

class MenuMenu : public StartMenu
{
public:
	MenuMenu();
	~MenuMenu();

	bool Initialize(ID3D11Device*, shared_ptr<Font>);

};

#endif//_MENU_MENU_H_