//----------------------------------------------------------------
//Waypoint.cpp
//----------------------------------------------------------------
//Define the Waypoints so that they can be stored and used by Seeker type objects

#include "Waypoint.h"

Waypoint::Waypoint()
{

}

Waypoint::Waypoint(const Waypoint& src)
{

}

Waypoint::~Waypoint()
{
	Shutdown();
}

void Waypoint::SetWaypoints(int count, ...)
{
	va_list argp;

	va_start(argp, count);
	
	for(int c = 0; c < count; c++)
		m_rNodes.push_back(va_arg(argp, Node*));

	va_end(argp);
}
void Waypoint::SetWaypoint(Node* node)
{
	m_rNodes.push_back(node);
}

Node* Waypoint::GetTarget(int index)
{
	return m_rNodes[index];
}
int Waypoint::Length()
{
	return m_rNodes.size();
}

void Waypoint::Shutdown()
{
	m_rNodes.clear();
}