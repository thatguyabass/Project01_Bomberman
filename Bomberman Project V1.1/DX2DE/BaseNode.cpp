//-------------------------------------------------------------
//BaseNode.cpp
//-------------------------------------------------------------
//

#include "BaseNode.h"

BaseNode::BaseNode()
{
	//Set Base Values
	m_F = 0;
	m_H = 0;
	m_G = 0;
}

BaseNode::~BaseNode()
{
	Shutdown();
}

bool BaseNode::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	if(_DEBUG_PATHFINDING)
	{
		result = m_pNode->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void BaseNode::Shutdown()
{
	for(int c = 0; c < m_pNodes.size(); c++)
		m_pNodes[c] = 0;

	if(m_pNode)
	{
		m_pNode.reset();
		m_pNode = 0;
	}
}

XMFLOAT2 BaseNode::GetDimensions()
{
	return XMFLOAT2(m_imageWidth, m_imageHeight);
}

bool BaseNode::CheckNode(BaseNode* node)
{
	//Check if the node is this node
	if(this == node)
		return false;

	//Make sure the node isn't already added to the adjacent nodes
	for(int c = 0; c < m_pNodes.size(); c++)
		if(node == m_pNodes[c])
			return false;
	
	return true;
}
void BaseNode::AddNode(BaseNode* node)
{
	if(node != this)
		m_pNodes.push_back(node);
}

BaseNode* BaseNode::GetNode(int index)
{
	if(m_pNodes.size() > 0)
		return m_pNodes[index];
	else 
		return NULL;
}

int BaseNode::NodeCount()
{
	return m_pNodes.size();
}

int BaseNode::GetID()
{
	return m_id;
}
XMFLOAT2 BaseNode::GetIndex()
{
	return m_index;
}

//Check if this node is active
bool BaseNode::GetNodeState()
{
	return (e_node == ACTIVE);
}
//Set the Node State
void BaseNode::SetNodeState(int value)
{
	switch(value)
	{
	case 0: e_node = ACTIVE;	break;
	case 1: e_node = INACTIVE; break;
	}
}

//Return the Tile State
int BaseNode::GetTileState()
{
	return e_tile;
}
//Set the Tile State
void BaseNode::SetTileState(int value)
{
	switch(value)
	{
	case 0: e_tile = SPACE; break;
	case 1: e_tile = NO_SPACE; break;
	case 2: e_tile = DESTRUCTIBLE; break;
	case 3: e_tile = EXIT; break;
	case 4: e_tile = SPAWN; break;
	case 5: e_tile = HELI; break;
	case 6: e_tile = YELLOW; break;
	case 7: e_tile = MAD_GUY; break;
	case 8: e_tile = MAD_BOMB; break;
	case 9: e_tile = RABBIT; break;
	case 10: e_tile = BLUE_EYE; break;
	}
}

//Get the H Value
int BaseNode::GetH()
{
	return m_H;
}
//Set the current movement
void BaseNode::SetG(int G)
{
	m_G = G;
}
//Get the Current Movement
int BaseNode::GetG()
{
	return m_G;
}
//Get the F Value
int BaseNode::GetF()
{
	return m_F;
}
//Set the pathfinding values
void BaseNode::SetScores(BaseNode* node)
{
	if(node != this)
	{
		//Get the Distance difference 
		int X = m_index.x - node->m_index.x;
		int Y = m_index.y - node->m_index.y;

		//Make sure the values are positive
		if(X < 0) X *= -1;
		if(Y < 0) Y *= -1;
		m_H = X + Y;//Set the Estimated Movement

		m_F = m_H + m_G;//Set the F value
	}
}