//------------------------------------------------------------------------
//MenuMenu.cpp
//------------------------------------------------------------------------
//

#include "MenuMenu.h"

MenuMenu::MenuMenu()
{

}

MenuMenu::~MenuMenu()
{

}

bool MenuMenu::Initialize(ID3D11Device* device, shared_ptr<Font> font)
{
	bool result;

	//Create and Initilaise the background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::GUIBlank(), 400 * ASPECT.x, 300 * ASPECT.y, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	m_pBackground->SetColor(Color::LightGray());

	//Initialize and Create the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(0, -65 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Quit", 5, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(0, 65 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Resume", 7, Color::White(), 2.0f);
	if(!result)
		return false;

	//Set the Function callbacks
	m_pButtons[0]->SetFunction(std::bind(&StartMenu::Quit, this));
	m_pButtons[1]->SetFunction(std::bind(&StartMenu::ToggleMenu, this));

	return true;
}
