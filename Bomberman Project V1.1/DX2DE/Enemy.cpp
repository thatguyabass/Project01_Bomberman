//-------------------------------------------
//enemy.cpp
//-------------------------------------------
//Define the Enemy class

#include "Enemy.h"

Enemy::Enemy()
{
	//Default Values
	m_speed = 0.05f;
	m_xVelocity = 0;
	m_yVelocity = 0;

	m_boundsMod = 0.8f;
	m_path = true;
	m_valid = true;

	//Pathfinding pre-set values
	m_pathIndex = 0;
	m_targetIndex = 0;
	m_foresight = 2;
	m_minDist = 5;
}

Enemy::Enemy(const Enemy& src)
{

}

Enemy::~Enemy()
{
	Shutdown();
}

bool Enemy::Initialize(ID3D11Device* device, XMFLOAT2 imageSize, Node* start, Waypoint* waypoint)
{
	bool result;

	result = Base2D::Initialize(device, imageSize.x * ASPECT.x, imageSize.y * ASPECT.y, start->GetPosition(), 0);
	if(!result)
		return false;

	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(imageSize.x * m_boundsMod, imageSize.y * m_boundsMod, start->GetPosition());

	m_pWaypoint = waypoint;

	//Waypoint Null Check - Will make the starting node the only node 
	if(m_pWaypoint->Length() <= 0)
		m_pWaypoint->SetWaypoint(start);

	Change(start, m_pWaypoint->GetTarget(0), Color::Yellow());

	m_pOrigin = start;
	m_pTarget = m_pWaypoint->GetTarget(0);

	//Normalize the Speed by applying the aspect ratio average to the speed
	m_speed *= ((ASPECT.x + ASPECT.y) / 2);

	return true;
}

void Enemy::Update(float gameTime)
{
	//Reset these values each update to check for variations
	m_path = false;
	m_hit = false;

	Move(m_speed * gameTime);
	UpdatePath();
}

bool Enemy::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void Enemy::Shutdown()
{
	//Release and reset the Path
	ResetPath();

	//Release Reference to Origin
	if(m_pOrigin)
		m_pOrigin = 0;

	//Release Target
	if(m_pTarget)
		m_pTarget = 0;

	//Release Reference to Waypoints
	if(m_pWaypoint)
		m_pWaypoint = 0;
}

//Update the Path - Increase the index if the next node was reached or Flag for a new path to be created
void Enemy::UpdatePath()
{
	//Get the Distance between the current node and the current position
	float x = m_pPath[m_pathIndex]->GetPosition().x - POSITION.x;
	float y = m_pPath[m_pathIndex]->GetPosition().y - POSITION.y;
	if(x < 0) x *= -1;//Normalize the values
	if(y < 0) y *= -1;
	float dis = x + y;
	//If the combined X-Y distance is less then min distance the node has been reached
	if(dis < m_minDist)
	{
		m_pathIndex++;
		//Check the nodes ahead of to see if they are valid
		for(int c = m_pathIndex; c < m_pathIndex + m_foresight; c++)
		{
			//if foresight exceeds the path size break
			if(c >= m_pPath.size())
				break;

			//if not valid, find a new path
			if(!m_pPath[c]->GetNodeState())
			{
				m_path = true;
				m_valid = false;

				Change(m_pPath[m_pathIndex - 1], m_color);
				m_pathIndex = 0;
			}
		}
		
		//Target Reached, find a new path
		if(m_pathIndex >= m_pPath.size())
		{
			m_path = true;
			int last = m_targetIndex;

			m_targetIndex++;
			if(m_targetIndex >= m_pWaypoint->Length())
				m_targetIndex = 0;

			Change(m_pPath[m_pPath.size() - 1], m_pWaypoint->GetTarget(m_targetIndex), m_color);

			m_pathIndex = 0;
		}
	}
}

//Mode the object towards the next Node in the Path Vector 
void Enemy::Move(float speed)
{
	XMFLOAT2 pos;
	float x;
	float y;

	//Get the Current Nodes Position
	pos = m_pPath[m_pathIndex]->GetPosition();
	x = pos.x - POSITION.x;
	y = pos.y - POSITION.y;

	//Normalize
	XMVECTOR t = XMVector2Normalize(XMLoadFloat2(&XMFLOAT2(x,y)));

	x = XMVectorGetX(t);
	y = XMVectorGetY(t);

	//Apply to the position
	POSITION.x += (speed * x);
	POSITION.y += (speed * y);
}

void Enemy::Hit()
{
	m_hit = true;
}

bool Enemy::GetHit()
{
	return m_hit;
}

bool Enemy::GetPath()
{
	return m_path;
}
//Must be called after a Valid path has been created
void Enemy::SetValid()
{
	m_valid = true;
}
bool Enemy::GetValid()
{
	return m_valid;
}

//Private SetOrigin|Target|Color Method
void Enemy::Change(Node* start, Node* target, XMFLOAT4 color)
{
	m_pOrigin = start;
	m_pTarget = target;
	m_color = color;
}
//Private SetOrigin|Color Method
void Enemy::Change(Node* start, XMFLOAT4 color)
{
	m_pOrigin = start;
	m_color = color;
}

void Enemy::AddNode(Node* node)
{
	m_pPath.push_back(node);
}

Node* Enemy::GetOrigin()
{
	return m_pOrigin;
}
Node* Enemy::GetTarget()
{
	return m_pTarget;
}
//Call when a new path is needed - Current Set path is invalid
void Enemy::SetTarget(Node* target)
{
	m_pTarget = target;
}
void Enemy::ResetPath()
{
	if(_DEBUG_PATHFINDING)
	{
		for(int c = 0; c < m_pPath.size(); c++)
			m_pPath[c]->GetDebugNode()->SetColor(Color::Blue());
	}
	//The Pointers added to the vector are Raw Pointers located in the LevelData() class - Var name : m_ppNodes
	m_pPath.clear();
}

void Enemy::RenderPath(XMFLOAT4 color)
{
	if(_DEBUG_PATHFINDING)
	{
		for(int c = 0; c < m_pPath.size(); c++)
			m_pPath[c]->GetDebugNode()->SetColor(color);
	}
}