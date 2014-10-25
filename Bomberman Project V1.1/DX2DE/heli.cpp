//----------------------------------------------------
//heli.cpp
//----------------------------------------------------
//Define the Heli class

#include "heli.h"

Heli::Heli()
{
	m_speed = 0.15f;
	m_boundsMod = 0.85f;
}

Heli::Heli(const Heli& src)
{

}

Heli::~Heli()
{

}

bool Heli::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::Heli());
	if(!result)
		return false;

	return true;
}