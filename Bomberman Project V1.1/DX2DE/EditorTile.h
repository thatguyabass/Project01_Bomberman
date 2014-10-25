//-------------------------------------------------------------------
//EditorTile.h
//-------------------------------------------------------------------
//Handles the behaviour of this Editor tile for the Editor grid

#ifndef _EDITOR_TILE_H_
#define _EDITOR_TILE_H_

#include "BaseNode.h"
#include "TileType.h"
#include "EditorWaypoint.h"

class EditorTile : public BaseNode
{
public:
	EditorTile();
	~EditorTile();

	bool Initialize(ID3D11Device* device, int imageWidth, int imageHeight, XMFLOAT2 position, XMFLOAT2, const shared_ptr<TileType>);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& orthoView, XMFLOAT4X4& ortho);

	//Check for any intersecting bounding boxes with this tile
	bool CheckBounds(BoundingBox*);

	//==Operator override
	bool operator==(const EditorTile&);
	bool operator==(EditorTile&);

	//Swap this tile for a different type
	void SwapTile(shared_ptr<TileType>);
	bool GetSpawn();//Is this tile the spawn
	bool GetExit();//Is this tile the exit
	bool GetEnemy();//Is this tile an enemy
	bool WaypointEmpty();//does this tile have any waypoints attached to it?

	void AddWaypoint(XMFLOAT2);//Add a waypoint to this tile
	void RemoveWaypoint(XMFLOAT2);//Remove a waypoint from this tile
	void ResetWaypoints();//Reset the waypoints to 0
	EdWaypoint& GetWaypoints();//Return all the waypoints 
	int GetWaypointCount();//Return waypoint count
	
	void SetInvalid(bool value = true);//Set the Invalid State
	bool GetInvalid();//Get the Invalid State

private:
	//Methods
	void Shutdown();
	void SwapTexture(shared_ptr<Texture>);//Swap this tiles texture with a different texture
	void NodeSetup(int);//Setup this node for pathfinding
	
	//Variables
	EdWaypoint m_waypoint;
	bool m_invalid;
	shared_ptr<Static2D> m_pInvalid; //Use to demenstrate an invalid node
	
	bool m_exit;
	bool m_spawn;
	bool m_enemy;
	int m_type;//Type used for enemy classification

};

#endif//_TILE_H_