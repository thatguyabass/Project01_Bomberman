//----------------------------------------------------------------------------
//MenuManager.h
//----------------------------------------------------------------------------
//This class will always be alive for the duration of the application. Because of this, the Cursor Global object is updated and rendered here
//the Class handles the management between all the Different Start menus

#ifndef _MENU_MANAGER_H_
#define _MENU_MANAGER_H_

#include "d3d.h"
#include "Global.h"

#include "GameMenu.h"
#include "MenuMenu.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------

class MenuManager
{
public:
	MenuManager();
	~MenuManager();

	bool Initialize(shared_ptr<D3D>);
	void Update(float, XMFLOAT2);
	bool Render(XMFLOAT4X4&);

private:
	//Methods
	void Shutdown();
	bool GetTypeToInitialize();

	template<typename T>
	bool InitializeMenu();

	//Variables
	shared_ptr<D3D> m_pD3D;
	shared_ptr<Font> m_pFont;
	shared_ptr<TextureShader> m_pTextureShader;

	//Start menu type
	shared_ptr<StartMenu> m_pStart;

	//Change which menu based on the Global Game State
	Util::GLOBAL_GAME_STATE e_last;
	
};

template<typename T>
bool MenuManager::InitializeMenu()
{
	bool result;

	//Release the current menu 
	if(m_pStart)
	{
		m_pStart.reset();
		m_pStart = 0;
	}

	//Create and initialize the new menu
	m_pStart = shared_ptr<T>(new T);
	result = dynamic_cast<T*>(m_pStart.get())->Initialize(m_pD3D->GetDevice(), m_pFont);
	if(!result)
		return false;

	return true;
}

#endif//_MENU_MANAGER_H_