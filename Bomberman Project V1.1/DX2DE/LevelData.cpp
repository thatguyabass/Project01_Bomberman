//-------------------------------------------------------
//LevelData.cpp
//-------------------------------------------------------
//Define the Level Data Class and create the playable level

#include "LevelData.h"

LevelData::LevelData()
{
	m_pBox = 0;
	m_connectionCount = 0;
}
LevelData::LevelData(const LevelData& src)
{

}

LevelData::~LevelData()
{
	Shutdown();
}

bool LevelData::Initialize(ID3D11Device* device, LData* level)
{
	bool result;

	//Set the Node Dimension counts
	m_X = level->GetX();
	m_Y = level->GetY();

	//Get the width and height of the Tiles
	int width = 100 * ASPECT.x;
	int height = 100 * ASPECT.y;

	//Get the starting position modifiers
	float startX = (width / 2);
	float startY = -(height / 2);

	//Create and Initialize Level Nodes
	int id = 0;
	m_ppNode = new Node*[m_Y];
	for(int c = 0; c < m_Y; c++)
	{
		m_ppNode[c] = new Node[m_X];
		for(int v = 0; v < m_X; v++)
			m_ppNode[c][v].Initialize(device, width,  height, level->GetChar(c, v), XMFLOAT2(startX + (width * v), startY - (height * c)), ++id, XMFLOAT2(v,c), m_spawn, m_exit);
	}

	BaseGrid::SetPathfinding(device);

	//Set up the Level Bounding Box to be used with the camera 
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(width * m_X, (height * m_Y) - (50 * ASPECT.y), XMFLOAT2(((width * m_X) / 2), (-((height * m_Y) + (25 * ASPECT.y)) / 2)));

	return true;
}


void LevelData::Shutdown()
{
	//Release box collider
	if(m_pBox)
	{
		m_pBox.reset();
		m_pBox = 0;
	}
}

//Generate a new path for this enemy object
void LevelData::FindPath(Enemy* enemy)
{
	m_pPath->FindPath(enemy);
}

void LevelData::ModifyLevel(int x, int y)
{
	//destroy this Node
	bool result = GetNode(x, y)->Destroy();
	
	//If the node was destoryed, roll an power up drop
	if(result)
	{
		if(!m_powerUpCallback._Empty())
			m_powerUpCallback(GetNode(x, y)->GetPosition());
	}
}

//Adjust the level grid to make this node pathable or not pathable
void LevelData::ModifyPath(int x, int y, int state)
{
	GetNode(x, y)->SetNodeState(state);
}

BoundingBox* LevelData::GetBox()
{
	return m_pBox.get();
}

//Call the power up method when a tile is destroyed
void LevelData::SetPowerUpCallback(function<void(XMFLOAT2)> callback)
{
	m_powerUpCallback = callback;
}

//Find the closest index to the provided position
void LevelData::GetClosestIndex(XMFLOAT2 position, XMFLOAT2& index)
{
	int x = 0;
	int y = 0;
	float distance = 10000;
	int range = 2;
	int minX, maxX;
	int minY, maxY;

	//Search around the current index - reduce the amount of time spent in the loop
	minX = index.x - range;
	maxX = range + index.x;
	if(minX < 1) minX = 1;
	if(maxX > m_X - 1) maxX = m_X -1;

	minY = index.y - range;
	maxY = range + index.y;
	if(minY < 1) minY = 1;
	if(maxY > m_Y - 1) maxY = m_Y - 1;

	for(int c = minX; c < maxX; c++)
	{
		for(int v = minY; v < maxY; v++)
		{
			//Loop through the nodes in the range and find the closet to the position
			XMFLOAT2 pos = GetNode(c, v)->GetPosition();
			float dist = sqrt(pow(pos.x - position.x,2) + pow(pos.y - position.y, 2));

			if(distance > dist)
			{
				//Store the distance for furture uses and store the index
				distance = dist;
				x = c;
				y = v;
			}
		}
	}

	//Update the Index
	index = XMFLOAT2(x,y);
}