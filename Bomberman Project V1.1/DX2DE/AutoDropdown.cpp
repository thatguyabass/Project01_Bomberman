//------------------------------------------------------
//AutoDropdown.cpp
//------------------------------------------------------
//Define the AutoDropdown class

#include "AutoDropdown.h"

AutoDropdown::AutoDropdown()
{

}

AutoDropdown::~AutoDropdown()
{
	Shutdown();
}

bool AutoDropdown::Initialize(ID3D11Device* device, shared_ptr<Font> font, wstring texture, wstring directory, 
							  int imageWidth, int imageHeight, XMFLOAT2 pos, std::function<void(int)> func, int display, float boxMod, float scale)
{
	bool result;
	vector<string> text;

	//Read the folder of the directory provided and add the file names into the text vector
	Util::ReadFolder(text, directory);

	//Call the base initialize
	result = Dropdown::Initialize(device, imageWidth, imageHeight, pos, display);
	if(!result)
		return false;

	//Populate this dropdown with the buttons using the file names gathered earlier
	for(int c = 0; c < text.size(); c++)
	{
		m_pButtons.push_back(shared_ptr<Button>(new Button));
		result = m_pButtons[c]->Initialize(device, &texture, imageWidth, imageHeight, XMFLOAT2(pos.x, pos.y + (m_offset * c)), ONCE, boxMod);
		if(!result)
			return false;

		m_pButtons[c]->InitializeText(device, font, text[c], text[c].size() + 1, Color::White(), scale);
		m_pButtons[c]->SetFunction(func, c);
	}

	//Adjust the number of buttons displayed if the button count is less then the display count
	if(m_pButtons.size() > display)
		m_display = display;
	else
	{
		m_display = m_pButtons.size();
		m_short = true;
	}

	//Clear the vector
	text.clear();

	return true;
}

void AutoDropdown::Update(float gameTime, BoundingBox* box)
{
	Dropdown::Update(gameTime, box);
}

bool AutoDropdown::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Dropdown::Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void AutoDropdown::Shutdown()
{

}