//--------------------------------------------------------------
//Continue.cpp
//--------------------------------------------------------------
//Define the Continue Class

#include "Continue.h"

Continue::Continue()
{
	e_continue = (CONTINUE_STATE)NONE_INDEX;
	m_min = 0;
}

Continue::~Continue()
{
	Shutdown();
}

bool Continue::Initialize(ID3D11Device* device, bool canContinue)
{
	bool result;

	//Create and Initialize font
	shared_ptr<Font> font = shared_ptr<Font>(new Font);
	result = font->Initialize(device, StringManager::FontText(), StringManager::FontTexture());
	if(!result)
		return false;

	//Button Dimensions
	int buttonWidth = 200;
	int buttonHeight = 100;

	//Create and Initialize the Background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::Continue(), Global::GetScreenWidth(), Global::GetScreenHeight(), XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(-300 * ASPECT.x, -200 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Next Level", 15, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), buttonWidth + 50, buttonHeight, XMFLOAT2(0, -200 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Return to Menu", 25, Color::White(), 2.0f);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(300 * ASPECT.x, -200 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(device, font, "Play Again", 15, Color::White(), 2.0f);
	if(!result)
		return false;

	//Set the Buttons Callback methods
	m_pButtons[0]->SetFunction(std::bind(&Continue::SetState, this, _1), NEXT);
	m_pButtons[1]->SetFunction(std::bind(&Continue::SetState, this, _1), QUIT);
	m_pButtons[2]->SetFunction(std::bind(&Continue::SetState, this, _1), AGAIN);

	if(!canContinue)
		m_min = 1;//Remove the Next Button from update/render

	return true;
}

void Continue::Update(float gameTime, XMFLOAT2 offset)
{
	if(Stats::BomberDead())
		m_min = 1;//Remove the Next Button from update/render

	//Update the Buttons - If m_min = 1, do not update the next button
	for(int c = m_min; c < m_pButtons.size(); c++)
	{
		m_pButtons[c]->SetOffset(offset);
		m_pButtons[c]->Update(gameTime, Global::GetCursor()->GetBox());
	}

	m_pBackground->SetOffset(offset);
	m_pBackground->Update(gameTime);
}

bool Continue::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pBackground->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	//Update the Buttons - If m_min = 1, do not Render the next button
	for(int c = m_min; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Set the Continue State
void Continue::SetState(int index)
{
	switch(index)
	{
	case 0: e_continue = NEXT; break;
	case 1: e_continue = QUIT; break;
	case 2: e_continue = AGAIN; break;
	}
}

int Continue::GetState()
{
	return e_continue;
}

void Continue::Shutdown()
{
	//Release the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release the Background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}
}