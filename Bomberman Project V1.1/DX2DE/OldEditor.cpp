//----------------------------------------------------------------------
//OldEditor.cpp
//----------------------------------------------------------------------
//

#include "OldEditor.h"

OldEditor::OldEditor()
{
	m_level = '\0';
}

OldEditor::~OldEditor()
{
	Shutdown();
}

bool OldEditor::Initialize(ID3D11Device* device, shared_ptr<Font> font, function<void(int)> callback)
{
	bool result;

	//Create and Initialize the Auto Dropdown
	m_pDrop = shared_ptr<AutoDropdown>(new AutoDropdown);
	result = m_pDrop->Initialize(device, font, StringManager::GUIBlank(), StringManager::CustomLevelDirectory(), 200, 100, XMFLOAT2(-450 * ASPECT.x, 270 * ASPECT.y),
		std::bind(&OldEditor::InitializeLevel, this, _1), 5);
	if(!result)
		return false;

	//Create and Initialize the Text
	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[0]->Initialize(device, font, "Level", 8, XMFLOAT2(100 * ASPECT.x, 200 * ASPECT.y), Color::White(), 3.0f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[1]->Initialize(device, font, "No Level Selected", 45, XMFLOAT2(0 * ASPECT.x, 150 * ASPECT.y), Color::White(), 2.5f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[2]->Initialize(device, font, "Level Width", 15, XMFLOAT2(-100 * ASPECT.x, 0 * ASPECT.y), Color::White(), 3.0f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[3]->Initialize(device, font, "XX", 3, XMFLOAT2(-20 * ASPECT.x, -50 * ASPECT.y), Color::White(), 2.5f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[4]->Initialize(device, font, "Level Height", 15, XMFLOAT2(250 * ASPECT.x, 0 * ASPECT.y), Color::White(), 3.0f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[5]->Initialize(device, font, "XX", 3, XMFLOAT2(330 * ASPECT.x, -50 * ASPECT.y), Color::White(), 2.5f);
	if(!result)
		return false;

	//Create and Initialize the Submit button
	m_pButton = shared_ptr<Button>(new Button);
	result = m_pButton->Initialize(device, &StringManager::GUIBlank(), 200, 100, XMFLOAT2(150 * ASPECT.x, 270 * ASPECT.y), ONCE);
	if(!result)
		return false;

	result = m_pButton->InitializeText(device, font, "Edit Level", 20, Color::White(), 2.0f);
	if(!result)
		return false;

	//Set the Submit Buttons Callback
	m_pButton->SetFunction(callback, 3);

	return true;
}

void OldEditor::Update(float gameTime, BoundingBox* box)
{
	m_pDrop->Update(gameTime, Global::GetCursor()->GetBox());

	//Check if the level is ready for editing
	if(Complete())
		m_pButton->Update(gameTime, Global::GetCursor()->GetBox());
}

bool OldEditor::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pButton->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pDrop->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}
	
//Mark the level ready for editing
void OldEditor::InitializeLevel(int num)
{
	//Set the button selected
	m_pDrop->SetSelected(num, Color::LightGray());
	m_level = m_pDrop->GetString(num);
	//Create a file path
	string TS = StringManager::CustomLevelDir() + m_level;
	wstring temp(TS.begin(), TS.end());
	
	m_pText[1]->SetText(m_level);
	//Read the Selected Levels data
	ReadFile(temp);
}
	
//Get the information required for creating a level
void OldEditor::GetLevelData(string& name, LevelInfo& level)
{
	//Create the name from the file path
	string trueName = m_level;
	trueName.erase(trueName.end() - 4, trueName.end());
	name = trueName;

	//Create the file path
	string path = StringManager::CustomLevelDir() + m_level;
	wstring levelPath(path.begin(), path.end()); 

	//Read the file
	level.ReadFile(levelPath);
}

void OldEditor::Shutdown()
{
	//Release the Auto Dropdown
	if(m_pDrop)
	{
		m_pDrop.reset();
		m_pDrop = 0;
	}

	//Release the Submit Button
	if(m_pButton)
	{
		m_pButton.reset();
		m_pButton = 0;
	}

	//Release the Text
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();
}

//Read the file to display more information to the user
bool OldEditor::ReadFile(wstring level)
{
	ifstream in;
	char input;
	int X;
	int Y;

	in.open(level.c_str());

	if(in.fail())
		return false;

	in >> X;	

	in.get(input);
	while(input != ':')
		in.get(input);

	in >> Y;

	in.close();
	
	m_pText[3]->SetText(std::to_string(X));
	m_pText[5]->SetText(std::to_string(Y));

	return true;
}

//Check if the level is ready to be edited
bool OldEditor::Complete()
{
	if(m_level[0] == '\0')
		return false;

	return true;
}