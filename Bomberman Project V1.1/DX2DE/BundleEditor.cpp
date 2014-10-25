//----------------------------------------------------------------
//BundleEditor.cpp
//----------------------------------------------------------------
//

#include "BundleEditor.h"

BundleEditor::BundleEditor()
{
	e_edit = NONE;
}

BundleEditor::~BundleEditor()
{
	Shutdown();
}

bool BundleEditor::Initialize(ID3D11Device* device, shared_ptr<Font> font)
{
	bool result;

	//Create a level bundle
	m_pBundle = shared_ptr<LevelBundle>(new LevelBundle);

	//Create and Initialize Text
	m_pText = shared_ptr<Text>(new Text);
	result = m_pText->Initialize(device, font, "Editor Level Bundles", 30, XMFLOAT2(-150 * ASPECT.x, 300 * ASPECT.y), Color::White(), 2.0f);
	if(!result)
		return false;

	//Create and initialize the Dynamic Dropdown
	m_pDrop = shared_ptr<DynamicDropdown>(new DynamicDropdown);
	result = m_pDrop->Initialize(device, font, StringManager::GUIBlank(), 200, 100, XMFLOAT2(-450 * ASPECT.x, 270 * ASPECT.y), 5, 1, 0.9f);
	if(!result)
		return false;

	//Create and initialize the Auto Dropdown
	m_pAuto = shared_ptr<AutoDropdown>(new AutoDropdown);
	result = m_pAuto->Initialize(device, font, StringManager::GUIBlank(), StringManager::BundleDirectory(), 200, 100, XMFLOAT2(0,  270 * ASPECT.y), std::bind(&BundleEditor::GetEditIndex, this,_1), 5, 1);
	if(!result)
		return false;

	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), 200, 100, XMFLOAT2(400 * ASPECT.x, 150 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Edit Bundle", 20, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), 200, 100, XMFLOAT2(400 * ASPECT.x, -150 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Save Bundle", 20, Color::White());
	if(!result)
		return false;

	//Set the buttons callbacks
	m_pButtons[0]->SetFunction(std::bind(&BundleEditor::EditBundle, this));
	m_pButtons[1]->SetFunction(std::bind(&BundleEditor::SaveBundle, this));

	//Create and Initialize the Text Box
	m_pBox = shared_ptr<TextBox>(new TextBox);
	result = m_pBox->Initialize(device, font, 300, 75, XMFLOAT2(0, 200 * ASPECT.y), false, "Name", 20);
	if(!result)
		return false;

	//Create and initialize the Message Prompt
	m_pPrompt = shared_ptr<MessagePrompt>(new MessagePrompt);
	result = m_pPrompt->Initialize(device, font);
	if(!result)
		return false;

	return true;
}

void BundleEditor::Update(float gameTime, BoundingBox* box)
{
	//Get the number of buttons
	int size = m_pButtons.size();
	//Update based on the Edit State
	if(e_edit == EDIT)
	{
		size -= 1;//Adjust the size to remove the save button from the update
		//Update the Auto Dropdown
		m_pAuto->Update(gameTime, box);
		m_pButtons[0]->SetText("Close");
	}
	else
	{
		m_pButtons[0]->SetText("Edit Bundle");
		//Update the Dynamic Dropdown
		m_pDrop->Update(gameTime, box);
		if(m_pDrop->GetState())
			m_pBox->Update(gameTime, box);
	}

	//Update the buttons
	for(int c = 0; c < size; c++) //Size determines if the Save button is updated
		m_pButtons[c]->Update(gameTime, box);

	m_pPrompt->Update(gameTime);
}

bool BundleEditor::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pBox->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	result = m_pText->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	result = m_pDrop->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	if(e_edit == EDIT)
	{
		result = m_pAuto->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pPrompt->Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void BundleEditor::Shutdown()
{
	//Rlease the Bounding Box
	if(m_pBox)
	{
		m_pBox.reset();
		m_pBox = 0;
	}

	//Release the Level Bundle
	if(m_pBundle)
	{
		m_pBundle.reset();
		m_pBundle = 0;
	}

	//Release the Dynamic Dropdown
	if(m_pDrop)
	{
		m_pDrop.reset();
		m_pDrop = 0;
	}

	//Release the Textbox
	if(m_pText)
	{
		m_pText.reset();
		m_pText = 0;
	}

	//Release the Message Prompt
	if(m_pPrompt)
	{
		m_pPrompt.reset();
		m_pPrompt = 0;
	}

	//Rlease the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release the Auto Dropdown
	if(m_pAuto)
	{
		m_pAuto.reset();
		m_pAuto = 0;
	}
}

void BundleEditor::SaveBundle()
{
	bool result;
	string name = m_pBox->GetText();
	if(name.size() < 4)//Ensure the name isn't too short
	{
		SaveMessage(false);
		return;
	}

	//Get the level names from teh dynamic dropdown
	vector<string> levels;
	m_pDrop->GetStrings(levels);

	//Write the names to a text file
	result = m_pBundle->WriteFile(name, levels);
	if(result)
		SaveMessage(true);
	else
		SaveMessage(false);

	levels.clear();
}
void BundleEditor::SaveMessage(bool pass)
{
	//Display a message to the player
	if(pass)
		m_pPrompt->Display(3.5f, "Save Successful", Color::White(), XMFLOAT2(0, -250 * ASPECT.y), XMFLOAT2(300 * ASPECT.x, 150 * ASPECT.y), 1.5f);
	else
		m_pPrompt->Display(3.5f, "Save Failed", Color::Red(), XMFLOAT2(0, -250 * ASPECT.y), XMFLOAT2(300 * ASPECT.x, 150 * ASPECT.y), 1.5f);
}

void BundleEditor::EditBundle()
{
	if(e_edit == NONE)
		e_edit = EDIT;
	else
		e_edit = NONE;
}

//Internal method to display the selected button in the auto dropdown and populate the dynamic dropdown
void BundleEditor::GetEditIndex(int num)
{
	m_pAuto->SetSelected(num, Color::LightGray());
	string path = m_pAuto->GetString(num);
	
	//Read the bundle
	m_pBundle->Initialize(path);

	//Populate the Bundle
	m_pDrop->Populate(m_pBundle->GetLevels());
}