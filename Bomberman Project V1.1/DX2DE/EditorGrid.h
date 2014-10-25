//---------------------------------------------
//EditorGrid.h
//---------------------------------------------
//Handles the Creation and modification of the Editor Level

#ifndef _EDITOR_GRID_H_
#define _EDITOR_GRID_H_

#include "EditorTile.h"
//#include "Connection.h"
#include "TileType.h"
//#include "Pathfinding.h"

#include "LevelInfo.h"

//#include<fstream>
#include "BaseGrid.h"

//-------------------------------------------------------------------------------------------------------------

#define NOT_FOUND XMFLOAT2(-1, -1)

class EditorGrid : public BaseGrid<EditorTile>
{
public:
	EditorGrid();
	~EditorGrid();

	//Initialize the Editor level based on the provided data
	bool Initialize(ID3D11Device*, int, int);//new levels
	bool Initialize(ID3D11Device*, LevelInfo*);//Editor existing levels

	//Swap the tile from one type to another
	void SwapTile(int, int, int);

	//Send this level to a text file to be used by the player
	bool Submit(string);

private:
	//Methods
	void Shutdown();

	//Initialize a level tile
	bool InitializeTile(ID3D11Device*, char, int, int);
	//Set up the Tile Presets
	bool SetTiles(ID3D11Device*);
	
	//Set up the Nodes based on the data previded in Initialize
	bool SetNodes(ID3D11Device*,int, int);
	bool SetNodes(ID3D11Device*, LData*);

	//Override the Base Pathfinding to add more functionality
	void SetPathfinding(ID3D11Device*);
	//Set the enemies around the level
	void SetEnemies(EData*);

	//Find the grid location of Spawn and Exit
	XMFLOAT2 FindSpawn();
	XMFLOAT2 FindExit();
	
	//Find and replace other spawn and exit indexes - only one or each can exist in a level
	void CheckExit(int, int);
	void CheckSpawn(int, int);
	//Does a clear path exist between two points
	bool ClearPath(EditorTile*, EditorTile*);
	bool CheckInvalid();//Check if the enemies can be reached by the player
	//Does this index exist?
	bool CheckIndex(XMFLOAT2);

	//Variables
	int m_typeCount;
	vector<shared_ptr<TileType>> m_pTile;

	//File path to the text file this level will be saved on
	string m_filePath;
	string m_fileExtention;

};

#endif//_EDITOR_GRID_H_