//----------------------------------------------------
//Rabbit.cpp
//----------------------------------------------------
//Define the Rabbit class

#include "Rabbit.h"

Rabbit::Rabbit()
{
	m_speed = 0.15f;
	m_boundsMod = 0.85f;
}

Rabbit::Rabbit(const Rabbit& src)
{

}

Rabbit::~Rabbit()
{

}

bool Rabbit::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::Rabbit());
	if(!result)
		return false;

	return true;
}