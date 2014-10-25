//------------------------------------------------
//Seeker.h
//-------------------------------------------------------
//This class will be used to demonstrate finding paths using the Pathfinding Grid 
//for multiple objects

//Debug - never used in the application, was only used when creating the pathfinding

#ifndef _SEEKER_H_
#define _SEEKER_H_

#include "Waypoint.h"

class Seeker : public Base2D
{
public:
	Seeker();
	Seeker(const Seeker&);
	~Seeker();

	bool Initialize(ID3D11Device*, int, int, Node*, Waypoint* , XMFLOAT4);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	void RenderPath();

	Node* GetOrigin();
	Node* GetTarget();
	void AddNode(Node*);
	void ResetPath();

	bool GetPath();

private:
	//Methods
	void Shutdown();
	void Move(float);
	void UpdatePath();
	void Change(Node*, Node*, XMFLOAT4);

	//Variables
	float m_speed;
	int m_foresight;

	Waypoint* m_pWaypoint;

	Node* m_origin;
	Node* m_target;
	XMFLOAT4 m_color;
	vector<Node*> m_pPath;

	bool m_path;
	int m_targetIndex;
	int m_pathIndex;

};

#endif//_SEEKER_H_