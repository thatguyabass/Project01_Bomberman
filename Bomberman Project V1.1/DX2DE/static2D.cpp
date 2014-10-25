//----------------------------------------
//static2D.cpp
//----------------------------------------
//Define the Static 2D class

#include "static2D.h"

Static2D::Static2D()
{

}

Static2D::Static2D(const Static2D& src)
{

}

Static2D::~Static2D()
{

}

bool Static2D::Initialize(ID3D11Device* device, wstring texture, int imageWidth, int imageHeight, XMFLOAT2 position, float rotation)
{
	bool result;

	result = Base2D::Initialize(device, imageWidth, imageHeight, position, rotation);
	if(!result)
		return false;

	//Load the Texture
	result = LoadTexture(device, texture);
	if(!result)
		return false;
	
	return true;
}

void Static2D::Update(float gameTime)
{
	Base2D::Update(gameTime);
}