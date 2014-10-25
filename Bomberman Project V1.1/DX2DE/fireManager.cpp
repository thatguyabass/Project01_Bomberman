//---------------------------------------------
//fireManager.cpp
//---------------------------------------------
//Class Handles
//Initialization
//Update
//Render
//Collision detection

#include "fireManager.h"

FireManager::FireManager()
{
	m_fireCount = 1;
	m_spread = 0;
	m_pLevelData = 0;
	m_blocked = 0;
	m_timer = 0;

	m_boundsMod = 0.9f;
	m_pBoxLR = 0;
	m_pBoxUD = 0;
}

FireManager::FireManager(const  FireManager& src)
{

}

FireManager::~FireManager()
{
	Shutdown();
}

bool FireManager::Initialize(ID3D11Device* device, XMFLOAT2 index, int fireSpread, shared_ptr<LevelData> data)
{
	bool result;

	m_index = index;
	m_pLevelData = data;
	m_spread = fireSpread;
	m_fireSize = m_pLevelData->GetNodeDimensions();

	//Create the Initial Fire
	m_pFire.push_back(shared_ptr<Fire>(new Fire));
	result = m_pFire[0]->Initialize(device, m_fireSize, m_pLevelData->GetPosition(m_index.x, m_index.y));
	if(!result)
		return false;

	//Left
	result = SetupFire(device, -1, true);
	if(!result)
		return false;

	//Right
	result = SetupFire(device, 1, true);
	if(!result)
		return false;

	//Top
	result = SetupFire(device, -1, false);
	if(!result)
		return false;

	//Bottom
	result = SetupFire(device, 1, false);
	if(!result)
		return false;

	SetupBoundingBoxes();

	return true;
}

bool FireManager::Update(float gameTime)
{
	m_timer += gameTime;

	if(m_timer > EXPLOSION_TIME)
		return true;

	return false;
}

bool FireManager::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pFire.size(); c++)
	{
		result = m_pFire[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void FireManager::Shutdown()
{
	for(int c = 0; c < m_pFire.size(); c++)
	{
		m_pFire[c].reset();
		m_pFire[c] = 0;
	}
	m_pFire.clear();

	if(m_pBoxLR)
	{
		m_pBoxLR.reset();
		m_pBoxLR = 0;
	}

	if(m_pBoxUD)
	{
		m_pBoxUD.reset();
		m_pBoxUD = 0;
	}
}

//Create a fire object for the length required or until hitting a blocker
bool FireManager::SetupFire(ID3D11Device* device, int indexMod, bool leftRight)
{
	XMFLOAT2 posTracker = m_index;
	bool result;
	//int count = 0;

	for(int c = 0; c < m_spread; c++)
	{
		//Direction of the fire
		if(leftRight)
			posTracker.x += indexMod;//Index Mod represents how much this will be increasing by (Normal case 1 OR -1)
		else
			posTracker.y += indexMod;

		//Check to see if the next position is open
		result = CheckGrid(posTracker);
		if(result)
		{
			m_pFire.push_back(shared_ptr<Fire>(new Fire));
			result = m_pFire[m_fireCount]->Initialize(device, m_fireSize, m_pLevelData->GetPosition(posTracker.x, posTracker.y));
			if(!result)
				return false;
			m_fireCount++;
		}	
		else
		{
			//Destroy the tile that blocked the fire if it can be destroyed
			m_pLevelData->ModifyLevel(posTracker.x, posTracker.y);
			break;
		}
	}

	return true;
}

void FireManager::SetupBoundingBoxes()
{
	float xAver = 0;
	float yAver = 0;

	int xCount = 1;
	int yCount = 1;	

	//Get the base Position, It will always be the center index
	XMFLOAT2 base = m_pFire[0]->GetPosition();
	
	float posX = base.x;
	float posY = base.y;


	//Get count and the position of the fire objects in each axis (X and Y)
	for(int c = 1; c < m_pFire.size(); c++)
	{
		if(m_pFire[c]->GetPosition().x != base.x)
		{
			xCount++;
			posX += m_pFire[c]->GetPosition().x;
		}
		else if(m_pFire[c]->GetPosition().y != base.y)
		{
			yCount++;
			posY += m_pFire[c]->GetPosition().y;
		}
	}

	//Calcualte the average from each position and use this as the center of each bounding box 
	posX /= xCount;
	posY /= yCount;

	//Create the Bounding Boxes
	m_pBoxLR = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBoxUD = shared_ptr<BoundingBox>(new BoundingBox);

	//Set the bounding Boxes
	m_pBoxLR->Set((m_fireSize.x * xCount) * m_boundsMod, m_fireSize.y * m_boundsMod, XMFLOAT2(posX, base.y));
	m_pBoxUD->Set(m_fireSize.x * m_boundsMod, (m_fireSize.y * yCount) * m_boundsMod, XMFLOAT2(base.x, posY));
}

//Check to see if the next index is free
bool FireManager::CheckGrid(XMFLOAT2 index)
{
	if(m_pLevelData->GetNode(index.x, index.y)->GetTileState() != Node::SPACE)
		return false;
	else
		return true;
}

bool FireManager::CheckIntersect(BoundingBox* box2)
{
	if(m_pBoxLR && m_pBoxUD && box2)
		if(m_pBoxLR->BoundingBoxIntersect(m_pBoxLR.get(), box2))
			return true;//Intersection with the Left Right BoundingBox
		else if(m_pBoxUD->BoundingBoxIntersect(m_pBoxUD.get(), box2))
			return true;//Intersection with the Up Down BoundingBox
		else 
			return false;//No intersection
	else
		return false;//A Bounding Box was null
}