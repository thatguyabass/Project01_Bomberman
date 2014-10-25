//---------------------------------------------------------------------
//HUD.cpp
//---------------------------------------------------------------------
//define the HUD class

#include "HUD.h"

HUD::HUD()
{
	m_pFont = 0;

	m_lives = -1;
	m_score = -1;
}

HUD::HUD(const HUD& src)
{

}

HUD::~HUD()
{
	Shutdown();
}

bool HUD::Initialize(ID3D11Device* device)
{
	bool result;

	//Initialize Font
	result = InitializeFont(device);
	if(!result)
		return false;

	//Create and Initialize the Text Objects
	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[FPS_INDEX]->Initialize(device, m_pFont, "Temp" , 15, XMFLOAT2(-300 * ASPECT.x, 355 * ASPECT.y), Color::White());
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[LIVES_INDEX]->Initialize(device, m_pFont, "Temp2", 15, XMFLOAT2(0 * ASPECT.x, 355 * ASPECT.y), Color::White());
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[SCORE_INDEX]->Initialize(device, m_pFont, "Temp3", 15, XMFLOAT2(300 * ASPECT.x, 355 * ASPECT.y), Color::White());
	if(!result)
		return false;

	m_background = Static2D();
	m_background.Initialize(device, StringManager::Connection(), Global::GetScreenWidth() * 1.1f, 50 * ASPECT.y, XMFLOAT2(0, (Global::GetScreenHeight() / 2) - (15 * ASPECT.y)), 0);
	if(!result)
		return false;
	m_background.SetColor(Color::Gray());

	return true;
}

void HUD::Update(XMFLOAT2 offset)
{
	m_background.SetOffset(offset);
	m_background.Update(0.0f);

	//Update FPS Text
	UpdateFPS();

	//Update Score Text
	UpdateScore();

	//Update Lives Text
	UpdateLives();

	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c]->SetOffset(offset);
		m_pText[c]->Update();
	}
}

bool HUD::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_background.Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	for(int c = 0; c< m_pText.size(); c++)
	{
		result = m_pText[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void HUD::Shutdown()
{
	//Release the Font
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}

	//Release the Text Objects
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
}

bool HUD::InitializeFont(ID3D11Device* device)
{
	bool result;

	if(!m_pFont)
	{
		//Create Font Object
		m_pFont = shared_ptr<Font>(new Font);
		if(!m_pFont)
			return false;

		//Initialzie Font
		result = m_pFont->Initialize(device, StringManager::FontText(), StringManager::FontTexture());
		if(!result)
			return false;
	}

	return true;
}

void HUD::UpdateFPS()
{
	int fps = FPS::GetFPS();
	m_pText[FPS_INDEX]->SetText("FPS: " + std::to_string(fps));
}

void HUD::UpdateLives()
{
	//Update if a change was detected
	if(m_lives != Stats::GetLives())
	{
		m_lives = Stats::GetLives();
		m_pText[LIVES_INDEX]->SetText("Lives: " + std::to_string(m_lives));
	}
}

void HUD::UpdateScore()
{
	//Update if a change was detected
	if(m_score != Stats::GetScore())
	{
		m_score = Stats::GetScore();
		m_pText[SCORE_INDEX]->SetText("Score: " + std::to_string(m_score));
	}
}

Font* HUD::GetFont()
{
	return m_pFont.get();
}