//---------------------------------------------
//tileGrid.h
//---------------------------------------------
//Initialize, Update, and draw the tile objects

#ifndef _TILE_GRID_H_
#define _TILE_GRID_H_

#include "tile.h"
#include "LData.h"

class TileGrid
{
public:
	TileGrid();
	TileGrid(const TileGrid&);
	~TileGrid();

	virtual bool Initialize(ID3D11Device*, int, int, int, int, float, float, LData*);
	void Update(float);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	Tile* GetTile(int, int);

	XMFLOAT2 GetTileDimensions();
	XMFLOAT2 GetSpawnIndex();
	XMFLOAT2 GetExitIndex();
	XMFLOAT2 GetDimensions();

protected:
	//Methods
	void Shutdown();

	//Variables
	Tile** m_ppTiles;

	XMFLOAT2 m_spawnIndex;
	XMFLOAT2 m_exitIndex;

	int m_X;
	int m_Y;

};

#endif//_TILE_GRID_H_