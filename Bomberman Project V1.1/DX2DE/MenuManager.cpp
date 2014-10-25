//-----------------------------------------------------
//MenuManager.cpp
//-----------------------------------------------------
//

#include "MenuManager.h"

MenuManager::MenuManager()
{

}

MenuManager::~MenuManager()
{
	Shutdown();
}

bool MenuManager::Initialize(shared_ptr<D3D> d3d)
{
	bool result;

	m_pD3D = d3d;

	//Create and initialize the Font Object
	m_pFont = shared_ptr<Font>(new Font);
	result = m_pFont->Initialize(m_pD3D->GetDevice(), StringManager::FontText(), StringManager::FontTexture());
	if(!result)
		return false;

	//Create and initialize a texture shader
	m_pTextureShader = shared_ptr<TextureShader>(new TextureShader);
	result = m_pTextureShader->Initialize(m_pD3D->GetDevice(), Global::GetHWND());
	if(!result)
		return false;

	//Create the Main Menu menu
	result = GetTypeToInitialize();
	if(!result)
		return false;

	return true;
}

void MenuManager::Update(float gameTime, XMFLOAT2 offset)
{
	//Update the global Cursor
	Global::GetCursor()->Update(gameTime);
	Global::GetCursor()->SetOffset(offset);

	//Toggle the menu when escape is pressed
	if(Input::GetDownSpecial() & KEY_ESCAPE)
		Util::ToggleMenu();

	//Update the menu when the menu is not in a closed state
	if(Util::e_globalMenu != Util::CLOSED)
	{
		//Check to see if the last game state matches the current game state
		if(e_last != Util::e_globalGame)
			GetTypeToInitialize();

		m_pStart->Update(gameTime, Global::GetCursor()->GetBox(), offset);
	}
}

bool MenuManager::Render(XMFLOAT4X4& view)
{
	bool result;
	XMFLOAT4X4 world, ortho;

	//Need to turn off the Z Buffer before gathering the render data
	m_pD3D->TurnZBufferOff();
	//Turn on alpha blending to remove any alpha from the texture
	m_pD3D->TurnOnAlphaBlending();

	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	if(Util::e_globalMenu != Util::CLOSED)
	{
		result = m_pStart->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = Global::GetCursor()->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	//Reenable alpha blending the the Z Buffer
	m_pD3D->TurnOffAlphaBlending();
	m_pD3D->TurnZBufferOff();

	return true;
}

bool MenuManager::GetTypeToInitialize()
{
	bool result;

	//Release the current menu
	if(m_pStart)
	{
		m_pStart.reset();
		m_pStart = 0;
	}

	//Create and initialize a new menu based on the current game state
	switch(Util::e_globalGame)
	{
	case Util::MENU: result = InitializeMenu<MenuMenu>(); break;
	case Util::GAME: result = InitializeMenu<GameMenu>(); break;
	case Util::LEVEL_EDITOR: result = InitializeMenu<GameMenu>(); break;
	}

	//Set the last game state
	e_last = Util::e_globalGame;

	return true;
}

void MenuManager::Shutdown()
{
	//Release the D3D
	if(m_pD3D)
	{
		m_pD3D.reset();
		m_pD3D = 0;
	}

	//Release the Font
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}

	//Release the TextureShader
	if(m_pTextureShader)
	{
		m_pTextureShader.reset();
		m_pTextureShader = 0;
	}

	//Release the current menu
	if(m_pStart)
	{
		m_pStart.reset();
		m_pStart = 0;
	}
}