//----------------------------------------------------
//NewEditor.cpp
//----------------------------------------------------
//

#include "NewEditor.h"

NewEditor::NewEditor()
{

}

NewEditor::~NewEditor()
{
	Shutdown();
}

bool NewEditor::Initialize(ID3D11Device* device, shared_ptr<Font> font, function<void(int)> callback)
{
	bool result;

	//Create and initialize Text
	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[0]->Initialize(device, font, "New Level", 16, XMFLOAT2(-200 * ASPECT.x, 325 * ASPECT.y), Color::White(), 7.5f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[1]->Initialize(device, font, "Enter Level Name", 25, XMFLOAT2(0 * ASPECT.x, 220 * ASPECT.y), Color::White(), 2.0f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[2]->Initialize(device, font, "Level Width", 20, XMFLOAT2(-60 * ASPECT.x, -10 * ASPECT.y), Color::White(), 2.0f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[3]->Initialize(device, font, "Level Height", 20, XMFLOAT2(260 * ASPECT.x, -10 * ASPECT.y), Color::White(), 2.0f);
	if(!result)
		return false;

	//Create and initialize the Submit Buttons
	m_pSubmit = shared_ptr<Button>(new Button);
	result = m_pSubmit->Initialize(device, &StringManager::GUIBlank(), 200,125, XMFLOAT2(-300 * ASPECT.x, 0), ONCE);
	if(!result)
		return false;
	result = m_pSubmit->InitializeText(device, font, "Start", 10, Color::White(), 2.5f);
	if(!result)
		return false;
	
	//Set the Submit callback
	m_pSubmit->SetFunction(callback, 3);
	
	//Create and initialize Text boxes
	m_pTextBox.push_back(shared_ptr<TextBox>(new TextBox));
	result = m_pTextBox[0]->Initialize(device, font, 500, 75, XMFLOAT2(150 * ASPECT.x, 150 * ASPECT.y), false, "Name", 20, 1.5f);
	if(!result)
		return false;

	m_pTextBox.push_back(shared_ptr<TextBox>(new TextBox));
	result = m_pTextBox[1]->Initialize(device, font, 100, 75, XMFLOAT2(50 * ASPECT.x, -100 * ASPECT.y), true, "00", 3, 2.5f);
	if(!result)
		return false;

	m_pTextBox.push_back(shared_ptr<TextBox>(new TextBox));
	result = m_pTextBox[2]->Initialize(device, font, 100, 75, XMFLOAT2(350 * ASPECT.x, -100 * ASPECT.y), true, "00", 3, 2.5f);
	if(!result)
		return false;

	return true;
}

void NewEditor::Update(float gameTime, BoundingBox* box)
{
	//Gather the Data to create the level
	ExtractData();

	//Check if the level is ready to be created
	if(GetComplete())
		m_pSubmit->Update(gameTime, box);

	//Update text boxes
	for(int c = 0; c < m_pTextBox.size(); c++)
		m_pTextBox[c]->Update(gameTime, box);
}

bool NewEditor::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	for(int c = 0; c < m_pTextBox.size(); c++)
	{
		result = m_pTextBox[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pSubmit->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void NewEditor::GetLevelInfo(string& name, int& x, int& y)
{
	name = m_name;
	x = m_X + 2;//Increase the X and Y size by to to create a buffer zone around the edge of the level
	y = m_Y + 2;
}

void NewEditor::Shutdown()
{
	//Rlease the Submit Button
	if(m_pSubmit)
	{
		m_pSubmit.reset();
		m_pSubmit = 0;
	}

	//Release the Text Boxes
	for(int c = 0; c < m_pTextBox.size(); c++)
	{
		m_pTextBox[c].reset();
		m_pTextBox[c] = 0;
	}
	m_pTextBox.clear();

	//Release the Text
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();
}

//Check if the level is ready to be created
bool NewEditor::GetComplete()
{
	if(m_name.size() < 4)
		return false;
	if(m_X <= 4)
		return false;
	if(m_Y <= 4)
		return false;

	return true;
}

void NewEditor::ExtractData()
{
	m_name = m_pTextBox[0]->GetText();

	m_X = atoi(m_pTextBox[1]->GetText().c_str());
	m_Y = atoi(m_pTextBox[2]->GetText().c_str());
}