//----------------------------------------------------
//BlueEye.cpp
//----------------------------------------------------
//Define the BlueEye class

#include "BlueEye.h"

BlueEye::BlueEye()
{
	m_speed = 0.15f;
	m_boundsMod = 0.85f;
}

BlueEye::BlueEye(const BlueEye& src)
{

}

BlueEye::~BlueEye()
{

}

bool BlueEye::Initialize(ID3D11Device* device, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Enemy::Initialize(device, size, start, waypoint);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::BlueEye());
	if(!result)
		return false;

	return true;
}