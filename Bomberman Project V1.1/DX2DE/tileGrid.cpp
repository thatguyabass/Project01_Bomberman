//------------------------------------------------
//tileGrid.cpp
//------------------------------------------------
//

#include "tileGrid.h"

TileGrid::TileGrid()
{

}

TileGrid::TileGrid(const TileGrid& src)
{

}

TileGrid::~TileGrid()
{
	Shutdown();
}

bool TileGrid::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, int width, int height, float startX, float startY, LData* level)
{
	bool result;

	m_X = level->GetX();
	m_Y = level->GetY();

	m_ppTiles = new Tile*[m_Y];
	for(int c = 0; c < m_Y; c++)
	{
		m_ppTiles[c] = new Tile[m_X];
		for(int v = 0; v < m_X; v++)
			m_ppTiles[c][v].Initialize(device, screenWidth, screenHeight, width, height, level->GetChar(c, v), XMFLOAT2(startX + (width * v), startY - (height * c)), XMFLOAT2(v, c), m_spawnIndex, m_exitIndex); 
	}

	return true;
}

void TileGrid::Update(float gameTime)
{

}

bool TileGrid::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			result = m_ppTiles[c][v].Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	return true;
}

void TileGrid::Shutdown()
{
	//Release Tiles
	if(m_ppTiles)
	{
		for(int c = 0; c < m_Y; c++)
			delete [] m_ppTiles[c];
		delete [] m_ppTiles;
	}
}

Tile* TileGrid::GetTile(int x, int y)
{
	return &m_ppTiles[y][x];
}

XMFLOAT2 TileGrid::GetTileDimensions()
{
	return GetTile(0,0)->GetDimensions();
}
XMFLOAT2 TileGrid::GetDimensions()
{
	XMFLOAT2 d = GetTileDimensions();
	return XMFLOAT2(d.x * m_X, d.y * m_Y);
}
XMFLOAT2 TileGrid::GetExitIndex()
{
	return XMFLOAT2(m_exitIndex.x, m_exitIndex.y);
}
XMFLOAT2 TileGrid::GetSpawnIndex()
{
	return XMFLOAT2(m_spawnIndex.x, m_spawnIndex.y);
}