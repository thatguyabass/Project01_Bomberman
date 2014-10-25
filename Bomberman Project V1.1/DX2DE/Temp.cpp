//--------------------------------------------------
//Temp.cpp
//--------------------------------------------------
//Define Temp class

#include "Temp.h"

Temp::Temp()
{

}

Temp::Temp(const Temp& src)
{

}

Temp::~Temp()
{
	Shutdown();
}

bool Temp::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, int imageWidth, int imageHeight, XMFLOAT2 position, float rotation)
{
	bool result;

	result = Base2D::Initialize(device, screenWidth, screenHeight, imageWidth, imageHeight, position, rotation);
	if(!result)
		return false;

	//Load the Texture
	result = LoadTexture(device, L"../DX2DE/Textures/levelFloor.jpg");
	if(!result)
		return false;

	m_pBox = new BoundingBox;
	if(!m_pBox)
		return false;
	m_pBox->Set(imageWidth, imageHeight, position);

	m_xV = 0;
	m_yV = 0;
	
	return true;
}

void Temp::Update(float gameTime)
{
	POSITION.x += m_xV;
	POSITION.y += m_yV;
}

void Temp::Shutdown()
{

}

bool Temp::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, projection);
	if(!result)
		return false;

	return true;
}

void Temp::Move(float d, XMFLOAT2 dir)
{
	float S = sin(ROTATION);
	float C = cos(ROTATION);

	float x = (dir.x * C) + (dir.y * S);
	float y = (dir.x * S) - (dir.y * C);

	m_xV = (d * x);
	m_yV = (d * y);
}

void Temp::Rotate(float d)
{
	float t = XMConvertToRadians(d);
	ROTATION += t;

	if(ROTATION > 360)
		ROTATION -= 360;
	else if(ROTATION < 0)
		ROTATION += 360;
}

void Temp::CollisionWithWall(int side)
{
	switch(side)
	{
	case 0://bottom
		if(m_yV < 0)
			m_yV *= -1;
		break;
	case 1://top
		if(m_yV > 0)
			m_yV *= -1;
		break;
	case 2://left
		if(m_xV < 0)
			m_xV *= -1;
		break;
	case 3://right
		if(m_xV > 0)
			m_xV *= -1;
		break;
	default:
		break;
	}
}