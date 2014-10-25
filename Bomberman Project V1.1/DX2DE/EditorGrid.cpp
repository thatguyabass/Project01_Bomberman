//----------------------------------------------------------------------
//EditorGrid.cpp
//----------------------------------------------------------------------
//

#include "EditorGrid.h"

EditorGrid::EditorGrid()
{
	m_connectionCount = 0;
	m_typeCount = 11;
}

EditorGrid::~EditorGrid()
{
	Shutdown();
}

//Initialize using an existing level
bool EditorGrid::Initialize(ID3D11Device* device, LevelInfo* level)
{
	bool result;

	SetTiles(device);

	SetNodes(device, level->GetLevelData());

	SetPathfinding(device);

	//Set up the enemies
	for(int c = 0; c < level->GetEnemyCount(); c++)
		SetEnemies(level->GetEnemyData(c));

	return true;
}

//Initialize using new information
bool EditorGrid::Initialize(ID3D11Device* device, int X, int Y)
{
	bool result;

	SetTiles(device);

	SetNodes(device, X, Y);

	SetPathfinding(device);

	return true;
}

void EditorGrid::Shutdown()
{
	for(int c = 0; c < m_pTile.size(); c++)
	{
		m_pTile[c].reset();
		m_pTile[c] = 0;
	}
	m_pTile.clear();
}

//Swap the tiles for another tile
void EditorGrid::SwapTile(int x, int y, int value)
{
	m_spawn = FindSpawn();
	m_exit = FindExit();

	//Value can be:
	//NONE, SPACE, NO_SPACE, DESTRUCTIBLE, EXIT, SPAWN, HELI, YELLOW, MAD_GUY, MAD_BOMB, RABBIT, BLUE_EYE

	switch(value)
	{
	case 0: break;
	case 1: GetNode(x, y)->SwapTile(m_pTile[0]); break;//Space
	case 2: //No Space
		if(!CheckIndex(m_spawn) && !CheckIndex(m_exit))//If spawn and exit are present in the level
		{
			int last = GetNode(x, y)->GetTileState();	//Get the current tile state
			GetNode(x, y)->SwapTile(m_pTile[1]);		//Swap the Node
			if(!ClearPath(GetNode(m_exit.x,m_exit.y), GetNode(m_spawn.x, m_spawn.y)))//Does this block the spawn to exit?
				GetNode(x ,y)->SwapTile(m_pTile[last]);//Reset the tile because the level is illegal
		}
		else
			GetNode(x, y)->SwapTile(m_pTile[1]);//Swap the Node
			
		break;

	case 3: GetNode(x, y)->SwapTile(m_pTile[2]); break;//DESTRUCTIBLE
	case 4://EXIT
			if(!CheckIndex(m_spawn))
			{
				//Is there a clear path to the spawn
				if(!ClearPath(GetNode(x,y), GetNode(m_spawn.x, m_spawn.y)))
					break;
			}

			//Swap and Set the Exit
			GetNode(x, y)->SwapTile(m_pTile[3]);
			CheckExit(x, y);
		break;
	case 5: //SPAWN
			if(!CheckIndex(m_exit))
			{
				//Is there a clear path to the Exit
				if(!ClearPath(GetNode(x,y),  GetNode(m_exit.x, m_exit.y)))
					break;
			}

			//Swap and Set the Exit
			GetNode(x, y)->SwapTile(m_pTile[4]);
			CheckSpawn(x, y);
		break;
	case 6: //HELI
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[5]); 
		break;
	case 7://YELLOW
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[6]);
		break;
	case 8://MAD_GUY
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[7]); 
		break;
	case 9://MAD_BOMB
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[8]);
		break;
	case 10://RABBIT
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[9]);
		break;
	case 11://BLUE EYE
		GetNode(x, y)->ResetWaypoints();
		GetNode(x, y)->SwapTile(m_pTile[10]);
		break;
	}
	CheckInvalid();
}

//Create a Node
bool EditorGrid::InitializeTile(ID3D11Device* device, char type, int x, int y)
{
	bool result;

	int width = 100 * ASPECT.x;
	int height = 100 * ASPECT.y;
	float startX = 0;
	float startY = 0;

	int index = -1;
	//Get the index of the Current Node Type
	switch(type)
	{
	case 'o': index = EditorTile::SPACE; break;
	case 'x': index = EditorTile::NO_SPACE; break;
	case 'y': index = EditorTile::DESTRUCTIBLE; break;
	case 'e': index = EditorTile::EXIT; break;
	case 's': index = EditorTile::SPAWN; break;
	default: index = EditorTile::SPACE; break;
	}

	m_ppNode[x][y].Initialize(device, width, height, XMFLOAT2(startX + (width * y), startY - (height * x)), XMFLOAT2(y, x), m_pTile[index]); 

	return true;
}

//Find if the exit exists, and replace it - Only one exit can exist in one level
void EditorGrid::CheckExit(int x, int y)
{
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			if(c == y && v == x)
				continue;

			if(m_ppNode[c][v].GetExit())
			{
				m_ppNode[c][v].SwapTile(m_pTile[0]);
				m_exit = XMFLOAT2(v,c);
			}
		}
	}
}

//Find if the Spawn exists, and replace it - Only one Spawn can exist in one level
void EditorGrid::CheckSpawn(int x, int y)
{
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			if(c == y && v == x)
				continue;

			if(m_ppNode[c][v].GetSpawn())
			{
				m_ppNode[c][v].SwapTile(m_pTile[0]);
				m_spawn = XMFLOAT2(v,c);
			}
		}
	}
}

//Find the spawn index
XMFLOAT2 EditorGrid::FindSpawn()
{
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			if(m_ppNode[c][v].GetSpawn())
				return XMFLOAT2(v,c);
		}
	}
	return XMFLOAT2(-1, -1);
}

//Find the exit index
XMFLOAT2 EditorGrid::FindExit()
{
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			if(m_ppNode[c][v].GetExit())
				return XMFLOAT2(v,c);
		}
	}
	return XMFLOAT2(-1, -1);
}

//Check for a clear path between two nodes
bool EditorGrid::ClearPath(EditorTile* start, EditorTile* end)
{
	return m_pPath->FindPath(start, end);
}

//Does the index exist within the level?
bool EditorGrid::CheckIndex(XMFLOAT2 index)
{
	return XMVector2Equal(XMLoadFloat2(&index), XMLoadFloat2(&NOT_FOUND));
}

//Save the level to a text file
bool EditorGrid::Submit(string name)
{
	//Spawn and Exit must exist within the level
	if(CheckIndex(m_spawn) || CheckIndex(m_exit))
		return false;

	//A clear path between spawn and exit must exist
	if(!ClearPath(GetNode(m_spawn.x, m_spawn.y), GetNode(m_exit.x, m_exit.y)))
		return false;

	//Check if any enemy nodes are invalid
	bool result = CheckInvalid();
	if(!result)
		return false;

	//Create the File Path
	string file = StringManager::CustomLevelDir() + name + StringManager::LevelExtention();
	wstring wFile(file.begin(), file.end());//Convert to wstring

	ofstream out;
	//Possible Tile types
	char type[11] = {'o', 'x', 'y', 'e', 's', 'H', 'Y', 'G', 'B', 'R', 'L'};

	out.open(wFile);

	//Write out the width and height of the level
	out << m_X << ':' << m_Y << endl;

	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			int i = GetNode(v, c)->GetTileState();//Get the index of the tile state
			if(i >= 5) i = 0; //Reset the tile ID to be space - Enemies are not read from the level grid
			out << type[i];//Write out the ID value
		}
		out << endl;
	}

	out << endl;

	vector<EditorTile*> tile;//Create a vector of all the enemy tiles
	for(int c = 0; c < m_Y; c++)//Loop through the level finding all the enemy tiles
	{
		for(int v = 0; v < m_X; v++)
		{
			if(GetNode(v,c)->GetEnemy())
				tile.push_back(GetNode(v,c));//Store the enemy tiles into the vector
		}
	}

	//Write out the E (looks good in the file) and the enemy count
	out << 'E' << ':' << tile.size() << endl;
	for(int c = 0; c < tile.size(); c++)
	{
		int T = tile[c]->GetTileState();//Get the index of the tile state
		XMFLOAT2 index = tile[c]->GetIndex();//Get the level index of the tile location
		out << type[T] << ':' << (int)index.x << ':' << (int)index.y << endl;//Write out Enemy type, and tile index location

		EdWaypoint waypoint = tile[c]->GetWaypoints();//Get the waypoints attached to the enemy
		out << waypoint.Length() << endl;//Write out the waypoint count
		for(int v = 0; v < waypoint.Length(); v++)
		{
			XMFLOAT2 pos = waypoint.GetWaypoint(v);//Get the index position of the waypoints
			out << (int)pos.x << ':' << (int)pos.y << endl;//Write out the Waypoints index positions
		}
		out << endl;
	}

	tile.clear();//Clear the tile vector

	out.close();//Close the text file

	return true;
}

//Set up the tile types to be used when swaping tiles
bool EditorGrid::SetTiles(ID3D11Device* device)
{
	for(int c = 0; c < m_typeCount; c++)//Create the tile types
	{
		m_pTile.push_back(shared_ptr<TileType>(new TileType));
		if(!m_pTile[c])
			return false;
	}

	//Initialize the Tile Types
	//Level Tiles
	m_pTile[EditorTile::SPACE]->Initialize(device, StringManager::TileSpace(), EditorTile::SPACE, false, false);
	m_pTile[EditorTile::NO_SPACE]->Initialize(device, StringManager::TileNoSpace(), EditorTile::NO_SPACE, false, false);
	m_pTile[EditorTile::DESTRUCTIBLE]->Initialize(device, StringManager::TileRemovable(), EditorTile::DESTRUCTIBLE, false, false);
	m_pTile[EditorTile::EXIT]->Initialize(device, StringManager::TileExit(), EditorTile::EXIT, false, true);
	m_pTile[EditorTile::SPAWN]->Initialize(device, StringManager::TileSpawn(), EditorTile::SPAWN, true, false);

	//Enemy Tiles
	m_pTile[EditorTile::HELI]->Initialize(device, StringManager::TileHeli(), EditorTile::HELI, 0);
	m_pTile[EditorTile::YELLOW]->Initialize(device, StringManager::TileYellow(), EditorTile::YELLOW, 1);
	m_pTile[EditorTile::MAD_GUY]->Initialize(device, StringManager::TileMadGuy(), EditorTile::MAD_GUY, 2);
	m_pTile[EditorTile::MAD_BOMB]->Initialize(device, StringManager::TileMadBomb(), EditorTile::MAD_BOMB, 3);
	m_pTile[EditorTile::RABBIT]->Initialize(device, StringManager::TileRabbit(), EditorTile::RABBIT, 4);
	m_pTile[EditorTile::BLUE_EYE]->Initialize(device, StringManager::TileBlueEye(), EditorTile::BLUE_EYE, 5);

	return true;
}

//Set up the noodes for a new level
bool EditorGrid::SetNodes(ID3D11Device* device, int X, int Y)
{
	bool result;

	//Store the X and Y values
	m_X = X;
	m_Y = Y;

	//Create the Rows of Nodes
	m_ppNode = new EditorTile*[m_Y];
	m_ppNode[0] = new EditorTile[m_X];//Top Buffer zone - Create a buffer zone that can not be edited
	for(int c = 0; c < m_X; c++)
	{
		result = InitializeTile(device, 'x', 0, c);
		if(!result)
			return false;
	}

	for(int c = 1; c < m_Y - 1; c++)//Start the Loop at index 1 to not recreate the first row - Already created above
	{
		m_ppNode[c] = new EditorTile[m_X];

		//Left side buffer zone
		result = InitializeTile(device, 'x', c, 0);
		if(!result)
			return false;
		
		for(int v = 1; v < m_X - 1; v++)//Create the nodes within the buffer zone - these nodes will be editable
		{
			result = InitializeTile(device, 'o', c, v);
			if(!result)
				return false;
		}
		//Right side buffer zone
		result = InitializeTile(device, 'x', c, m_X -1);
		if(!result)
			return false;
	}

	m_ppNode[m_Y - 1] = new EditorTile[m_X];//Bottom Buffer Row
	for(int c = 0; c < m_X; c++)
	{
		result = InitializeTile(device, 'x', m_Y - 1, c);
		if(!result)
			return false;
	}

	return true;
}

//Set up the nodes using a pre-existing level
bool EditorGrid::SetNodes(ID3D11Device* device, LData* level)
{
	bool result; 

	//Store the X and Y values
	m_X = level->GetX();
	m_Y = level->GetY();

	//Recreate the level using the level data
	m_ppNode = new EditorTile*[m_Y];
	for(int c = 0; c < m_Y; c++)
	{
		m_ppNode[c] = new EditorTile[m_X];
		for(int v = 0; v < m_X; v++)
		{
			result = InitializeTile(device, level->GetChar(c, v), c, v);
			if(!result)
				return false;
		}
	}
	
	return true;
}

//Set up the level with pathfinding
void EditorGrid::SetPathfinding(ID3D11Device* device)
{
	//Find if spawn or exit already exist
	m_spawn = FindSpawn();
	m_exit = FindExit();

	BaseGrid<EditorTile>::SetPathfinding(device);
}

//Set up any enemies with their waypoints
void EditorGrid::SetEnemies(EData* data)
{
	//Find the tile at the data position
	EditorTile* tile = GetNode(data->GetPosition().x, data->GetPosition().y);
		
	//Get the enemy type
	char type = data->GetType();

	EditorTile::TILE_STATE e_tile;
	switch(type)
	{
	case 'H': e_tile = EditorTile::HELI; break;
	case 'Y': e_tile = EditorTile::YELLOW; break;
	case 'G': e_tile = EditorTile::MAD_GUY; break;
	case 'B': e_tile = EditorTile::MAD_BOMB; break;
	case 'R': e_tile = EditorTile::RABBIT; break;
	case 'L': e_tile = EditorTile::BLUE_EYE; break;
	}

	//Swap the current tile with the correct enemy tile
	tile->SwapTile(m_pTile[e_tile]);

	//Populate the enemies Waypoints
	for(int v = 0; v < data->GetCount(); v++)
		tile->AddWaypoint(data->GetWaypoint(v));
}

bool EditorGrid::CheckInvalid()
{
	//Loop through the Editable level and find all the enemies
	bool result = true;
	for(int c = 1;c < m_Y - 1; c++)
	{
		for(int v = 1; v < m_X - 1; v++)
		{
			if(GetNode(v, c)->GetEnemy())
			{
				if(CheckIndex(m_spawn))//Spawn must be in the level
				{
					result = false;
					GetNode(v, c)->SetInvalid();//INVALID
					continue;
				}

				if(!ClearPath(GetNode(v, c), GetNode(m_spawn.x, m_spawn.y)))//Is there a path to the spawn?
				{
					result = false;
					GetNode(v, c)->SetInvalid();//INVALID
				}
				else
					GetNode(v, c)->SetInvalid(false);//VALID
			}
			else if(GetNode(v, c)->GetInvalid())
				GetNode(v, c)->SetInvalid(false);
		}
	}
	if(!result)
		return false;

	return true;
}