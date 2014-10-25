//----------------------------------------------------------------
//EditorWaypoint.cpp
//----------------------------------------------------------------
//Define the Waypoints so that they can be stored and used by Seeker type objects

#include "EditorWaypoint.h"

EdWaypoint::EdWaypoint()
{

}
EdWaypoint::~EdWaypoint()
{
	Shutdown();
}

//Set the waypoints with the provided positions
void EdWaypoint::SetWaypoints(int count, ...)
{
	va_list argp;

	va_start(argp, count);
	
	for(int c = 0; c < count; c++)
		m_waypoints.push_back(va_arg(argp, XMFLOAT2));

	va_end(argp);
}
//Add a position to the waypoint
void EdWaypoint::SetWaypoint(XMFLOAT2 node)
{
	m_waypoints.push_back(node);
}

//Remove a waypoint from the vector
void EdWaypoint::RemoveWaypoint(XMFLOAT2 point)
{
	int index;
	for(int c = 0; c < m_waypoints.size(); c++)
	{
		//Find the waypoint to be removed from the list of points
		if(XMVector2Equal(XMLoadFloat2(&point), XMLoadFloat2(&m_waypoints[c])))
		{
			m_waypoints.erase(m_waypoints.begin() + c);
			break;
		}
	}
}

//Reset the waypoitns
void EdWaypoint::Reset()
{
	Shutdown();
}

//Return all the waypoints
vector<XMFLOAT2>& EdWaypoint::GetWaypoints()
{
	return m_waypoints;
}

//Return the waypoint at position
XMFLOAT2 EdWaypoint::GetWaypoint(int index)
{
	return m_waypoints[index];
}

//Retrun the Length of the waypoint
int EdWaypoint::Length()
{
	return m_waypoints.size();
}

void EdWaypoint::Shutdown()
{
	m_waypoints.clear();
}