//-----------------------------------------------------------------------------------
//Waypoint.h
//-----------------------------------------------------------------------------------
//This class will contain a number of nodes that will act as targets for Seeker type objects

#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include<cstdarg>

#include "Node.h"

//-------------------------------------------------------------------

class Waypoint
{
public:
	Waypoint();
	Waypoint(const Waypoint&);
	~Waypoint();

	void SetWaypoints(int, ...);
	void SetWaypoint(Node*);

	Node* GetTarget(int);//Get a Node from the Waypoint
	int Length();//Get the Waypoint Length

private:
	//Methods
	void Shutdown();

	//Variables
	vector<Node*> m_rNodes;

};

#endif//_WAYPOINT_H_