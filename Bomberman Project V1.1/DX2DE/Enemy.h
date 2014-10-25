//---------------------------------------------------
//enemy.h
//---------------------------------------------------
//base class for all enemy type objects. This class will should never be used independent from the child classes due to haveint no texture contorol.
//Each of the child classes are responcible for their own texture. This was implemented before the StringManager:: static class was created. 

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "base2D.h"
#include "Waypoint.h"

class Enemy : public Base2D
{
public:
	Enemy();
	Enemy(const Enemy&);
	virtual ~Enemy();

	//Node* Start will be used to track the starting position, Waypoints will be used for future movement through the level
	virtual bool Initialize(ID3D11Device*, XMFLOAT2 size, Node* start, Waypoint*);
	//Update the enemeies movement, animation, Collision and Position
	virtual void Update(float);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);
	//Debug used for rendering the path the object is taking
	void RenderPath(XMFLOAT4);

	bool GetHit();
	void Hit();
	
	//Get the original location - Normally the last node reached before starting a new path
	Node* GetOrigin();
	//Set the next desired location
	void SetTarget(Node*);
	Node* GetTarget();
	//Used as a back-up in case the target node is completly blocked
	void AddNode(Node*);
	void ResetPath();
	//Flags used to check if a path was found and if that path is a valid path (can reach point A-B)
	bool GetPath();
	void SetValid();
	bool GetValid();

protected:
	//Method
	void Shutdown();
	//The objects will always move towards the next node in the path
	void Move(float);
	//Update the current node the object is traveling towards
	void UpdatePath();
	//Change the current path
	void Change(Node* start, XMFLOAT4); //<- Node is used as the origin - Color for Debug
	void Change(Node* start, Node* target, XMFLOAT4); //<- First node used for Origin - Second for Target - Color for Debug

	//Variable
	//Target nodes used for pathfinding
	Waypoint* m_pWaypoint;

	//Current starting point for the path
	Node* m_pOrigin;
	//Target for the current path
	Node* m_pTarget;
	//Pathfinding Debug
	XMFLOAT4 m_color;
	//Nodes along the current path this object is taking
	vector<Node*> m_pPath;

	float m_speed;
	//How far in advance can the object before generating a new path
	int m_foresight;

	float m_xVelocity;
	float m_yVelocity;

	//Pathfinding 
	bool m_valid;
	bool m_path;
	//Index of the current node in the vector Node Array
	int m_pathIndex;
	int m_targetIndex;
	//Minimum distance before the updating the path
	int m_minDist;

	//has this object been hit?
	bool m_hit;

};

#endif//_ENEMY_H_
