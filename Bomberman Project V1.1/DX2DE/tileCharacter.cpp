//----------------------------------------------------
//tileCharacter.cpp
//----------------------------------------------------
//Define the Tile Characteristics

#include "tileCharacter.h"

TileCharacter::TileCharacter()
{

}

TileCharacter::TileCharacter(const TileCharacter& src)
{

}

TileCharacter::~TileCharacter()
{

}


void TileCharacter::SetChar(int index)
{
	m_pTexture = L"";
	m_alt = false;
	m_pALT = L"";
	m_boundingBox = false;
	m_exit = false;
	m_spawn = false;

	switch(index)
	{
	case 0: Space();		break;
	case 1: NoSpace();		break;
	case 2: Removable();	break;
	case 3: SetExit();		break;
	case 4: SetSpawn();		break;
	default: Space();		break;
	}
}

void TileCharacter::Space()
{
	m_pTexture = StringManager::TileSpace();
}
void TileCharacter::NoSpace()
{
	m_pTexture = StringManager::TileNoSpace();
	m_boundingBox = true;
}
void TileCharacter::Removable()
{
	m_pTexture = StringManager::TileRemovable();
	m_alt = true;
	m_pALT = StringManager::TileSpace();
	m_boundingBox = true;
}
void TileCharacter::SetExit()
{
	m_pTexture = StringManager::TileRemovable();
	m_alt = true;
	m_pALT = StringManager::TileExit();
	m_boundingBox = true;
	m_exit = true;
}
void TileCharacter::SetSpawn()
{
	m_spawn = true;
	Space();
}
wstring TileCharacter::Texture()
{
	return m_pTexture;
}
bool TileCharacter::CheckAlt()
{
	return m_alt;
}
wstring TileCharacter::Alt()
{
	return m_pALT;
}
bool TileCharacter::BoundingBox()
{
	return m_boundingBox;
}
bool TileCharacter::Exit()
{
	return m_exit;
}
bool TileCharacter::Spawn()
{
	return m_spawn;
}