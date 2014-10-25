//-------------------------------------------------------------------------
//Button.cpp
//-------------------------------------------------------------------------
//Define the button class

#include "Button.h"

Button::Button()
{
	//base values
	m_timer = 0;
	m_delay = false;
	m_inputDelay = INPUT_DELAY;
}

Button::~Button()
{
	Shutdown();
}

bool Button::Initialize(ID3D11Device* device, wstring* texture, int imageWidth, int imageHeight, XMFLOAT2 pos, PRESS_TYPE press, float boxMod)
{
	bool result;

	e_press = press;

	result = Base2D::Initialize(device, imageWidth * ASPECT.x, imageHeight * ASPECT.y, pos, 0);
	if(!result)
		return false;

	//If no texture is provided, dont load a texture
	if(texture)
	{
		result = Base2D::LoadTexture(device, *texture);
		if(!result)
			return false;
	}
	
	//Set up the bounding box
	m_boundsMod = boxMod;
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(m_imageWidth * m_boundsMod, m_imageHeight * m_boundsMod, POSITION);

	return true;
}

void Button::Update(float gameTime, BoundingBox* box)
{
	//Update the position
	Base2D::Update(gameTime);

	//if the button has been pressed, do not process the next press until after the delay is complete
	if(m_delay)
	{
		m_timer += gameTime;
		if(m_timer >= m_inputDelay)
		{
			m_timer = 0;
			m_delay = false;
		}
	}
	else
	{
		if(box)
			Pressed(box);
		else
			Pressed();
	}
}

bool Button::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	//If no texture is found do not render
	if(m_pTexture)
	{
		//Render the Button
		result = Base2D::Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}
	
	//If no text is found do not render
	if(m_pText)
	{
		result = m_pText->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void Button::Shutdown()
{

}

//Update pressed using the Raw Mouse Position
void Button::Pressed()
{
	XMFLOAT2 mouse = Input::GetMousePosition();

	XMFLOAT2 D = this->GetDimensions();
	XMFLOAT2 O = this->GetLocal();

	XMFLOAT2 X;
	XMFLOAT2 Y;
	X.x	= O.x - (D.x / 2);
	X.y = O.x + (D.x / 2);
		
	Y.x = O.y - (D.y / 2);
	Y.y = O.y + (D.y / 2);

	if(mouse.x > X.x && mouse.x < X.y)
	{
		if(mouse.y > Y.x && mouse.y < Y.y)
		{
			//Check to see if the input matches the PRESS TYPE
			if(GetPressType(MOUSE_LEFT))
			{
				m_delay = true;
				ExecuteCallback();
			}
		}
	}
}

//Update the pressed method using bounding box collision
void Button::Pressed(BoundingBox* box)
{
	if(box->BoundingBoxIntersect(m_pBox.get()))
	{
		//Check to see if the input matches the PRESS TYPE
		if(GetPressType(MOUSE_LEFT))
		{
			m_delay = true;
			ExecuteCallback();
		}
	}
}

//Set the void() Callback method
void Button::SetFunction(std::function<void(void)> callback)
{
	m_funcVoid = callback;
}
//Set the void(int) Callback method - Value will be passed into this callback
void Button::SetFunction(std::function<void(int)> callback, int value)
{
	m_funcInt = callback;
	m_funcIntValue = value;
}
void Button::ResetFunctionValue(int value)
{
	m_funcIntValue = value;
}

//Set up text for this button
bool Button::InitializeText(ID3D11Device* device, shared_ptr<Font> font, string text, int length, XMFLOAT4 color, float fontScale)
{
	bool result;

	m_pText = shared_ptr<Text>(new Text);
	result = m_pText->Initialize(device, font, text, length, XMFLOAT2(0,0), color, fontScale);
	if(!result)
		return false;

	RepositionText();

	return true;
}

//Ensure the text is in the center of the button
void Button::RepositionText()
{
	XMFLOAT2 pos = this->GetPosition();
	XMFLOAT2 textDimensions = m_pText->GetFont()->GetTextDimension(m_pText->GetText(), m_pText->GetScale());

	if(textDimensions.x > this->GetDimensions().x)
		pos.x -= this->GetDimensions().x / 2;
	else
		pos.x -= textDimensions.x / 2;

	pos.y += this->GetDimensions().y * 0.2f;

	m_pText->SetPosition(pos);
	m_pText->SetLocal(pos);
}

//Change the text displayed on the button
void Button::SetText(string text)
{
	if(m_pText)
	{
		m_pText->SetText(text);
		RepositionText();
	}
}
string Button::GetText()
{
	if(m_pText)
		return m_pText->GetText();
	else
		return '\0';
}

void Button::Move(float d, XMFLOAT2 dir)
{
	LOCAL.x += d * dir.x;
	LOCAL.y += d * dir.y;
	if(m_pText)
		m_pText->Move(d, dir);
}

//move the button a much larger distance - Used in the AutoDropdown and DynamicDropdown classes
void Button::AdjustPos(XMFLOAT2 offset)
{
	LOCAL.x += offset.x;
	LOCAL.y += offset.y;

	if(m_pText)
		m_pText->AdjustPos(offset);
}

void Button::SetOffset(XMFLOAT2 offset)
{
	if(m_pText)
	{
		m_pText->SetOffset(offset);
		m_pText->Update();
	}

	Base2D::SetOffset(offset);
}

void Button::SetInputDelay(int delay)
{
	m_inputDelay = delay;
}

//Determine if the input given meets the PRESS_TYPE for this object
bool Button::GetPressType(int key)
{
	int current = Input::GetCurrentMouse();
	int down = Input::GetDownMouse();
	int release = Input::GetUpMouse();
	bool result = false;

	switch(e_press)
	{
	case HOLD: result = (current & key); break;
	case ONCE: result = (down & key); break;
	case RELEASE: result = (release & key); break;
	}

	return result;
}

//Call the callback method
void Button::ExecuteCallback()
{
	if(!m_funcInt._Empty())
		m_funcInt(m_funcIntValue);
	else if(!m_funcVoid._Empty())
		m_funcVoid();
}