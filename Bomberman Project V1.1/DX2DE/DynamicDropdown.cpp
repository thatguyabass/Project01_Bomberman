//------------------------------------------------------
//DynamicDropdown.cpp
//------------------------------------------------------
//

#include "DynamicDropdown.h"

DynamicDropdown::DynamicDropdown()
{
	m_buttonCount = 0;
	m_selected = false;
	e_dynamic = DYNAMIC;

	m_modWidth = 75;
	m_modHeight = 75;
}

DynamicDropdown::~DynamicDropdown()
{
	Shutdown();
}

bool DynamicDropdown::Initialize(ID3D11Device* device, shared_ptr<Font> font, wstring texture, int imageWidth, int imageHeight, XMFLOAT2 pos, int display, float boxMod, float scale)
{
	bool result;

	//Values stored for the creation of future buttons
	m_pDevice = device;
	m_pFont = font;
	m_scale = scale;
	m_baseDisplay = display;

	result = Dropdown::Initialize(device, imageWidth, imageHeight, pos, display);
	if(!result)
		return false;

	//Create and initialize the Add Auto Dropdown
	m_pDrop = shared_ptr<AutoDropdown>(new AutoDropdown);
	result = m_pDrop->Initialize(device, font, texture, StringManager::CustomLevelDirectory(), imageWidth, imageHeight, XMFLOAT2(0, pos.y), std::bind(&DynamicDropdown::StartAdd, this, _1), display, boxMod, scale);
	if(!result)
		return false;

	//Create and initialize the Add Dropdown Buttons
	m_pDropBut.push_back(shared_ptr<Button>(new Button));
	result = m_pDropBut[0]->Initialize(device, &StringManager::GUICancel(), 75, 75, XMFLOAT2(225 * ASPECT.x, pos.y), ONCE);
	if(!result)
		return false;

	m_pDropBut.push_back(shared_ptr<Button>(new Button));
	result = m_pDropBut[1]->Initialize(device, &StringManager::GUIAdd(), 75, 75, XMFLOAT2(225 * ASPECT.x, pos.y + (-450 * ASPECT.y)), ONCE);
	if(!result)
		return false;
	
	//Set the Add Callback functions to the Add Dropdown Buttons
	m_pDropBut[0]->SetFunction(std::bind(&DynamicDropdown::AddState, this));
	m_pDropBut[1]->SetFunction(std::bind(&DynamicDropdown::Add, this));

	//Create a screensize texture to fade the background when in the Add State
	m_pDropBack = shared_ptr<Static2D>(new Static2D);
	result = m_pDropBack->Initialize(device, StringManager::Connection(), Global::GetScreenWidth(), Global::GetScreenHeight(), XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	XMFLOAT4 color = Color::Gray();
	m_pDropBack->SetColor(XMFLOAT4(color.x, color.y, color.z, 0.5f));

	//Calculate the position of the Modification buttons
	int modPosX = pos.x + ((imageWidth * 0.70f) * ASPECT.x);

	//Create and initialize the Mod Buttons
	m_pModButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pModButtons[0]->Initialize(device, &StringManager::GUIUpArrow(), m_modWidth, m_modHeight, XMFLOAT2(modPosX + (m_modWidth * ASPECT.x), pos.y + (-100 * ASPECT.y)), ONCE);
	if(!result)
		return false;

	m_pModButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pModButtons[1]->Initialize(device, &StringManager::GUIAdd(), m_modWidth, m_modHeight, XMFLOAT2(modPosX + (m_modWidth * ASPECT.x), pos.y + (-200 * ASPECT.y)), ONCE);
	if(!result)
		return false;

	m_pModButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pModButtons[2]->Initialize(device, &StringManager::GUIRemove(), m_modWidth, m_modHeight, XMFLOAT2(modPosX + (m_modWidth * ASPECT.x), pos.y + (-300 * ASPECT.y)), ONCE);
	if(!result)
		return false;

	m_pModButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pModButtons[3]->Initialize(device, &StringManager::GUIDownArrow(), m_modWidth, m_modHeight, XMFLOAT2(modPosX + (m_modWidth * ASPECT.x), pos.y + (-400 * ASPECT.y)), ONCE);
	if(!result)
		return false;

	//Set the Mod buttons Callback functions
	m_pModButtons[0]->SetFunction(std::bind(&DynamicDropdown::MoveUp, this));
	m_pModButtons[1]->SetFunction(std::bind(&DynamicDropdown::AddState, this));
	m_pModButtons[2]->SetFunction(std::bind(&DynamicDropdown::Remove, this));
	m_pModButtons[3]->SetFunction(std::bind(&DynamicDropdown::MoveDown, this));

	return true;
}

//create the background to accommodate the added buttons
bool DynamicDropdown::InitializeBackground(ID3D11Device* device)
{
	bool result;
	int W = (m_buttonWidth * 2.0f) * ASPECT.x;								//Background Width
	int H = ((m_buttonHeight * m_display) * 1.2f) * ASPECT.y;				//Background Height
	int X = m_position.x + (((m_arrowWidth + m_modWidth) * 0.5f) * ASPECT.x);//Background X position
	int Y = m_position.y - (H / 2) + ((m_buttonHeight * 0.75) * ASPECT.y);	//Background Y Position

	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(device, StringManager::LevEdBackground(), W, H, XMFLOAT2(X, Y), 0);
	if(!result)
		return false;
	
	return true;
}

void DynamicDropdown::Update(float gameTime, BoundingBox* box)
{
	//Update based on the current State
	switch(e_dynamic)
	{
	case DYNAMIC:
		Dropdown::Update(gameTime, box);

		for(int c = 0; c < m_pModButtons.size(); c++)
			m_pModButtons[c]->Update(gameTime, box);
	break;
	case ADD:
		m_pDrop->Update(gameTime, box);

		for(int c = 0; c < m_pDropBut.size(); c++)
			m_pDropBut[c]->Update(gameTime, box);
	break;
	}
}

bool DynamicDropdown::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Dropdown::Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	for(int c = 0; c < m_pModButtons.size(); c++)
	{
		result = m_pModButtons[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	if(e_dynamic == ADD)
	{		
		result = m_pDropBack->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;

		result = m_pDrop->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;

		for(int c = 0; c < m_pDropBut.size(); c++)
		{
			result = m_pDropBut[c]->Render(context, textureShader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	return true;
}

void DynamicDropdown::Shutdown()
{
	//Release the Device Reference
	if(m_pDevice)
		m_pDevice = 0;

	//Release the Add Dropdown
	if(m_pDrop)
	{
		m_pDrop.reset();
		m_pDrop = 0;
	}

	//Release teh Add background
	if(m_pDropBack)
	{
		m_pDropBack.reset();
		m_pDropBack = 0;
	}

	//Release the Add buttons
	for(int c = 0; c < m_pDropBut.size(); c++)
	{
		m_pDropBut[c].reset();
		m_pDropBut[c] = 0;
	}
	m_pDropBut.clear();

	//Release the Mod Buttons
	for(int c = 0; c < m_pModButtons.size(); c++)
	{
		m_pModButtons[c].reset();
		m_pModButtons[c] = 0;
	}
	m_pModButtons.clear();

	//Release the Font
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}
}

//Toggle the Add State
void DynamicDropdown::AddState()
{
	if(e_dynamic == ADD)
		e_dynamic = DYNAMIC;
	else if(e_dynamic == DYNAMIC)
		e_dynamic = ADD;
}
//Set the current Selected and set the visual element for the user
void DynamicDropdown::StartAdd(int value)
{
	m_pDrop->SetSelected(value, Color::LightGray());
	m_currentSelected = value;
}

//Add the new button
void DynamicDropdown::Add()
{
	//Move the list back to the top - needed for proper placement
	while(m_top != 0)
		Arrow(1);

	//Create and initialize the Button
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	m_pButtons[m_buttonCount]->Initialize(m_pDevice, &StringManager::GUIBlank(), m_buttonWidth, m_buttonHeight, XMFLOAT2(m_position.x, m_position.y + (m_offset * m_buttonCount)), ONCE);

	//Get the text from teh selected button from teh Add Dropdown
	string text = m_pDrop->GetString(m_currentSelected);

	//Set the Buttons Text
	m_pButtons[m_buttonCount]->InitializeText(m_pDevice, m_pFont, text, 50, Color::White(), m_scale);
	m_pButtons[m_buttonCount]->SetFunction(std::bind(&DynamicDropdown::GetButtonIndex, this, _1), m_buttonCount);

	//Increase the button count
	m_buttonCount++;

	//Modify the Display value if it is still under the base display value
	m_display = m_pButtons.size();
	if(m_display > m_baseDisplay)
		m_display = m_baseDisplay;

	//readjust the buttons to display the new button at the bottom
	int top = m_pButtons.size() - m_display;
	if(top > 0)
	{
		while(m_top != top)
			Arrow(0);
	}

	//Update the buttons
	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->Update(0.1f);
}

void DynamicDropdown::Remove()
{
	//Make sure a button is selected
	if(m_selected)
	{
		//Delete the Button
		m_pButtons[m_lastSelected].reset();
		m_pButtons[m_lastSelected] = 0;

		//Remove the button from the vector
		m_pButtons.erase(m_pButtons.begin() + m_lastSelected);

		//Readjust the display value
		m_display = m_pButtons.size();
		if(m_display > m_baseDisplay)
			m_display = m_baseDisplay;

		//Initialize the values to adjust the buttons up one to fill the empty spot
		int offset = -m_offset;
		int start = m_lastSelected;
		int mod = 0;

		//if the top index and Display are greater then the button size, the user is at the bottom of the dropdown
		if(m_top + m_display > m_pButtons.size())
		{
			//Reverse the values to move the buttons down one position
			m_top -= 1;
			offset *= -1;
			start = 0;
			mod = m_pButtons.size() - m_lastSelected;
		}

		//Reposition the buttons
		for(int c = start; c < m_pButtons.size() - mod; c++)
		{
			m_pButtons[c]->AdjustPos(XMFLOAT2(0, offset));
			m_pButtons[c]->ResetFunctionValue(c);
		}

		//Reset and decrease count
		m_lastSelected = 0;
		m_buttonCount--;
		m_selected = false;
	}
}

//Internal call back to get the current selected button
void DynamicDropdown::GetButtonIndex(int value)
{
	m_selected = true;
	this->SetSelected(value, Color::LightGray());
}
 
//Swap the text between two values
void DynamicDropdown::Swap(shared_ptr<Button>& A, shared_ptr<Button>& B)
{
	string text = A->GetText();
	A->SetText(B->GetText());
	B->SetText(text);
}

//Move the selected button up one position
void DynamicDropdown::MoveUp()
{
	if(m_selected)
	{
		//Valid movement check
		if(m_lastSelected - 1 >= 0)
		{
			Swap(m_pButtons[m_lastSelected], m_pButtons[m_lastSelected - 1]);
			int value = m_lastSelected - 1;
			if(value < 0)
				value = 0;

			this->SetSelected(value, Color::LightGray());
		}
	}
}
//Move the Selected button down one position
void DynamicDropdown::MoveDown()
{
	if(m_selected)
	{
		//Valid movement check
		if(m_lastSelected + 1 < m_pButtons.size())
		{
			Swap(m_pButtons[m_lastSelected], m_pButtons[m_lastSelected + 1]);
			int value = m_lastSelected + 1;
			if(value > m_pButtons.size())
				value = m_pButtons.size() - 1;

			this->SetSelected(value, Color::LightGray());
		}
	}
}

//Populate this dropdown with buttons
void DynamicDropdown::Populate(vector<wstring>& src)
{
	//Release and clear any buttons that already exist
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();
	//Set the button count for future buttons
	m_buttonCount = src.size();

	vector<string> text;
	//Get the size of the Level Dir string to remove the file path from the string
	int size = StringManager::CustomLevelDir().size();
	for(int c = 0; c < src.size(); c++)
		text.push_back(string(src[c].begin() + size, src[c].end()));

	//Create and Initialize the buttons
	for(int c = 0; c < src.size(); c++)
	{
		m_pButtons.push_back(shared_ptr<Button>(new Button));
		m_pButtons[c]->Initialize(m_pDevice, &StringManager::GUIBlank(), m_buttonWidth, m_buttonHeight, XMFLOAT2(m_position.x, m_position.y + (m_offset * c)), ONCE);

		m_pButtons[c]->InitializeText(m_pDevice, m_pFont, text[c], 50, Color::White(), m_scale);
		m_pButtons[c]->SetFunction(std::bind(&DynamicDropdown::GetButtonIndex, this, _1), c);
	}

	//Reset Top
	m_top = 0;

	//Readjust the display
	m_display = m_pButtons.size();
	if(m_display > m_baseDisplay)
		m_display = m_baseDisplay;
}

//Return the strings of each buttons
void DynamicDropdown::GetStrings(vector<string>& src)
{
	for(int c = 0; c < m_pButtons.size(); c++)
		src.push_back(m_pButtons[c]->GetText());
}

bool DynamicDropdown::GetState()
{
	return (e_dynamic == DYNAMIC);
}