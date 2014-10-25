//----------------------------------------------------------------------
//BaseGrid.h
//----------------------------------------------------------------------
//Base Grid contains the methods and variables that are used in both LevelData and EditorGrid

#ifndef _BASE_GRID_H_
#define _BASE_GRID_H_

#include "LevelInfo.h"
#include "Connection.h"

#include "Color.h"
#include "Pathfinding.h"

#include "static2D.h"

#include<functional>
#include<fstream>
using namespace std;

//--------------------------------------------------------------------------------------------------------------

template<class N>
class BaseGrid
{
public:
	BaseGrid();
	~BaseGrid();

	virtual void Update(float);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	//Node Count Access Methods
	int GetGridX();
	int GetGridY();

	N* GetNode(int, int);

	//Get the world position of a level piece
	XMFLOAT2 GetPosition(int, int);
	//Get the total dimensions of the level
	XMFLOAT2 GetLevelDimensions();
	//Get the dimensions of a node
	XMFLOAT2 GetNodeDimensions();
	//Get the Spawn and exit index
	XMFLOAT2 GetSpawn();
	XMFLOAT2 GetExit();

protected:
	//Methods
	virtual void Shutdown();

	virtual void SetPathfinding(ID3D11Device* device);

	//Internal Node Position
	XMFLOAT2 GetNodePosition(N*);
	//Add a node to each adjacent node
	void AddNode(ID3D11Device*, N*, N*);
	//Create a visual connection between the nodes - Debug
	void MakeConnection(ID3D11Device*, N*, N*);

	//Variables
	N** m_ppNode;

	//Pathfinding Debug
	int m_connectionCount;
	vector<Connection*> m_pConnection;

	//Pathfinding Object using Enemy and Node Objects
	shared_ptr<Pathfinding<Enemy,N>> m_pPath;

	//Node dimension counts
	int m_X;
	int m_Y;

	//Spawn and exit index
	XMFLOAT2 m_spawn;
	XMFLOAT2 m_exit;

};

template<typename N>
BaseGrid<N>::BaseGrid()
{
	m_connectionCount = 0;
}

template<typename N>
BaseGrid<N>::~BaseGrid()
{
	Shutdown();
}

template<typename N>
void BaseGrid<N>::Update(float gameTime)
{
	if(_DEBUG_PATHFINDING)
	{
		for(int c = 0; c < m_connectionCount; c++)
			m_pConnection[c]->Update(gameTime);
	}
}

template<typename N>
bool BaseGrid<N>::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	//Render the level
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			result = m_ppNode[c][v].Render(context, shader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	if(_DEBUG_PATHFINDING)
	{
		for(int c = 0; c < m_pConnection.size(); c++)
			m_pConnection[c]->Render(context, shader, world, view, ortho);
	}

	return true;
}

template<typename N>
void BaseGrid<N>::Shutdown()
{
	//Release Pathfinding
	if(m_pPath)
	{
		m_pPath.reset();
		m_pPath = 0;
	}

	for(int c= 0; c < m_Y; c++)
		delete[] m_ppNode[c];
	delete[] m_ppNode;

	if(_DEBUG_PATHFINDING)
	{
		//Release Connections
		for(int c = 0; c < m_pConnection.size(); c++)
			delete m_pConnection[c];
		m_pConnection.clear();
	}
}

//Add the Nodes to each adjacent node for pathfinding
template<typename N>
void BaseGrid<N>::AddNode(ID3D11Device* device, N* N1, N* N2)
{
	if(N1->CheckNode(N2))
	{
		N1->AddNode(N2);
		N2->AddNode(N1);
		if(_DEBUG_PATHFINDING)
			MakeConnection(device, N1, N2);
	}
}

//Create a visual connection between the nodes - Debug
template<typename N>
void BaseGrid<N>::MakeConnection(ID3D11Device* device, N* N1, N* N2)
{
	m_pConnection.push_back(new Connection);
	m_pConnection[m_connectionCount]->Initialize(device, N1->GetPosition(), N2->GetPosition(), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	m_pConnection[m_connectionCount]->SetObjects(N1, N2);
	m_connectionCount++;
}

template<typename N>
void BaseGrid<N>::SetPathfinding(ID3D11Device* device)
{
	//Connect the Nodes to complete the pathfinding
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			//Up and Down
			if(c - 1 >= 0)
				AddNode(device, &m_ppNode[c][v], &m_ppNode[c - 1][v]);
			if(c + 1 < m_Y)
				AddNode(device, &m_ppNode[c][v], &m_ppNode[c + 1][v]);

		
			//Left and Right	
			if(v - 1 >= 0)
				AddNode(device, &m_ppNode[c][v], &m_ppNode[c][v - 1]);
			if(v + 1 < m_X)
				AddNode(device, &m_ppNode[c][v], &m_ppNode[c][v + 1]);
		}
	}

	m_pPath = shared_ptr<Pathfinding<Enemy, N>>(new Pathfinding<Enemy, N>);
}

//Get the world position of the node
template<typename N>
XMFLOAT2 BaseGrid<N>::GetPosition(int x, int y)
{
	return m_ppNode[y][x].GetPosition();
}

template<typename N>
int BaseGrid<N>::GetGridX()
{
	return m_X;
}
template<typename N>
int BaseGrid<N>::GetGridY()
{
	return m_Y;
}

//Get the Node at the Provided index
template<typename N>
N* BaseGrid<N>::GetNode(int x, int y)
{
	return &m_ppNode[y][x];
}

//Get the Nodes Dimensions
template<typename N>
XMFLOAT2 BaseGrid<N>::GetNodeDimensions()
{
	return GetNode(0,0)->GetDimensions();
}
//Get the dimensions of the whole level
template<typename N>
XMFLOAT2 BaseGrid<N>::GetLevelDimensions()
{
	XMFLOAT2 d = GetNodeDimensions();
	return XMFLOAT2(d.x * m_X, d.y * m_Y);
}
template<typename N>
XMFLOAT2 BaseGrid<N>::GetExit()
{
	return XMFLOAT2(m_exit.x, m_exit.y);
}
template<typename N>
XMFLOAT2 BaseGrid<N>::GetSpawn()
{
	return XMFLOAT2(m_spawn.x, m_spawn.y);
}

#endif//_BASE_GRID_H_
