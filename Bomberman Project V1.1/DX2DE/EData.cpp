//-----------------------------------------------------
//EData.cpp
//-----------------------------------------------------
//Define the EData class

#include "EData.h"

EData::EData()
{

}

EData::~EData()
{
	Shutdown();
}

void EData::SetType(char type)
{
	m_type = type;
}
char EData::GetType()
{
	return m_type;
}

void EData::SetPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
}
XMFLOAT2 EData::GetPosition()
{
	return m_position;
}

void EData::SetCount(int count)
{
	m_count = count;
	Shutdown();
}
int EData::GetCount()
{
	return m_count;
}

void EData::SetWaypoint(int x, int y)
{
	m_pWaypoint.push_back(XMFLOAT2(x,y));
}
XMFLOAT2 EData::GetWaypoint(int index)
{
	return m_pWaypoint[index];
}

void EData::Shutdown()
{
	m_pWaypoint.clear();
}