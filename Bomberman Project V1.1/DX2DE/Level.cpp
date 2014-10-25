//------------------------------------
//Level.cpp
//------------------------------------
//

#include "Level.h"

Level::Level()
{
	//Set the base values
	m_pBomber = 0;

	e_level = PLAYING;
	m_enemyCount = 0;

	m_dropChance = 25;
	m_pHUD = 0;
}

Level::Level(const Level& src)
{
}

Level::~Level()
{
	Shutdown();
}

bool Level::Initialize(shared_ptr<D3D> d3d, wstring level, bool cont)
{
	bool result;

	//Set the Global Game State
	Util::e_globalGame = Util::GAME;

	result = BaseLevel::Initialize(d3d);
	if(!result)
		return false;

	//Read the Level file
	LevelInfo* info = new LevelInfo;
	info->ReadFile(level);

	//Create and Initialize the Grid
	result = InitializeGrid(info->GetLevelData());
	if(!result)
		return false;

	//Create and Initialize Bomberman
	result = InitializeBomberman(XMFLOAT2(50, 80), m_pLevelData->GetSpawn());
	if(!result)
		return false;

	//Create and Initialize the font and font shader
	result = InitializeHUD(cont);
	if(!result)
		return false;

	InitializeWaypoints(info);

	//Create, Initialize and add Enemies
	SetEnemy(info);

	for(int c = 0; c < m_pEnemy.size(); c++)
	{
		//Find a path for each enemy
		m_pLevelData->FindPath(m_pEnemy[c].get());
		if(_DEBUG_PATHFINDING)
			m_pEnemy[c]->RenderPath(Color::Yellow());
	}

	//Track bomberman
	m_pCamera->Track(m_pBomber.get(), m_pLevelData->GetBox());
	
	delete info;

	return true;
}
bool Level::InitializeGrid(LData* data)
{
	bool result;

	if(!m_pLevelData)
	{
		//Create and Initialize the Level 
		m_pLevelData = shared_ptr<LevelData>(new LevelData);
		result = m_pLevelData->Initialize(m_pD3D->GetDevice(), data);
		if(!result)
			return false;
	
		//find all the open nodes in the level and store them in the open vector
		for(int c = 0; c < m_pLevelData->GetGridX(); c++)
		{
			for(int v = 0; v < m_pLevelData->GetGridY(); v++)
			{
				if(m_pLevelData->GetNode(c, v)->GetNodeState())
					m_pOpen.push_back(m_pLevelData->GetNode(c, v));
			}
		}

		//Set the power up callback
		m_pLevelData->SetPowerUpCallback(std::bind(&Level::DropPowerUp, this, _1));
	}

	return true;
}

bool Level::InitializeBomberman(XMFLOAT2 size, XMFLOAT2 index)
{
	bool result;

	if(!m_pBomber)
	{
		//Create and Initialize Bomberman
		m_pBomber = shared_ptr<Bomberman>(new Bomberman);
		result = m_pBomber->Initialize(m_pD3D->GetDevice(), size, index, m_pLevelData);
		if(!result)
			return false;

		//Set the Camera Callback Method
		m_pBomber->SetCameraCallback(std::bind(&Level::CameraCallback, this));
	}

	return true;
}

//Create and Initialize the HUD and Continue Menu
bool Level::InitializeHUD(bool cont)
{
	bool result;

	if(!m_pHUD)
	{
		m_pHUD = shared_ptr<HUD>(new HUD);
		if(!m_pHUD)
			return false;

		result = m_pHUD->Initialize(m_pD3D->GetDevice());
		if(!result)
			return false;
	}

	if(!m_pCont)
	{
		m_pCont = shared_ptr<Continue>(new Continue);
		result = m_pCont->Initialize(m_pD3D->GetDevice(), cont);
		if(!result)
			return false;
	}

	return true;
}

void Level::Update(float gameTime)
{
	switch(e_level)
	{
		case PLAYING:
			//Update the Camera
			m_pCamera->Update(gameTime);

			m_pHUD->Update(m_pCamera->GetPosition2D());
	
			//Level and Bomberman Update
			m_pLevelData->Update(gameTime);
			m_pBomber->Update(gameTime);
	
			//Enemy Update and Collision Detection
			for(int c = 0; c < m_pEnemy.size(); c++)
			{
				m_pEnemy[c]->Update(gameTime);
				if(m_pEnemy[c]->GetPath())//does the enemy need a new path?
				{
					//If the path is no longer valid give the enemy a new target
					if(!m_pEnemy[c]->GetValid())
					{
						int ran = rand() % m_pOpen.size();
						m_pEnemy[c]->SetTarget(m_pOpen[ran]);//Give the enemy a target from the open vector
					}

					//Generate a new path
					m_pLevelData->FindPath(m_pEnemy[c].get());
					if(_DEBUG_PATHFINDING)
						m_pEnemy[c]->RenderPath(Color::Yellow());
				}

				//Collision Check
				m_pBomber->CheckEnemyIntersects(m_pEnemy[c].get());
				if(m_pEnemy[c]->GetHit())
				{
					DropPowerUp(m_pEnemy[c]->GetPosition());
					m_pEnemy.erase(m_pEnemy.begin() + c);
					c--;//Reduce the counter so that the next item will properly get updated
				}
			}

			for(int c = 0; c < m_pPower.size(); c++)
			{
				if(m_pBomber->CheckIntersect(m_pPower[c]->GetBox()))
				{
					m_pPower[c]->IncreasePower();
					m_pPower.erase(m_pPower.begin() + c);
					c--;//Reduce the counter so the loop doesn't miss an item
				}
			}

			//Level completed check
			LevelComplete();
		break;

		case CONTINUE:	m_pCont->Update(gameTime, m_pCamera->GetPosition2D());	break;
	}
}

bool Level::Render()
{
	BaseLevel::BeginScene();
	
	bool result;
	XMFLOAT4X4 world, view, ortho;

	//Get the world, view, and projection Matrixes form the camera amd D3D objects
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	switch(e_level)
	{
		case PLAYING:
			result = m_pLevelData->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
			if(!result)
				return false;

			result = m_pBomber->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
			if(!result)
				return false;

			for(int c = 0; c < m_pEnemy.size(); c++)
			{
				result = m_pEnemy[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
				if(!result)
					return false;
			}

			for(int c = 0; c < m_pPower.size(); c++)
			{
				result = m_pPower[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
				if(!result)
					return false;
			}

			result = m_pHUD->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
			if(!result)
				return false;
		break;
		case CONTINUE:
			result = m_pCont->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
			if(!result)
				return false;
		break;
	}

	BaseLevel::EndScene();

	return true;
}

void Level::Shutdown()
{
	//Release Bomberman
	if(m_pBomber)
	{
		m_pBomber.reset();
		m_pBomber = 0;
	}

	//Relase the Enemies
	for(int c = 0; c < m_pEnemy.size(); c++)
	{
		m_pEnemy[c].reset();
		m_pEnemy[c] = 0;
	}
	m_pEnemy.clear();

	//Release the Waypoints
	for(int c = 0; c < m_pWaypoints.size(); c++)
	{
		m_pWaypoints[c].reset();
		m_pWaypoints[c] = 0;
	}
	m_pWaypoints.clear();

	//Relase the Level Data, Tile Grid and Pathfinding
	if(m_pLevelData)
	{
		m_pLevelData.reset();
		m_pLevelData = 0;
	}

	//Release the HUD
	if(m_pHUD)
	{
		m_pHUD.reset();
		m_pHUD = 0;
	}

	//Relase the Continue screen
	if(m_pCont)
	{
		m_pCont.reset();
		m_pCont = 0;
	}

	//Release reference to the open nodes
	for(int c = 0; c < m_pOpen.size(); c++)
		m_pOpen[c] = 0;
	m_pOpen.clear();

	//Release the Powers
	for(int c = 0; c < m_pPower.size(); c++)
	{
		m_pPower[c].reset();
		m_pPower[c] = 0;
	}
	m_pPower.clear();
}

void Level::InitializeWaypoints(LevelInfo* info)
{
	int eCount = info->GetEnemyCount();

	for(int c = 0; c < eCount; c++)
	{
		//Get the Waypoint Count for this enemy
		int wpCount = info->GetEnemyData(c)->GetCount();

		//Create a new Waypoint for the Enmey 
		m_pWaypoints.push_back(shared_ptr<Waypoint>(new Waypoint));
		for(int v = 0; v < wpCount; v++)
		{
			//Get the Waypoint Position
			XMFLOAT2 pos = info->GetEnemyData(c)->GetWaypoint(v);
			//Set Waypoint
			m_pWaypoints[c]->SetWaypoint(m_pLevelData->GetNode(pos.x, pos.y));
		}
	}
}

void Level::SetEnemy(LevelInfo* info)
{
	//Get the enemy count
	int eCount = info->GetEnemyCount();

	for(int c = 0; c < eCount; c++)
	{
		//Get the position of the enemy
		XMFLOAT2 pos = info->GetEnemyData(c)->GetPosition();

		//Call the correct Add Enemy Method based on the enemy type
		switch(info->GetEnemyData(c)->GetType())
		{
		case 'H': AddEnemy(new Heli, XMFLOAT2(40,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		case 'Y': AddEnemy(new Yellow, XMFLOAT2(60,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		case 'G': AddEnemy(new MadGuy, XMFLOAT2(40,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		case 'B': AddEnemy(new MadBomb, XMFLOAT2(60,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		case 'R': AddEnemy(new Rabbit, XMFLOAT2(40,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		case 'L': AddEnemy(new BlueEye, XMFLOAT2(60,60), m_pLevelData->GetNode(pos.x, pos.y), m_pWaypoints[c].get()); break;
		}
	}
}

//Level Complete Check
void Level::LevelComplete()
{
	//All enemies defeated and Player is on the exit point
	if(m_pEnemy.size() <= 0 && XMVector2Equal(XMLoadFloat2(&m_pBomber->GetIndex()), XMLoadFloat2(&m_pLevelData->GetExit())))
		e_level = CONTINUE;

	//Bomberman Dead
	if(Stats::BomberDead())
		e_level = CONTINUE;
	
	//DEBUG
	if(Input::GetCurrentKeys() & KEY_P)
		e_level = CONTINUE;
}
//Reposition the Camera
void Level::CameraCallback()
{
	m_pCamera->Retarget();
}

void Level::DropPowerUp(XMFLOAT2 pos)
{
	//Generate a random number
	int ran = rand() & 100 + 1;

	//If the Random number is within the drop chance create and initialize a powerup
	if(ran <= m_dropChance)
	{
		m_pPower.push_back(shared_ptr<PowerUp>(new PowerUp));
		m_pPower[m_pPower.size() - 1]->Initialize(m_pD3D->GetDevice(), 50, 50, pos);
	}
}

void Level::ResetStats()
{
	Stats::Reset();
}

//Get the Current Level State
bool Level::LevelStatus()
{
	if(e_level == PLAYING)
		return false;
	else if(m_pCont->GetState() == NONE_INDEX)
		return false;
	else
		return true;
}

//Get the Continue state
int Level::GetContinueState()
{
	return m_pCont->GetState();
}