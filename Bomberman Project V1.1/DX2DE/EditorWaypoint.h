//-----------------------------------------------------------
//EditorWaypoint.h
//-----------------------------------------------------------
//Waypoints to be used for the level editor - More dynamic then the Pathfinding waypoints

#ifndef _EDITOR_WAYPOINT_H_
#define _EDITOR_WAYPOINT_H_

#include<Windows.h>
#include<xnamath.h>
#include<cstdarg>
#include<algorithm>

#include<vector>
using namespace std;

class EdWaypoint
{
public:
	EdWaypoint();
	~EdWaypoint();

	//Set the waypoints with provided XMFLOAT values
	void SetWaypoints(int, ...);
	void SetWaypoint(XMFLOAT2);//Add a position to the waypoint
	void RemoveWaypoint(XMFLOAT2);//Remove a position from the waypoint
	void Reset();//Reset the vector

	vector<XMFLOAT2>& GetWaypoints();//Return waypoint vector
	XMFLOAT2 GetWaypoint(int);//Return a waypoint using index
	int Length();//Get the vector length

private:
	//Methods
	void Shutdown();

	//Variables
	vector<XMFLOAT2> m_waypoints;

};

#endif//_EDITOR_WAYPOINT_H_