//--------------------------------------------------
//yellow.cpp
//--------------------------------------------------
//Define the Yellow Enemy class

#include "yellow.h"

Yellow::Yellow()
{
	m_speed = 0.1f;
}

Yellow::Yellow(const Yellow& src)
{

}

Yellow::~Yellow()
{

}

bool Yellow::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::Yellow());
	if(!result)
		return false;

	return true;
}