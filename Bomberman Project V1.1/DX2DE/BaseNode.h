//-------------------------------------------------------------
//baseNode.h
//-------------------------------------------------------------
//Base Nodes handle the common behavior for the Editor Tiles and the Level Nodes

#ifndef _BASE_NODE_H_
#define _BASE_NODE_H_

#include "base2D.h"
#include "static2D.h"
#include "tileCharacter.h"

//------------------------------------------------------------------------------------

class BaseNode : public Base2D
{
public:
	BaseNode();
	~BaseNode();

	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	XMFLOAT2 GetDimensions();
	bool CheckNode(BaseNode*);//Check if the parameter node has been added to this node, Used for pathfinding
	void AddNode(BaseNode*);//Add the parameter node to this nodes vector - neighbouring nodes
	BaseNode* GetNode(int);//Get neighbouring nodes
	int NodeCount();//Get the number of neighbouring nodes

	//Pathfinding F = H + G
	int GetH();//Estimated Path Value
	void SetG(int);//Set the Move Value
	int GetG();//Get the Move Value
	int GetF();//Get the Correct Final Value
	void SetScores(BaseNode*);//Calculate the F value using H + G using this node and the target node

	int GetID();//Get the Tile State
	int GetID() const;
	XMFLOAT2 GetIndex();//Get the Tile Index
	
	bool GetNodeState();//Is this node active?
	void SetNodeState(int);//Set the Node State

	int GetTileState();//Return the Tile State
	void SetTileState(int);//Set the Tile State

	//States - Each Node will use both these states
	enum NODE_STATE{ACTIVE, INACTIVE };//Used for pathfinding
	enum TILE_STATE{ SPACE, NO_SPACE, DESTRUCTIBLE, EXIT, SPAWN , HELI, YELLOW, MAD_GUY, MAD_BOMB, RABBIT, BLUE_EYE};//Used for creating and displaying the correct tile

	bool operator==(const BaseNode&);
	bool operator==(BaseNode&);

protected:
	//Methods
	virtual void Shutdown();

	//State
	NODE_STATE e_node;
	TILE_STATE e_tile;

	//Variables
	int m_id;
	XMFLOAT2 m_index;

	//Pathfinding
	vector<BaseNode*> m_pNodes;//neighbouring nodes - Each node has this variable to create the network for pathfinding
	int m_F;//Final
	int m_H;//Estimated movement
	int m_G;//Actual movement
	shared_ptr<Static2D> m_pNode;//Debug

};

#endif//_BASE_NODE_H_