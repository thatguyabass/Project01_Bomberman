//-----------------------------------
//basecamera.cpp
//-----------------------------------
//Define the Base camera Methods

#include "basecamera.h"

BaseCamera::BaseCamera()
{
	m_position = XMFLOAT3(0,0,0);
	m_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
}
BaseCamera::~BaseCamera()
{

}

void BaseCamera::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
}
void BaseCamera::SetPosition(float x, float y)
{
	m_position = XMFLOAT3(x, y, m_position.z);
}
void BaseCamera::SetPosition(XMFLOAT3 pos)
{
	m_position = pos;
}
XMFLOAT3 BaseCamera::GetPosition()
{
	return m_position;
}
XMFLOAT2 BaseCamera::GetPosition2D()
{
	return XMFLOAT2(m_position.x, m_position.y);
}

void BaseCamera::GetViewMatrix(XMFLOAT4X4& matrix)
{
	matrix =  m_viewMatrix;
}
void BaseCamera::Move(float d, XMFLOAT3 dir)
{
	m_position.x = (dir.x * d);
	m_position.y = (dir.y * d);
	m_position.z = (dir.y * d);
}
void BaseCamera::MoveX(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_right);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}
void BaseCamera::MoveY(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_up);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}
void BaseCamera::MoveZ(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_forward);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}

void BaseCamera::Render()
{

}