//------------------------------------------------------------------------
//MainMenu.h
//------------------------------------------------------------------------
//this class defines the main menu class
//the main menu class will be used as a starting point for the users

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#include "baselevel.h"

#include "text.h"
#include "font.h"

#include "BundleSelect.h"
#include "LevelSelect.h"
#include "Button.h"
#include "Global.h"

//----------------------------------------------------------------------------------------------------------

class MainMenu : public BaseLevel
{
public:
	MainMenu();
	~MainMenu();

	bool Initialize(shared_ptr<D3D>, wstring = L"");
	void Update(float);
	bool Render();

	bool LevelSet();//Is a Level Set
	int GetLevel();//Get the index of the next level
	string GetSelectedLevel();//Get the Selected Level Path
	string GetSelectedBundle();//Get the Selected Bundle Path

	bool GetMenuState();//Check if the menu state is MAIN
	//Menu State
	enum MAIN_MENU { MAIN, CUSTOM, BUNDLE}; 

private:
	//Methods
	void Shutdown();
	bool Render(XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	void SelectLevel(int);//Change the Current Menu State
	void Back();//Return the menu state to MAIN

	//Change the current Level
	void SetLevel(int);
	
	//States
	MAIN_MENU e_mainMenu;

	//Variables
	bool m_levelSet;//Has a level been selected
	int m_levelIndex;//Set Level index

	shared_ptr<Font> m_pFont;

	shared_ptr<LevelSelect> m_pLevel;
	shared_ptr<BundleSelect> m_pBundle;
	shared_ptr<Static2D> m_pMainMenu;
	vector<shared_ptr<Button>> m_pButtons;
	vector<shared_ptr<Text>> m_pText;

};

#endif//_MAIN_MENU_H_