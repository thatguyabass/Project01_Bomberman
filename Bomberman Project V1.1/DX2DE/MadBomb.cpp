//----------------------------------------------------
//MadBomb.cpp
//----------------------------------------------------
//Define the MadBomb class

#include "MadBomb.h"

MadBomb::MadBomb()
{
	m_speed = 0.15f;
	m_boundsMod = 0.85f;
}

MadBomb::MadBomb(const MadBomb& src)
{

}

MadBomb::~MadBomb()
{

}

bool MadBomb::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::MadBomb());
	if(!result)
		return false;

	return true;
}