//-------------------------------------------------------------
//levelManager.h
//-------------------------------------------------------------
//This class handles the Initalize, Update and Render for every level in the application
//It will also handle the Transition from one level to another

#ifndef _LEVEL_MANAGER_H_
#define _LEVEL_MANAGER_H_

#include "d3d.h"
#include "input.h"

#include "MenuManager.h"

#include "MainMenu.h"
#include "loading.h"
#include "Editor.h"
#include "Level.h"
#include "LevelBundle.h"

#include "Global.h"

#include<memory>

//-------------------------------------------------------------------------------------------

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	bool Initialize(shared_ptr<D3D>);
	void Update(float);
	bool Render();

	bool QuitApplication();

	//Level States
	enum LEVEL_TYPE { MENU, EDITOR, LEVEL, CUSTOM };

private:
	//Methods
	void Shutdown();

	void SetContinueType();//Process the Continue state and continue based on the users input
	void NextLevel();//Increment the Level Count in the Level Bundle
	void PlayAgain();//Play the current Selected Level again
	void SetLevel(int);//Set the Level State and initialize the selected level
	void MenuReturn();//Return to the Main Menu

	bool GetTypeInitialize();//Find the level index and initialize the level - this will destroy the current level
	void GetTypeUpdate(float);//Find the level index and update the level
	bool GetTypeRender();//Find the level index and Render the level

	void CustomLevelFlow();//User has selected a custom level - create the file path to load the selected level
	void CustomBundleFlow();//User has selected a custom level bundle - create the file path to load the correct level bundle
	void InitializeBundle(wstring);//Initialize a level bundle using the full file path
	void InitializeBundle(string);//Initialize a level bundle using just the bundle name - this will recreate the full file path

	template<typename T>
	bool InitializeLevel(wstring = L"", bool = false);//Initialize the Level
	bool InitializeMainMenu();//Initialize the Main Menu
	bool InitializeEditor();//Initialize the Editor

	template<typename T>
	void UpdateLevel(float);//Update the Current Level

	template<typename T>
	bool RenderLevel();//Render the Current Level

	//Variables
	wstring m_levelPath;

	shared_ptr<D3D> m_pD3D;
	shared_ptr<BaseLevel> m_pCurrent;
	shared_ptr<MenuManager> m_pMenu;
	shared_ptr<LevelBundle> m_pBundle;

	LEVEL_TYPE e_levelType;

};

template<typename T>
bool LevelManager::InitializeLevel(wstring level, bool cont)
{
	bool result;

	//Create Level
	m_pCurrent = shared_ptr<T>(new T);
	if(!m_pCurrent)
		return false;

	//Initialize Level
	result = dynamic_cast<T*>(m_pCurrent.get())->Initialize(m_pD3D, level, cont);
	if(!result)
		return false;

	return true;
}


template<typename T>
void LevelManager::UpdateLevel(float gameTime)
{
	dynamic_cast<T*>(m_pCurrent.get())->Update(gameTime);
}

template<typename T>
bool LevelManager::RenderLevel()
{
	bool result;
	result = dynamic_cast<T*>(m_pCurrent.get())->Render();
	if(!result)
		return false;

	return true;
}


#endif//_LEVEL_MANAGER_H_