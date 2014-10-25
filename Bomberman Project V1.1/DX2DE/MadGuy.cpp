//----------------------------------------------------
//MadGuy.cpp
//----------------------------------------------------
//Define the MadGuy class

#include "MadGuy.h"

MadGuy::MadGuy()
{
	m_speed = 0.15f;
	m_boundsMod = 0.85f;
}

MadGuy::MadGuy(const MadGuy& src)
{

}

MadGuy::~MadGuy()
{

}

bool MadGuy::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::MadGuy());
	if(!result)
		return false;

	return true;
}