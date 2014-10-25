//-------------------------------------------------
//tile.cpp
//-------------------------------------------------
//Define the Tile class 

#include "EditorTile.h"

EditorTile::EditorTile()
{
	m_exit = false;
	m_spawn = false;
	m_enemy = false;
	m_invalid = false;
	m_type = -1;
}


EditorTile::~EditorTile()
{
	Shutdown();
}

bool EditorTile::Initialize(ID3D11Device* device, int imageWidth, int imageHeight, XMFLOAT2 position, XMFLOAT2 index, const shared_ptr<TileType> type)
{
	bool result;

	//Establish this tiles position on the grid
	m_index = index;

	//Set if this tile is either spawn or exit
	m_spawn = type->GetSpawn();
	m_exit = type->GetExit();

	//Get the tile state from the ID value
	e_tile = (TILE_STATE)type->GetID();
	m_type = type->GetType();//if type is not -1, this tile is an enemy
	if(m_type != -1)
		m_enemy = true;

	//Initialize the image
	result = Base2D::Initialize(device, imageWidth, imageHeight, position, 0);
	if(!result)
		return false;

	//Setup the bounding box
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(imageWidth, imageHeight, position);

	//Swap the current texture for the tile types texture
	SwapTexture(type->GetTexture());

	//Create a new Texture
	m_pNode = shared_ptr<Static2D>(new Static2D);
	result = m_pNode->Initialize(device, StringManager::Node(), imageWidth / 5, imageHeight / 5, position, 0);
	if(!result)
		return false;

	//Set the pathfinding node 
	NodeSetup(type->GetID());

	//Create and Initialize the Invalid Object
	m_pInvalid = shared_ptr<Static2D>(new Static2D);
	result = m_pInvalid->Initialize(device, StringManager::Connection(), imageWidth, imageHeight, position, 0);
	if(!result)
		return false;

	//Set the Invalid Color
	XMFLOAT4 color = Color::Red();
	m_pInvalid->SetColor(XMFLOAT4(color.x, color.y, color.z, 0.5f));

	return true;
}

bool EditorTile::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = BaseNode::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	if(m_invalid)
	{
		result = m_pInvalid->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Check this tiles bounding box to another
bool EditorTile::CheckBounds(BoundingBox* box)
{
	return (box->BoundingBoxIntersect(m_pBox.get()));
}

void EditorTile::Shutdown()
{
	if(m_pInvalid)
	{
		m_pInvalid.reset();
		m_pInvalid = 0;
	}
}

bool EditorTile::operator==(EditorTile& node)
{
	if(this == &node)
		return true;
	return false;
}
bool EditorTile::operator==(const EditorTile& node)
{
	if(this == &node)
		return true;
	return false;
}

//Swap this tile to a different type
void EditorTile::SwapTile(shared_ptr<TileType> type)
{
	//Swap the textures
	SwapTexture(type->GetTexture());
	
	//Set if this tile is now spawn or exit
	m_spawn = type->GetSpawn();
	m_exit = type->GetExit();

	//Get the tile type
	m_type = type->GetType();
	if(m_type != -1)//if type is -1 it is not an enemy
		m_enemy = true;
	else
		m_enemy = false;

	//Set the tile state
	e_tile = (TILE_STATE)type->GetID();

	//Pathfinding setup
	NodeSetup(type->GetID());
}

//Swap this texture to a new texture
void EditorTile::SwapTexture(shared_ptr<Texture> texture)
{
	//If a texture already exists, release it
	if(m_pTexture)
	{
		m_pTexture.reset();
		m_pTexture = 0;
	}
	
	//Set the new texture to the provided texture
	m_pTexture = texture;
}

//Set this tile for pathfinding
void EditorTile::NodeSetup(int value)
{
	switch(value)
	{
	case NO_SPACE:
		if(_DEBUG_PATHFINDING)
			m_pNode->SetColor(XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f));
		e_node = INACTIVE;//pathfinding entities will avoid this tile
		break;
	default:
		if(_DEBUG_PATHFINDING)
			m_pNode->SetColor(Color::Blue());
		e_node = ACTIVE;//Pathfinding entities will path through this tile
	}
}

//Spawn / Exit access methods
bool EditorTile::GetSpawn()
{
	return m_spawn;
}
bool EditorTile::GetExit()
{
	return m_exit;
}

//Enemy access methods
bool EditorTile::GetEnemy()
{
	return m_enemy;
}
bool EditorTile::WaypointEmpty()
{
	return (m_waypoint.Length() == 0);
}

//Waypoint methods
void EditorTile::AddWaypoint(XMFLOAT2 point)
{
	m_waypoint.SetWaypoint(point);
}
void EditorTile::RemoveWaypoint(XMFLOAT2 point)
{
	m_waypoint.RemoveWaypoint(point);
}
void EditorTile::ResetWaypoints()
{
	m_waypoint.Reset();
}
EdWaypoint& EditorTile::GetWaypoints()
{
	return m_waypoint;
}
int EditorTile::GetWaypointCount()
{
	return m_waypoint.Length();
}

void EditorTile::SetInvalid(bool value)
{
	m_invalid = value;
}
bool EditorTile::GetInvalid()
{
	return m_invalid;
}