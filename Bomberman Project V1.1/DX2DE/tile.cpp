//-------------------------------------------------
//tile.cpp
//-------------------------------------------------
//Define the Tile class 

#include "tile.h"

Tile::Tile()
{
	m_pAlt = 0;
}

Tile::Tile(const Tile& src)
{

}

Tile::~Tile()
{
	Shutdown();
}

bool Tile::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, int imageWidth, int imageHeight, char tileType, XMFLOAT2 position, XMFLOAT2 index, XMFLOAT2& spawn, XMFLOAT2& exit)
{
	bool result;
	TileCharacter tileChar;

	//Establish this tiles position on the grid
	m_index = index;

	//Initialize the image
	result = Base2D::Initialize(device, screenWidth, screenHeight, imageWidth, imageHeight, position, 0);
	if(!result)
		return false;

	switch(tileType)
	{
	case 'o': m_eCurrentState = SPACE;			break;
	case 'x': m_eCurrentState = NO_SPACE;		break;
	case 'y': m_eCurrentState = DESTRUCTIBLE;	break;
	case 'e': m_eCurrentState = EXIT;			break;
	case 's': m_eCurrentState = SPAWN;			break;
	default:  m_eCurrentState = SPACE;			break;
	}
	tileChar.SetChar(m_eCurrentState);

	//Load Base Texture
	result = Base2D::LoadTexture(device, tileChar.Texture());
	if(!result)
		return false;

	if(tileChar.CheckAlt())
	{
		Texture* temp;
		result = LoadTexture(device, temp, tileChar.Alt());
		if(!result)
			return false;

		m_pAlt = shared_ptr<Texture>(temp);
		temp = 0;
	}

	if(tileChar.BoundingBox())
	{
		m_pBox = new BoundingBox;
		m_pBox->Set(imageWidth, imageHeight, position);
	}

	if(tileChar.Exit())
	{
		m_eCurrentState = DESTRUCTIBLE;
		exit = m_index;
	}

	if(tileChar.Spawn())
	{
		m_eCurrentState = SPACE;
		spawn = m_index;
	}

	return true;
}

void Tile::Update(float gameTime)
{

}

bool Tile::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, projection);
	if(!result)
		return false;

	return true;
}

void Tile::Shutdown()
{
	if(m_pAlt)
	{
		m_pAlt.reset();
		m_pAlt = 0;
	}
}

TileState Tile::GetState()
{
	return m_eCurrentState;
}
XMFLOAT2 Tile::GetDimensions()
{
	return XMFLOAT2(m_imageWidth, m_imageHeight);
}

bool Tile::LoadTexture(ID3D11Device* device, Texture*& texturePtr, WCHAR* texture)
{
	bool result;

	texturePtr = new Texture;
	result = texturePtr->Initialize(device, texture);
	if(!result)
		return false;

	return true;
}

void Tile::TileDestroyed()
{
	if(m_eCurrentState == DESTRUCTIBLE)
	{
		ReleaseTexture();
		delete m_pBox;
		m_pBox = 0;

		m_eCurrentState = SPACE;
		m_pTexture = m_pAlt;
	}
}