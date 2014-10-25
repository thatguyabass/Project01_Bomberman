//----------------------------------------------------
//tile.h
//----------------------------------------------------
//Handles the behaviour of this tile on the grid

#ifndef _TILE_H_
#define _TILE_H_

#include "base2D.h"
#include "tileCharacter.h"

enum TileState
{
	SPACE = 0,
	NO_SPACE = 1,
	DESTRUCTIBLE = 2,
	EXIT = 3,
	SPAWN = 4
};

class Tile : public Base2D
{
public:
	Tile();
	Tile(const Tile&);
	~Tile();

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, int imageWidth, int imageHeight, char, XMFLOAT2 position, XMFLOAT2 index, XMFLOAT2& start, XMFLOAT2& exit);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& orthoView, XMFLOAT4X4& ortho);

	TileState GetState();
	XMFLOAT2 GetDimensions();

	bool LoadTexture(ID3D11Device*, Texture*&, WCHAR*);
	void TileDestroyed();

protected:
	//Methods
	void Shutdown();

	//Variables
	TileState m_eCurrentState;
	XMFLOAT2 m_index;

	int m_width;
	int m_height;

	shared_ptr<Texture> m_pAlt;

};

#endif//_TILE_H_