//----------------------------------------------------------------------
//tileCharacter.h
//----------------------------------------------------------------------
//Define the Characteristics of the tile

#ifndef _TILE_CHARACTER_H_
#define _TILE_CHARACTER_H_

#include<Windows.h>

#include "stringManager.h"

#define INSTANCE StringManager::INSTANCE

class TileCharacter
{
public:
	TileCharacter();
	TileCharacter(const TileCharacter&);
	~TileCharacter();

	void SetChar(int);
	wstring Texture();
	bool CheckAlt();
	wstring Alt();
	bool BoundingBox();	
	bool Exit();
	bool Spawn();

private:
	//Methods
	void Space();
	void NoSpace();
	void Removable();
	void SetExit();
	void SetSpawn();

	//Variables
	wstring m_pTexture;
	bool m_alt;
	wstring m_pALT;
	bool m_boundingBox;
	bool m_exit;
	bool m_spawn;

};

#endif//_TILE_CHARACTER_H_