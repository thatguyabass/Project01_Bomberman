//--------------------------------------------------------------------
//StartMenu.cpp
//--------------------------------------------------------------------
//Define the Start Menu Class

#include "StartMenu.h"

StartMenu::StartMenu()
{
	m_pBackground = 0;
}

StartMenu::~StartMenu()
{

}

bool StartMenu::Initialize()
{
	bool result;

	return true;
}

void StartMenu::Update(float gameTime, BoundingBox* box, XMFLOAT2 offset)
{
	m_pBackground->SetOffset(offset);
	m_pBackground->Update(gameTime);

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c]->SetOffset(offset);
		m_pButtons[c]->Update(gameTime, box);
	}
}

bool StartMenu::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	if(m_pBackground)
	{
		result = m_pBackground->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void StartMenu::Shutdown()
{
	//Release Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release Background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}
}

void StartMenu::Quit()
{
	Util::GetFlags().exit = true;
}
void StartMenu::Return()
{
	Util::GetFlags().back = true;
}
void StartMenu::ToggleMenu()
{
	Util::ToggleMenu();
}
void FlagReset()
{
	Util::ResetMenuFlags();
}