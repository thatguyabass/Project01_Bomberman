//----------------------------------------------------------------------
//LevelData.h
//----------------------------------------------------------------------
//Master Level class that will contain the Pathfinding and the Level Assets

#ifndef _LEVEL_DATA_H_
#define _LEVEL_DATA_H_

#include "LData.h"
#include "BaseGrid.h"
#include "Node.h"

//--------------------------------------------------------------------------------------------------------------

class LevelData : public BaseGrid<Node>
{
public:
	LevelData();
	LevelData(const LevelData&);
	~LevelData();

	//Create a level using the data stored in LData
	bool Initialize(ID3D11Device*, LData*);

	//Set the method to callback when a piece of the level was removed - Used when dropping power ups
	void SetPowerUpCallback(function<void(XMFLOAT2)>);

	//Generate a path for a enemy object
	void FindPath(Enemy*);

	//Modify the level in runtime
	void ModifyLevel(int, int);
	//Modify the pathfinding to avoid or allow pathing over this object
	void ModifyPath(int, int, int);

	//Get the level bounding box
	BoundingBox* GetBox();

	void GetClosestIndex(XMFLOAT2 position, XMFLOAT2& index);

private:
	//Methods
	void Shutdown();

	//Variables
	shared_ptr<BoundingBox> m_pBox;

	//Callback method
	function<void(XMFLOAT2)> m_powerUpCallback;

};

#endif//_LEVEL_DATA_H_
