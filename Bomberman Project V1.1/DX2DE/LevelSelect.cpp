//------------------------------------------------------------------
//LevelSelect.cpp
//------------------------------------------------------------------
//

#include "LevelSelect.h"

LevelSelect::LevelSelect()
{
	m_level = "";
	m_confirm = false;
}

LevelSelect::~LevelSelect()
{

}

bool LevelSelect::Initialize(ID3D11Device* device, shared_ptr<Font> font, wstring dir, function<void(int)> levelCallback, int callbackValue, function<void(void)> backCallback)
{
	bool result;

	//Create and Initialize the AutoDropdown
	m_pDrop = shared_ptr<AutoDropdown>(new AutoDropdown);
	result = m_pDrop->Initialize(device, font, StringManager::GUIBlank(), dir, 200, 100, XMFLOAT2(-400 * ASPECT.x, 250 * ASPECT.y), 
		std::bind(&LevelSelect::GetCustomLevel, this, _1), 5, 1, 0.9f);
	if(!result)
		return false;

	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), 300, 100, XMFLOAT2(400 * ASPECT.x, 100 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Play", 5, Color::White(), 5.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), 200, 100, XMFLOAT2(400 * ASPECT.x, -200 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Back", 5, Color::White(), 2.0f);
	if(!result)
		return false;
	
	//Set the Buttons Callback Methods
	m_pButtons[0]->SetFunction(levelCallback, callbackValue);//This will set the level state in the Level Manager
	m_pButtons[1]->SetFunction(backCallback);

	//Create and initialize the Background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::Continue(), Global::GetScreenWidth(), Global::GetScreenHeight(), XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	return true;
}

void LevelSelect::Update(float gameTime)
{
	m_pDrop->Update(gameTime, Global::GetCursor()->GetBox());

	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->Update(gameTime, Global::GetCursor()->GetBox());

}

bool LevelSelect::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pBackground->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	result = m_pDrop->Render(context, shader, world ,view, ortho);
	if(!result)
		return false;

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void LevelSelect::Shutdown()
{
	//Release the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}

	//Release the Dropdown
	if(m_pDrop)
	{
		m_pDrop.reset();
		m_pDrop = 0;
	}

	//Release the Text
	if(m_pText)
	{
		m_pText.reset();
		m_pText = 0;
	}
	
	//Release the Background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}
}

void LevelSelect::Confirm()
{
	m_confirm = true;
}
bool LevelSelect::LevelStatus()
{
	return m_confirm;
}

string LevelSelect::GetLevel()
{
	return m_level;
}

void LevelSelect::GetCustomLevel(int num)
{
	m_pDrop->SetSelected(num, Color::LightGray());//Mark the selected level
	m_level = m_pDrop->GetString(num);//Get the Level name
}