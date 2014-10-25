//-------------------------------------------------------------
//Dropdown.cpp
//-------------------------------------------------------------
//

#include "Dropdown.h"

Dropdown::Dropdown()
{
	m_top = 0;
	m_display = 1;
	m_adjust = 1;
	m_short = false;
	m_lastSelected = 0;

	m_arrowWidth = 50;
	m_arrowHeight = 50;
}

Dropdown::~Dropdown()
{
	Shutdown();
}

bool Dropdown::Initialize(ID3D11Device* device, int imageWidth, int imageHeight, XMFLOAT2 pos, int display)
{
	bool result;

	//Set the base values
	m_position = pos;
	m_buttonWidth = imageWidth;
	m_buttonHeight = imageHeight;
	m_display = display;

	//Calculate the offset
	int off = imageHeight * ASPECT.x;
	m_offset = -off - (off * 0.1f);

	//Get the arrows X position
	int AW = pos.x + ((imageWidth * 0.70f) * ASPECT.x);
	
	//Create and initialize the Two navigation arrows
	m_pArrows.push_back(shared_ptr<Button>(new Button));
	result = m_pArrows[0]->Initialize(device, &StringManager::GUIUpArrow(), m_arrowWidth, m_arrowHeight, XMFLOAT2(AW, m_position.y), HOLD);
	if(!result)
		return false;
	
	m_pArrows.push_back(shared_ptr<Button>(new Button));
	result = m_pArrows[1]->Initialize(device, &StringManager::GUIDownArrow(), m_arrowWidth, m_arrowHeight, XMFLOAT2(AW, m_position.y + (-450 * ASPECT.y)), HOLD);
	if(!result)
		return false;

	//Set the color - used for debug then thought they looked cool
	m_pArrows[0]->SetColor(Color::Yellow());
	m_pArrows[1]->SetColor(Color::LightBlue());

	//Set the Arrow Functions for callback
	m_pArrows[0]->SetFunction(std::bind(&Dropdown::Arrow, this, _1), 1);
	m_pArrows[1]->SetFunction(std::bind(&Dropdown::Arrow, this, _1), 0);

	//Create and Initialize the Scroll icon - Mark vertical progress through the files
	m_pScrollIcon = shared_ptr<Static2D>(new Static2D);
	result = m_pScrollIcon->Initialize(device, StringManager::GUIBlank(), m_arrowWidth * ASPECT.x, (m_arrowHeight / 2) * ASPECT.y, 
		XMFLOAT2(m_position.x + ((imageWidth * 0.70f) * ASPECT.x), m_position.y - ((m_arrowHeight * 0.8f) * ASPECT.y)), 0);
	if(!result)
		return false;

	m_pScrollIcon->SetColor(Color::Green());
	
	//Calculate how long the scroll bar needs to be
	float distance = m_pArrows[0]->GetPosition().y - m_pArrows[1]->GetPosition().y;

	//Crease and position the base between the two arrow buttons
	m_pScrollBar = shared_ptr<Static2D>(new Static2D);
	result = m_pScrollBar->Initialize(device, StringManager::Connection(), (m_arrowWidth * 0.75f) * ASPECT.x, distance,
		XMFLOAT2(m_pArrows[0]->GetPosition().x, m_position.y - (((m_buttonHeight * m_display) * 0.45f) * ASPECT.y)), 0);
	if(!result)
		return false;

	m_pScrollBar->SetColor(Color::Gray());

	//Create the background
	InitializeBackground(device);

	return true;
}

bool Dropdown::InitializeBackground(ID3D11Device* device)
{
	bool result;
	//Get the base width of the background
	int W = (m_buttonWidth * 1.5f) * ASPECT.x;
	//Adjust the height based on the button height and display count
	int H = ((m_buttonHeight * m_display) * 1.2f) * ASPECT.y;
	
	int X;
	if(m_short) X = m_position.x;
	else X = m_position.x + ((m_arrowWidth * 0.5f) * ASPECT.x);	//Adjust the width of the background to fit the arrow buttons and scroll objects
	
	//Reposition the background to fit every object
	int Y = m_position.y - (H / 2) + ((m_buttonHeight * 0.75) * ASPECT.y);

	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::LevEdBackground(), W, H, XMFLOAT2(X, Y), 0);
	if(!result)
		return false;
	
	return true;
}

void Dropdown::Update(float gameTime, BoundingBox* box)
{
	//Button Null Check
	if(m_pButtons.size() > 0)
	{
		//Only update the buttons that are currently displayed 
		for(int c = m_top; c < (m_display + m_top); c++)
			m_pButtons[c]->Update(gameTime, box);
	}

	if(!m_short)
	{
		for(int c = 0; c < m_pArrows.size(); c++)
			m_pArrows[c]->Update(gameTime, box);
	}
}

bool Dropdown::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pBackground->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	if(m_pButtons.size() > 0)
	{
		//Only Render the buttons that are currently displayed
		for(int c = m_top; c < (m_display + m_top); c++) 
		{
			result = m_pButtons[c]->Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	if(!m_short)
	{
		result = m_pScrollBar->Render(context, shader, world, view, ortho);
		if(!result)
			return false;

		result = m_pScrollIcon->Render(context, shader, world, view, ortho);
		if(!result)
			return false;

		for(int c = 0; c < m_pArrows.size(); c++)
		{
			result = m_pArrows[c]->Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	return true;
}

void Dropdown::Shutdown()
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

	//Release the Scroll Icon
	if(m_pScrollIcon)
	{
		m_pScrollIcon.reset();
		m_pScrollIcon = 0;
	}

	//Release the Scroll Bar
	if(m_pScrollBar)
	{
		m_pScrollBar.reset();
		m_pScrollBar = 0;
	}

	//Release the Arrows
	for(int c = 0; c < m_pArrows.size(); c++)
	{
		m_pArrows[c].reset();
		m_pArrows[c] = 0;
	}
	m_pArrows.clear();

}

//Adjust the buttons based on which button was pressed
void Dropdown::Arrow(int value)
{
	switch(value)
	{
	case 0: 
		if(m_display + m_top < m_pButtons.size())
			Adjust(0); 
		break;
	case 1: 
		if(m_top > 0)
			Adjust(1); 
		break;
	}
}

void Dropdown::Move(float d, XMFLOAT2 dir)
{
	m_position.x += d * dir.x;
	m_position.y += d * dir.y;

	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->Move(d, dir);
}

//Adjust the position of the buttons using a direction + offset
void Dropdown::Adjust(int dir)
{
	int offset = m_offset * m_adjust;

	switch(dir)
	{
	case 0: 
		{
			int value = IncreaseTop();
			if(value != -1)
				offset = m_offset * (m_adjust - value);
			offset *= -1; 
		}
		break;
	case 1: 
		DecreaseTop();
		break;
	}

	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->AdjustPos(XMFLOAT2(0, offset));

	SetScrollIcon();
}

//Increase the Top value - Move the buttons downward
int Dropdown::IncreaseTop()
{
	int size = m_pButtons.size();

	//Increase the Top value
	m_top += m_adjust;
	//This will only become true if the adjust is not 1
	if(m_top + m_display > size)
	{
		m_top = size - m_display;
		return (size - m_adjust) - m_top;
	}

	return -1;
}
//Decrease the top value - Move the buttons upward
void Dropdown::DecreaseTop()
{
	m_top -= m_adjust;
	if(m_top < 0)
		m_top = 0;
}

//Move the Scroll icon up and down the scroll bar
void Dropdown::SetScrollIcon()
{
	float start = m_pArrows[0]->GetPosition().y - (m_pArrows[0]->GetDimensions().y * 0.8f);//Position A
	float end = m_pArrows[1]->GetPosition().y + (m_pArrows[1]->GetDimensions().y * 0.8f);//Position B
	float distance = start - end; //Distance between A and B

	//Get the number of buttons available after the display
	int available = m_pButtons.size() - m_display;
	
	//Get the Percentage between the top value and the number available
	float per = (float)m_top / (float)available;
	//Get the position using the Percentage of the distance
	float p = distance * per;
	p = start - p;

	//Set the position of the Scroll Icon
	float x = m_pScrollIcon->GetPosition().x;
	m_pScrollIcon->SetPosition(XMFLOAT2(x, p));
}

//Get the string of the button pressed
string Dropdown::GetString(int index)
{
	return m_pButtons[index]->GetText();
}

//Change the color of the selected button to visualize the players choice
void Dropdown::SetSelected(int index, XMFLOAT4 color)
{
	m_pButtons[m_lastSelected]->SetColor(Color::White());
	m_pButtons[index]->SetColor(color);
	m_lastSelected = index;
}