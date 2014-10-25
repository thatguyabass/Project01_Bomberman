//----------------------------------------------------
//bomb.cpp
//----------------------------------------------------
//Define the Bomb class

#include "bomb.h"

Bomb::Bomb()
{
	m_timer = 0;
	m_finished = true;
	m_pFire = 0;
	m_eBombState = PLANTED;
	m_remove = false;

	m_boundsMod = 0.7f;
}

Bomb::~Bomb()
{
	Shutdown();
}

bool Bomb::Initialize(ID3D11Device* device, XMFLOAT2 imageSize, shared_ptr<LevelData> data, XMFLOAT2 index, int fireSpread)
{
	bool result;

	m_pLevelData = data;
	m_index = index;
	m_pDevice = device;
	m_fireSpread = fireSpread;

	//Initialize the base class
	result = Base2D::Initialize(device, imageSize.x, imageSize.y, m_pLevelData->GetPosition(m_index.x, m_index.y), 0);
	if(!result)
		return false;

	//Load the Texture
	result = LoadTexture(device, StringManager::Bomb());
	if(!result)
		return false;

	//Set the bounding box for the bomb
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	XMFLOAT2 d = m_pLevelData->GetNodeDimensions();
	m_pBox->Set(d.x * m_boundsMod, d.y * m_boundsMod, POSITION);

	return true;
}

void Bomb::Update(float gameTime)
{
	m_timer += gameTime;
	
	switch(m_eBombState)
	{
	case PLANTED:
		{
			if(m_timer > DETONATION_TIME)
			{
				//BLOW UP
				m_timer = 0;
				m_remove = true;
			}
		}
		break;

	case INITIALIZE_BOOM:
		m_pFire = shared_ptr<FireManager>(new FireManager);
			m_pFire->Initialize(m_pDevice, m_index, m_fireSpread, m_pLevelData);
		
		m_eBombState = BOOM;
		break;

	case BOOM:
		{
			if(m_pFire->Update(gameTime))
				m_finished = false;
		}
		break;
	}
}

bool Bomb::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;
	
	switch(m_eBombState)
	{
	case PLANTED:
		{
			//Render the Bomb
			result = Base2D::Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
		break;

	case BOOM:
		{
			//Render the Fire
			result = m_pFire->Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
		break;
	}

	return true;
}

void Bomb::Shutdown()
{
	//Release the fire manager
	if(m_pFire)
	{
		m_pFire.reset();
		m_pFire = 0;
	}

	//Remove this objects reference to the device
	if(m_pDevice)
		m_pDevice = 0;

	//Remvoe this objects reference to the tile grid
	if(m_pLevelData)
	{
		m_pLevelData.reset();
		m_pLevelData = 0;
	}
}

bool Bomb::IsPlanted()
{
	return (m_eBombState == PLANTED);
}
bool Bomb::IsBoom()
{
	return (m_eBombState == BOOM);
}

//Entire process complete?
bool Bomb::IsFinished()
{
	return m_finished;
}
XMFLOAT2 Bomb::GetIndex()
{
	return m_index;
}

//Check if the bomb is ready to be removed from the total planted count
bool Bomb::RemoveFromCount()
{
	return (m_remove == true && m_eBombState == PLANTED);
}
void Bomb::Boom()
{
	m_eBombState = INITIALIZE_BOOM;
}

//Check if the fire bounding box has intersected another
bool Bomb::CheckFireBounds(BoundingBox* box2)
{
	if(m_eBombState == BOOM)
		return m_pFire->CheckIntersect(box2);
	else 
		return false;
}