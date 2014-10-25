//------------------------------------------------------------------
//MessagePrompt.cpp
//------------------------------------------------------------------
//

#include "MessagePrompt.h"

MessagePrompt::MessagePrompt()
{
	m_timer = 0;
	m_displayTime = 0;
	m_display = 0;
}

MessagePrompt::~MessagePrompt()
{
	Shutdown();
}

bool MessagePrompt::Initialize(ID3D11Device* device, shared_ptr<Font> font)
{
	bool result;

	m_pText = shared_ptr<Text>(new Text);
	result = m_pText->Initialize(device, font, "Temp", 50, XMFLOAT2(0,0), Color::White(), 1.0f);
	if(!result)
		return false;

	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::GUIBlank(), 100, 100, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	return true;
}

void MessagePrompt::Update(float gameTime, XMFLOAT2 offset)
{
	//Only update if the message prompt is being displayed
	if(m_display)
	{
		m_pText->SetOffset(offset);
		m_pText->Update();

		m_pBackground->SetOffset(offset);
		m_pBackground->Update(gameTime);

		m_timer += gameTime;
		if(m_timer > m_displayTime)
			m_display = false;
	}
}

bool MessagePrompt::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	//Only render if the message prompt is being displayed
	if(m_display)
	{
		result = m_pBackground->Render(context, shader, world, view, ortho);
		if(!result)
			return false;

		result = m_pText->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void MessagePrompt::Shutdown()
{
	if(m_pText)
	{
		m_pText.reset();
		m_pText = 0;
	}

	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}
}

//Set the values to be displayed 
void MessagePrompt::Display(float delay, string message, XMFLOAT4 color, XMFLOAT2 boxPos, XMFLOAT2 boxSize, float scale)
{
	m_display = true;
	m_displayTime = (delay * 1000);
	m_timer = 0;

	m_pText->SetText(message);
	m_pText->SetScale(scale);

	XMFLOAT2 pos = boxPos;
	XMFLOAT2 dime = m_pText->GetFont()->GetTextDimension(message, scale);

	pos.x -= (dime.x / 2);
	pos.y += (dime.y /2);

	m_pText->SetLocal(pos);
	m_pText->SetColor(color);

	m_pBackground->SetLocal(boxPos);
	m_pBackground->SetDimensions(boxSize);
}