//-----------------------------------------------------------
//LevelEditorWaypointRender.cpp
//-----------------------------------------------------------
//Define the EdWaypointRender class

#include "LevelEditorWaypointRender.h"

EdWaypointRender::EdWaypointRender()
{
	m_pSelected = 0;
	m_pFont = 0;
	m_pGrid = 0;
	m_render = false;

	m_textCount = 0;
}

EdWaypointRender::~EdWaypointRender()
{
	Shutdown();
}

bool EdWaypointRender::Initialize(ID3D11Device* device, shared_ptr<Font> font, shared_ptr<EditorGrid> grid)
{
	bool result;

	//Set the Variables
	m_pDevice = device,
	m_pFont = font;
	m_pGrid = grid;

	//Get the Nodes Dimensions
	XMFLOAT2 D = m_pGrid->GetNodeDimensions();

	//Create and Initialize the Selected Object
	m_pSelected = shared_ptr<Static2D>(new Static2D);
	result = m_pSelected->Initialize(device, StringManager::Connection(), D.x, D.y, m_pGrid->GetNodeDimensions(), 0);
	if(!result)
		return false;

	XMFLOAT4 gray = Color::Teal();
	m_pSelected->SetColor(XMFLOAT4(gray.x, gray.y, gray.z, 0.5f));

	return true;
}

bool EdWaypointRender::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	if(!m_render)
		return true;

	bool result;

	result = m_pSelected->Render(context, textureShader, world, view, ortho);
	if(!result)
		return false;

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Set the selected Tile
void EdWaypointRender::SetSelected(int x, int y)
{
	m_render = true;
	m_pSelected->SetPosition(m_pGrid->GetPosition(x, y));
}
//Turn off the Renderer
void EdWaypointRender::Deselect()
{
	m_render = false;
}
bool EdWaypointRender::GetRender()
{
	return m_render;
}

//Add a waypoint to be rendered
void EdWaypointRender::AddWaypoint(int x, int y)
{
	m_pText.push_back(shared_ptr<Text>(new Text));
	m_pText[m_pText.size() - 1]->Initialize(m_pDevice, m_pFont, std::to_string(m_pText.size()), 4, m_pGrid->GetPosition(x, y), Color::White(), 2);
}
void EdWaypointRender::AddWaypoint(XMFLOAT2 index)
{
	AddWaypoint(index.x, index.y);
}

//Remove a waypoint from the list
void EdWaypointRender::RemoveWaypoint(int index)
{
	m_pText[index].reset();
	m_pText.erase(m_pText.begin() + index);

	for(int c = 0; c < m_pText.size(); c++)
		m_pText[c]->SetText(std::to_string(c + 1));
}

//Clear the Waypoints
void EdWaypointRender::Clear()
{
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();
}

void EdWaypointRender::Shutdown()
{
	//Release teh Selected Object
	if(m_pSelected)
	{
		m_pSelected.reset();
		m_pSelected = 0;
	}

	//Release teh Text Object
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();

	//Release teh Font object
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}

	//Release the Grid object
	if(m_pGrid)
	{
		m_pGrid.reset();
		m_pGrid = 0;
	}
	
	//Release the device Reference
	if(m_pDevice)
		m_pDevice = 0;
}