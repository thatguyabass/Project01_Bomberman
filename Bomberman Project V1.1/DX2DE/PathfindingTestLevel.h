//------------------------------------------------------------------------------------------------
//PathfindingTestLevel.h
//------------------------------------------------------------------------------------------------
//This level will be where all pathfinding is tested and finalized


#ifndef _PATHFINDING_TEST_LEVEL_H_
#define _PATHFINDING_TEST_LEVEL_H_

#include "baselevel.h"
#include "LevelData.h"
#include "HUD.h"

#include "Array.h"

#define PATH m_pData->GetPath()

class PathfindingTestLevel : public BaseLevel
{
public:
	PathfindingTestLevel();
	PathfindingTestLevel(const PathfindingTestLevel&);
	~PathfindingTestLevel();

	bool Initialize(shared_ptr<D3D>, shared_ptr<Input>, int, int, HWND, WCHAR* = 0);
	void Update(float);
	bool Render();

private:
	//Method
	void Shutdown();
	void InitializeWaypoints();

	//Variables
	LevelData* m_pData;
	//PathGrid* m_pPathGrid;
	HUD* m_pHUD;

	float time;
	bool M;

	int m_waypointCount;
	Array<Waypoint*, 10> m_pWaypoint;

	int m_seekerCount;
	Array<Seeker*, 10> m_pSeeker;
	bool test;

};

#endif