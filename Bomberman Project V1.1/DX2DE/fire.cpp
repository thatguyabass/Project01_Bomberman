//------------------------------------------------------------------
//fire.cpp
//------------------------------------------------------------------
//Define the Fire class

#include "fire.h"

Fire::Fire()
{

}

Fire::Fire(const Fire& src)
{

}

Fire::~Fire()
{
	Shutdown();
}

bool Fire::Initialize(ID3D11Device* device, XMFLOAT2 imageSize, XMFLOAT2 position)
{
	bool result;

	result = Base2D::Initialize(device, imageSize.x, imageSize.y, position, 0);
	if(!result)
		return false;

	result = LoadTexture(device, StringManager::Fire());
	if(!result)
		return false;

	return true;
}

void Fire::Update(float gameTime)
{

}

bool Fire::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;
	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void Fire::Shutdown()
{

}