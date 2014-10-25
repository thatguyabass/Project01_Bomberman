//-----------------------------------------------------------------------------
//GameMenu.cpp
//-----------------------------------------------------------------------------
//

#include "GameMenu.h"

GameMenu::GameMenu()
{

}

GameMenu::~GameMenu()
{

}

bool GameMenu::Initialize(ID3D11Device* device, shared_ptr<Font> font)
{
	bool result;
	
	//Initialize and Create the Background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::GUIBlank(), 400 * ASPECT.x, 430 * ASPECT.y, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	m_pBackground->SetColor(Color::LightBlue());

	//Initialize and Create the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(0, 130 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Resume", 10, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(0, 0 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Return to Menu", 20, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(0, -130 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(device, font, "Quit", 20, Color::White(), 2.0f);
	if(!result)
		return false;

	//Set the Function callbacks
	m_pButtons[0]->SetFunction(std::bind(&StartMenu::ToggleMenu, this));
	m_pButtons[1]->SetFunction(std::bind(&StartMenu::Return, this));
	m_pButtons[2]->SetFunction(std::bind(&StartMenu::Quit, this));

	return true;
}