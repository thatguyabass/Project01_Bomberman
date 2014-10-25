//------------------------------------------------------------------------------
//EData.h
//------------------------------------------------------------------------------
//Object to store the Type and waypoints of enemies read from the level text file. 
//Used to create the enemies for the level

#ifndef _ENEMY_DATA_H_
#define _ENEMY_DATA_H_

#include<Windows.h>
#include<xnamath.h>

#include<vector>
using namespace std;

//----------------------------------------------------------------------------------

class EData
{
public:
	EData();
	~EData();

	//Type Access Methods
	void SetType(char);
	char GetType();

	//Starting Position(based on the level grid) Access Methods
	void SetPosition(int,int);
	XMFLOAT2 GetPosition();

	//Waypoint Count Access Methods
	void SetCount(int);
	int GetCount();
	
	//Waypoint Access methods
	void SetWaypoint(int x, int y);
	XMFLOAT2 GetWaypoint(int);

private:
	//Methods
	void Shutdown();

	//Variables
	char m_type;
	XMFLOAT2 m_position;
	int m_count;
	vector<XMFLOAT2> m_pWaypoint;

};

#endif//_ENEMY_DATA_H_