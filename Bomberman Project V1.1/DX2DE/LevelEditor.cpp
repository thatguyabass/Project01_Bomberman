//----------------------------------------------------------
//LevelEditor.cpp
//----------------------------------------------------------
//Define the Level Editor Class

#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
	//Set the default editor states
	e_state = LEVEL;
	e_submit = NONE;
}

LevelEditor::~LevelEditor()
{
	Shutdown();
}

//New Level Creation Flow
bool LevelEditor::Initialize(ID3D11Device* device, string name, int X, int Y, shared_ptr<Font> font, function<void(void)> backCallback)
{
	bool result;

	//Set the level variables
	m_X = X;
	m_Y = Y;
	m_name = name;

	//Create the Level Grid
	m_pGrid = shared_ptr<EditorGrid>(new EditorGrid);
	result = m_pGrid->Initialize(device, m_X, m_Y);
	if(!result)
		return false;

	//Base Initialize
	result = Initialize(device, font, backCallback);
	if(!result)
		return false;

	return true;
}

//Edit an existing level
bool LevelEditor::Initialize(ID3D11Device* device, string name, LevelInfo* level, shared_ptr<Font> font, function<void(void)> backCallback)
{
	bool result;

	//Set the level variables
	m_X = level->GetLevelData()->GetX();
	m_Y = level->GetLevelData()->GetY();
	m_name = name;

	//Create the Grid
	m_pGrid = shared_ptr<EditorGrid>(new EditorGrid);
	result = m_pGrid->Initialize(device, level);
	if(!result)
		return false;

	//Base Initialize
	result = Initialize(device, font, backCallback);
	if(!result)
		return false;

	return true;
}

bool LevelEditor::Initialize(ID3D11Device* device, shared_ptr<Font> font, function<void(void)> backCallback)
{
	bool result;
	float GUIWidth = 255;
	float mainGUIHeight = 610;
	float buttonHeight = 75;

	//Create the Background
	m_pBackground = shared_ptr<Static2D>(new Static2D);

	//Initialize the background
	result = m_pBackground->Initialize(device, StringManager::LevEdBackground(), GUIWidth * ASPECT.x, mainGUIHeight * ASPECT.y, XMFLOAT2(515 * ASPECT.x, -50 * ASPECT.y), 0);
	if(!result)
		return false;
	
	//Create a bounding box to block out button input when the hud is over the level
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(GUIWidth * ASPECT.x, Global::GetScreenHeight() * ASPECT.y, XMFLOAT2(0,0));

	//Create and initialize the buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), GUIWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, 330 * ASPECT.y), ONCE);
	if(!result)
		return false;	
	result = m_pButtons[0]->InitializeText(device, font, "Level", 7, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), GUIWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, 260 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Navigation", 15, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(device, &StringManager::GUIBlank(), 200, buttonHeight, XMFLOAT2(515 * ASPECT.x, -180 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(device, font, "Save", 5, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[3]->Initialize(device, &StringManager::GUIBlank(), 200, buttonHeight, XMFLOAT2(515 * ASPECT.x, -280 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[3]->InitializeText(device, font, "Back", 5, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons[0]->SetColor(Color::Red());

	//SetState callbacks assigned to each button
	m_pButtons[0]->SetFunction(std::bind(&LevelEditor::SetState, this, _1), 0);
	m_pButtons[1]->SetFunction(std::bind(&LevelEditor::SetState, this, _1), 1);
	m_pButtons[2]->SetFunction(std::bind(&LevelEditor::Submit, this));
	m_pButtons[3]->SetFunction(backCallback); //<- Return to the main menu

	//Create and initialize the level creation editor
	m_pEnviro = shared_ptr<LevEdEnvironment>(new LevEdEnvironment);
	result = m_pEnviro->Initialize(device, font);
	if(!result)
		return false;

	//Create and initialize the Enemy Waypoint Editor
	m_pEnemies = shared_ptr<LevEdEnemies>(new LevEdEnemies);
	result = m_pEnemies->Initialize(device, font, m_pGrid);
	if(!result)
		return false;

	//Create and Initialize the Message Prompt
	m_pPrompt = shared_ptr<MessagePrompt>(new MessagePrompt);
	result = m_pPrompt->Initialize(device, font);
	if(!result)
		return false;

	return true;
}

void LevelEditor::Update(float gameTime, XMFLOAT2 offset, BoundingBox* cursor)
{
	//Set the Offset and Update each object for proper placement when moving the camera
	m_pBackground->SetOffset(offset);
	m_pBackground->Update(gameTime);

	m_pBox->Update(XMFLOAT2((515 * ASPECT.x) + offset.x, 0 - offset.y));

	m_pPrompt->Update(gameTime, offset);

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c]->SetOffset(offset);
		m_pButtons[c]->Update(gameTime, cursor);
	}

	//Update the current Editor Selection
	switch(e_state)
	{
		case LEVEL: m_pEnviro->Update(gameTime, offset); break;
		case ENEMIES: m_pEnemies->Update(gameTime, offset); break;
	}

	//Update the level grid
	m_pGrid->Update(gameTime);
	for(int c = 1; c < m_X - 1; c++)//Start at index 1 and -1 in the count to not update the edge of the grid - Buffer zone
	{
		for(int v = 1; v < m_Y - 1; v++)//Start at index 1 and -1 in the count to not update the edge of the grid - Buffer zone
		{
			//If the button is intersecting the Box do not update this button - It is behind the Hud
			if(!m_pGrid->GetNode(c,v)->CheckBounds(m_pBox.get()))
			{
				//Is the cursor within the nodes bounding box
				if(m_pGrid->GetNode(c, v)->CheckBounds(cursor))
				{
					//Was mouse input given
					if(Input::GetCurrentMouse() & MOUSE_LEFT)
					{
						switch(e_state)
						{
						case LEVEL: m_pGrid->SwapTile(c, v, m_pEnviro->GetState()); break;//Used this method to avoid passing the level grid to the EnrivoEditor Object
						case ENEMIES: m_pEnemies->SelectedNode(c, v); break;
						case WAYPOINTS: break;
						}
					}
				}
			}
		}
	}
}

bool LevelEditor::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pGrid->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	if(e_state == ENEMIES)
	{
		result = m_pEnemies->RenderWaypointOverlay(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pBackground->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;
	
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	switch(e_state)
	{
		case LEVEL:
			result = m_pEnviro->Render(context, textureShader, world, view, ortho);
			if(!result)
				return false;
		break;
		case ENEMIES:
			result = m_pEnemies->Render(context, textureShader, world, view, ortho);
			if(!result)
				return false;
		break;
	}

	result = m_pPrompt->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void LevelEditor::Shutdown()
{
	//Release Background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}

	//Release Level Editor Environment object
	if(m_pEnviro)
	{
		m_pEnviro.reset();
		m_pEnviro = 0;
	}

	//Relase Enemy Editor
	if(m_pEnemies)
	{
		m_pEnemies.reset();
		m_pEnemies = 0;
	}

	//Release Box
	if(m_pBox)
	{
		m_pBox.reset();
		m_pBox = 0;
	}

	//Release Message Prompt
	if(m_pPrompt)
	{
		m_pPrompt.reset();
		m_pPrompt = 0;
	}

	//Release the Toggle
	if(m_pToggle)
	{
		m_pToggle.reset();
		m_pToggle = 0;
	}

	//Release the level grid
	if(m_pGrid)
	{
		m_pGrid.reset();
		m_pGrid = 0;
	}

	//Release the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();
}

//Set the Editor State - Button Callbacks
void LevelEditor::SetState(int value)
{
	switch(value)
	{
		case 0: e_state = LEVEL;		break;
		case 1: e_state = ENEMIES;		break;
		case 2: e_state = WAYPOINTS;	break;
		default: e_state = LEVEL;		break;
	}
}

//Submit the level created/edited
void LevelEditor::Submit()
{
	bool result;
	
	e_submit = SUMBIT_INITIALIZE;
	//Submit
	result =  m_pGrid->Submit(m_name);

	//Display a message prompt to the user if the submit was successful or not
	if(!result)
	{
		e_submit = SUBMIT_FAILED;
		m_pPrompt->Display(3.5f, "Save Failed", Color::Red(), XMFLOAT2(0, -250 * ASPECT.y), XMFLOAT2(300 * ASPECT.x, 150 * ASPECT.y), 2.0f);
	}
	else
	{
		e_submit = SUBMIT_SUCCESS;
		m_pPrompt->Display(3.5f, "Save Successful", Color::White(), XMFLOAT2(0, -250 * ASPECT.y), XMFLOAT2(400 * ASPECT.x, 150 * ASPECT.y), 2.0f);
	}
}

XMFLOAT2 LevelEditor::GetGridDimensions()
{
	return m_pGrid->GetLevelDimensions();
}