//--------------------------------------------------------
//TileType.h
//--------------------------------------------------------
//Tile Types hold the information used when creating tiles in the editor
//This way complicated recreations can be simpler

#ifndef _TILE_TYPE_H_
#define _TILE_TYPE_H_

#include "textureclass.h"

#include<memory>
using namespace std;

class TileType
{
public:
	TileType();
	~TileType();

	bool Initialize(ID3D11Device*, wstring texture, int id, bool spawn, bool exit);//Use when creating a level tile
	bool Initialize(ID3D11Device*, wstring, int id, int type);//Use when creating a enemy tile

	//Access Methods
	shared_ptr<Texture> GetTexture();
	int GetID();
	int GetType();
	bool GetSpawn();
	bool GetExit();

private:
	//Variables
	int m_id;//Tile State
	int m_type;//Enemy Type
	shared_ptr<Texture> m_pTexture;
	bool m_spawn;
	bool m_exit;

};

#endif//_TILE_TYPE_H_