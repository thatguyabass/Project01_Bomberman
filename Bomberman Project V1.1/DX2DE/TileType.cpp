//---------------------------------------------------------
//TileType.cpp
//---------------------------------------------------------
//

#include "TileType.h"

TileType::TileType()
{
	//Set the base values
	m_id = -1;
	m_spawn = false;
	m_exit = false;
	m_pTexture = 0;
	m_type = -1;
}

TileType::~TileType()
{
	//Release the Texture
	if(m_pTexture)
	{
		m_pTexture.reset();
		m_pTexture = 0;
	}
}

bool TileType::Initialize(ID3D11Device* device, wstring texture, int id, bool spawn, bool exit)
{
	//Set the values of the tile
	m_id = id;
	m_spawn = spawn;
	m_exit = exit;

	//Create and initialize the Texture
	m_pTexture = shared_ptr<Texture>(new Texture);
	bool result = m_pTexture->Initialize(device, texture);
	if(!result)
		return false;

	return true;
}

bool TileType::Initialize(ID3D11Device* device, wstring texture, int id, int type)
{
	bool result;
	//Set the enemy type
	m_type = type;
	
	//Initialize the Rest of the values
	result = Initialize(device, texture, id, false, false);
	if(!result)
		return false;

	return true;
}

int TileType::GetID()
{
	return m_id;
}
int TileType::GetType()
{
	return m_type;
}

shared_ptr<Texture> TileType::GetTexture()
{
	return m_pTexture;
}

bool TileType::GetSpawn()
{
	return m_spawn;
}
bool TileType::GetExit()
{
	return m_exit;
}