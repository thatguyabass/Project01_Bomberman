//-------------------------------------------------------------------
//Powers.cpp
//-------------------------------------------------------------------
//Define the Powers Class

#include "Powers.h"

PowerUp::PowerUp()
{
	e_powerType = BOMB;
	m_powerCount = 4;
}

PowerUp::~PowerUp()
{
	Shutdown();
}

bool PowerUp::Initialize(ID3D11Device* device, int width, int height, XMFLOAT2 pos)
{
	bool result;

	result = Base2D::Initialize(device, width, height, pos, 0);
	if(!result)
		return false;
	wstring texture;

	//Random between the power ups
	int ran = rand() % m_powerCount;
	switch(ran)
	{
	case 0: 
		texture = StringManager::PowerUpCount(); 
		e_powerType = BOMB;
		break;
	case 1: 
		texture = StringManager::PowerUpSpread(); 
		e_powerType = SPREAD;
		break;
	case 2: 
		texture = StringManager::PowerUpExtraLife(); 
		e_powerType = LIFE;
		break;
	case 3:
		texture = StringManager::PowerUpSpeed();
		e_powerType = SPEED;
		break;
	}

	result = Base2D::LoadTexture(device, texture);
	if(!result)
		return false;

	SetBox();

	return true;
}

void PowerUp::Update(float gameTime)
{

}

bool PowerUp::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void PowerUp::Shutdown()
{

}

void PowerUp::IncreasePower()
{
	switch(e_powerType)
	{
	case BOMB: Stats::IncreaseBombCount(); break;
	case SPREAD: Stats::IncreaseBombSpread(); break;
	case LIFE:	Stats::IncreaseLives(); break;
	case SPEED: Stats::IncreaseSpeed(0.01f); break;
	}
}