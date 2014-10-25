//-----------------------------------------------------
//LevelEditorEnvironment.cpp
//-----------------------------------------------------
//

#include "LevelEditorEnemies.h"

LevEdEnemies::LevEdEnemies()
{
	e_waypoint = NONE;
	m_pWaypoint = 0;
	m_pSelected = 0;
}

LevEdEnemies::~LevEdEnemies()
{
	Shutdown();
}

bool LevEdEnemies::Initialize(ID3D11Device* device, shared_ptr<Font> font, shared_ptr<EditorGrid> grid)
{
	bool result;

	//Button dimensions
	float buttonWidth = 150;
	float buttonHeight = 65;

	//Store the Grid
	m_pGrid = grid;

	//Create and Initialize the Text
	m_pText = shared_ptr<Text>(new Text);
	result = m_pText->Initialize(device, font, "Waypoints", 15, XMFLOAT2(440 * ASPECT.x, 210 * ASPECT.y), Color::White(), 1.5f);
	if(!result)
		return false;

	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, 140 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(device, font, "Select", 10, Color::White(), 2.0f);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, 60 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(device, font, "Add", 4, Color::White(), 2.0f);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, -20 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(device, font, "Remove", 7, Color::White(), 2.0f);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[3]->Initialize(device, &StringManager::GUIBlank(), buttonWidth, buttonHeight, XMFLOAT2(515 * ASPECT.x, -100 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[3]->InitializeText(device, font, "Apply", 7, Color::White(), 2.0f);
	if(!result)
		return false;
	
	//Set the Callbacks for each button
	m_pButtons[0]->SetFunction(std::bind(&LevEdEnemies::SetState, this, _1), SELECT);
	m_pButtons[1]->SetFunction(std::bind(&LevEdEnemies::SetState, this, _1), PLACE);
	m_pButtons[2]->SetFunction(std::bind(&LevEdEnemies::SetState, this, _1), REMOVE);
	m_pButtons[3]->SetFunction(std::bind(&LevEdEnemies::SetState, this, _1), CONFIRM);

	//Null the Selected Node Pointer
	m_pSelected = 0;
	//Create and Initializ the Editor Waypoint Renderer
	m_pWaypoint = shared_ptr<EdWaypointRender>(new EdWaypointRender);
	result = m_pWaypoint->Initialize(device, font, m_pGrid);
	if(!result)
		return false;

	//Create and initialize the Toggle object
	m_pToggle = shared_ptr<Static2D>(new Static2D);
	result = m_pToggle->Initialize(device, StringManager::Connection(), buttonWidth * ASPECT.x, buttonHeight * ASPECT.y, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	XMFLOAT4 gray = Color::Gray();
	m_pToggle->SetColor(XMFLOAT4(gray.x, gray.y, gray.z, 0.5f));

	return true;
}

void LevEdEnemies::Update(float gameTime, XMFLOAT2 offset)
{
	//Set the Offset and Update each object for proper placement when moving the camera
	m_pText->SetOffset(offset);
	m_pText->Update();

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c]->SetOffset(offset);
		m_pButtons[c]->Update(gameTime);
	}

	switch(e_waypoint)
	{
	case NONE: break;
	default:
		m_pToggle->SetPosition(m_pButtons[e_waypoint - 1]->GetPosition());
		m_pToggle->SetOffset(offset);
		break;
	}

	//Enter the Confirmation flow
	if(e_waypoint == CONFIRM)
	{
		//Was a Node Selected
		if(m_pSelected)
		{
			//Reset the any waypoints already assigned to the node
			m_pSelected->ResetWaypoints();

			//Fill the Waypoint with new waypoint data
			for(int c = 0; c < m_pos.size(); c++)
				m_pSelected->AddWaypoint(m_pos[c]);
		}

		//Waypoint confirmation reset
		e_waypoint = NONE;
	}
}

bool LevEdEnemies::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pText->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	if(e_waypoint != NONE)
	{
		result = m_pToggle->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Render the waypoints - needed for draw order
bool LevEdEnemies::RenderWaypointOverlay(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pWaypoint->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void LevEdEnemies::Shutdown()
{
	//Release Text
	if(m_pText)
	{
		m_pText.reset();
		m_pText = 0;
	}

	//Release Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release Grid
	if(m_pGrid)
	{
		m_pGrid.reset();
		m_pGrid = 0;
	}

	//Release Toggle
	if(m_pToggle)
	{
		m_pToggle.reset();
		m_pToggle = 0;
	}

	//Release the Waypoint Render
	if(m_pWaypoint)
	{
		m_pWaypoint.reset();
		m_pWaypoint = 0;
	}
}

int LevEdEnemies::GetState()
{
	return e_waypoint;
}
//Set the Waypoint State
void LevEdEnemies::SetState(int value)
{
	if(value == e_waypoint)
		value = 0;
	
	switch(value)
	{
	case 0: e_waypoint = NONE; break;
	case 1: e_waypoint = SELECT; break;
	case 2: e_waypoint = PLACE; break;
	case 3: e_waypoint = REMOVE; break;
	case 4: e_waypoint = CONFIRM; break;
	}
}

//Called from the Level Editor Class
void LevEdEnemies::SelectedNode(int x, int y)
{
	//Use the Waypoint State to update the correct area
	switch(e_waypoint)
	{
	case NONE: break;
	case SELECT://Select a waypoint and repopulate the waypoint renderer with data
		{
			//Make sure the Selected node is an enemy
			if(!m_pGrid->GetNode(x, y)->GetEnemy())
				return;

			//Clear old data
			m_pos.clear();
			m_pWaypoint->Clear();

			//Assign the selected node and waypoint
			m_pSelected = m_pGrid->GetNode(x, y);
			m_pWaypoint->SetSelected(x, y);

			//Repopulate the Waypoint with this nodes waypoint data
			EdWaypoint point = m_pSelected->GetWaypoints();
			for(int c = 0; c < point.Length(); c++)
			{
				m_pos.push_back(point.GetWaypoint(c));
				m_pWaypoint->AddWaypoint(point.GetWaypoint(c));
			}
		}
		break;
	case PLACE://Add a new waypoint to the waypoint data
		if(m_pSelected)
		{
			if(m_pGrid->GetNode(x, y)->GetNodeState())
			{
				int index = 0;
				//Ensure the selected node doesn't already have a waypoint for the selected node
				bool found = FindWaypoints(x, y, index);

				if(!found)
				{	
					//Add new waypoint
					m_pos.push_back(XMFLOAT2(x,y));
					m_pWaypoint->AddWaypoint(x, y);
				}
			}
		}
		break;
	case REMOVE://Remove a pre-existing waypoint from the waypoint renderer
		if(m_pSelected)
		{
			int index = 0;
			//Ensure the Node has a waypoint to remove
			bool found = FindWaypoints(x, y, index);

			if(found)
			{
				//Remove the Waypoint
				m_pos.erase(m_pos.begin() + index);
				m_pWaypoint->RemoveWaypoint(index);
			}
		}
		break;
	}
}

//Check to see if this index already exists within the waypoint renderer
bool LevEdEnemies::FindWaypoints(int x, int y, int& index)
{
	for(int c = 0; c < m_pos.size(); c++)
	{
		if(XMVector2Equal(XMLoadFloat2(&XMFLOAT2(x,y)), XMLoadFloat2(&m_pos[c])))
		{
			index = c;
			return true;
		}
	}

	return false;
}