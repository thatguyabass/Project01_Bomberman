//-----------------------------------------------------------------------------
//TextBox.cpp
//-----------------------------------------------------------------------------
//

#include "TextBox.h"

TextBox::TextBox() :
	BLINK_TIME(500)
{
	e_focus = OUT_OF_FOCUS;
	m_text = "";
	m_timer = 0;
	m_color = Color::White();
	m_blink = true;
}

TextBox::~TextBox()
{
	Shutdown();
}

bool TextBox::Initialize(ID3D11Device* device, shared_ptr<Font> font, int imageWidth, int imageHeight, XMFLOAT2 pos, bool num, string message, int length, float scale)
{
	bool result;

	//Create and initialize the background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::Connection(), imageWidth * ASPECT.x, imageHeight * ASPECT.y, pos, 0);
	if(!result)
		return false;

	//Create and initialize the text area - Where the text will appear
	m_pTextArea = shared_ptr<Static2D>(new Static2D);
	result = m_pTextArea->Initialize(device,StringManager::Connection(), (imageWidth * 0.9f) * ASPECT.x, (imageHeight * 0.9f) * ASPECT.y, pos, 0);
	if(!result)
		return false;

	//Set the backgrounds colors
	m_baseColor = Color::LightGray();
	m_pBackground->SetColor(Color::LightGray());
	m_pTextArea->SetColor(Color::Black());

	//Create a button to process when the user has clicked the text box
	m_pButton = shared_ptr<Button>(new Button);
	result = m_pButton->Initialize(device, 0, imageWidth * ASPECT.x, imageHeight * ASPECT.y, pos, ONCE);
	if(!result)
		return false;

	//Set the Focus to the Buttons callback
	m_pButton->SetFunction(std::bind(&TextBox::SetFocus, this, _1), 0);

	//Total number of characters allowed
	m_length = length;

	//Create and initialize the text
	m_pText = shared_ptr<Text>(new Text);
	result = m_pText->Initialize(device, font, message, length, XMFLOAT2(pos.x - (((imageWidth / 2) * 0.9f) * ASPECT.x), pos.y), Color::White(), scale);
	if(!result)
		return false;

	//Position the Text in the correct position
	XMFLOAT2 offset = m_pText->GetTextDimensions();
	XMFLOAT2 tPos = m_pText->GetPosition();
	m_pText->SetPosition(XMFLOAT2(tPos.x, tPos.y + ((offset.y / 2) * ASPECT.y))); 

	//Is this text box only registering number input
	m_num = num;

	return true;
}

void TextBox::Update(float gameTime, BoundingBox* box)
{
	//Update the button
	m_pButton->Update(gameTime, box); 
	
	//Update the text and background strob when in focus
	if(e_focus == IN_FOCUS)
	{
		UpdateSign(gameTime);

		if(m_num)
			HandleNumberInput();
		else
			HandleInput();
		m_pText->SetText(m_text);

		//Check if this text box is still in focus
		ReturnFocus(box);
	}
}

bool TextBox::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pBackground->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pTextArea->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pText->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void TextBox::Shutdown()
{
	//Release the background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}

	//Release the Text Area
	if(m_pTextArea)
	{
		m_pTextArea.reset();
		m_pTextArea = 0;
	}

	//Release the button
	if(m_pButton)
	{
		m_pButton.reset();
		m_pButton = 0;
	}

	//Release the Text
	if(m_pText)
	{
		m_pText.reset();
		m_pText = 0;
	}
}

//Focus Toggle
void TextBox::SetFocus(int value)
{
	switch(value)
	{
	case 0: e_focus = IN_FOCUS; break;
	case 1: 
		e_focus = OUT_OF_FOCUS; 
		m_pBackground->SetColor(m_baseColor);
		break;
	}
}
//Process number input
void TextBox::HandleNumberInput()
{
	char sent = Input::GetCurrentNumberInput();

	if(sent != '\0')
		AssignText(sent);
}
//Process letter input
void TextBox::HandleInput()
{
	char sent = Input::GetCurrentInput();

	if(sent != '\0')
		AssignText(sent);
}

//Assign the processed char into the string
void TextBox::AssignText(char c)
{
	if(c == '\b' && m_text.size() > 0)
	{
		m_text.erase(m_text.size() -1);
		return;
	}
	else if(c == '\b')
		return;

	if(m_text.size() + 1 < m_length)
		m_text += c;
}

//Remove focus from the text box
void TextBox::ReturnFocus(BoundingBox* box)
{
	if(Input::GetDownMouse() & MOUSE_LEFT)
		if(!m_pButton->GetBox()->BoundingBoxIntersect(box))
			SetFocus(1);

	if(Input::GetCurrentSpecial() & KEY_ENTER)
		SetFocus(1);
}

string TextBox::GetText()
{
	return m_text;
}

void TextBox::UpdateSign(float gameTime)
{
	//Strob between the two colors
	m_timer += gameTime;
	if(m_timer > BLINK_TIME)
	{
		m_timer = 0;
		m_color = Color::LightGray();
		if(m_blink)
		{
			m_color = Color::White();
			m_blink = false;
		}
		else
			m_blink = true;

		m_pBackground->SetColor(m_color);
	}
}