//---------------------------------------------------
//Seeker.cpp
//---------------------------------------------------
//Define the Seeker Class
//Demonstrate the ability to find a valid path using multiple objects 

//Debug - never used in the application, was only used when creating the pathfinding

#include "Seeker.h"

Seeker::Seeker()
{
	m_pathIndex = 0;
	m_targetIndex = 0;
	m_speed = 0.35f;
	m_foresight = 2;
}

Seeker::Seeker(const Seeker& src)
{

}

Seeker::~Seeker()
{
	Shutdown();
}

bool Seeker::Initialize(ID3D11Device* device, int imageWidth, int imageHeight, Node* start, Waypoint* waypoint, XMFLOAT4 color)
{
	bool result;

	result = Base2D::Initialize(device, imageWidth, imageHeight, start->GetPosition(), 0);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, StringManager::Seeker());
	if(!result)
		return false;

	m_pWaypoint = waypoint;

	Change(start, waypoint->GetTarget(m_targetIndex), color);

	return true;
}

void Seeker::Update(float gameTime)
{
	m_path = false;
	Move(m_speed * gameTime);
	UpdatePath();
}

bool Seeker::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}


void Seeker::RenderPath()
{
	for(int c = 0; c < m_pPath.size(); c++)
		m_pPath[c]->SetColor(m_color);
}

void Seeker::Shutdown()
{
	m_pPath.clear();
}

Node* Seeker::GetOrigin()
{
	return m_origin;
}
Node* Seeker::GetTarget()
{
	return m_target;
}

void Seeker::AddNode(Node* node)
{
	m_pPath.push_back(node);
}
void Seeker::ResetPath()
{
	for(int c = 0; c < m_pPath.size(); c++)
		m_pPath[c]->SetColor(Color::Blue());

	m_pPath.clear();
}

void Seeker::Change(Node* origin, Node* target, XMFLOAT4 color)
{
	m_origin = origin;
	m_target = target;
	m_color = color;
}

void Seeker::Move(float d)
{
	XMFLOAT2 pos;
	float x; 
	float y;

	pos = m_pPath[m_pathIndex]->GetPosition();
	x = pos.x - POSITION.x;
	y = pos.y - POSITION.y;

	XMVECTOR t = XMVector2Normalize(XMLoadFloat2(&XMFLOAT2(x, y)));

	x = XMVectorGetX(t);
	y = XMVectorGetY(t);

	POSITION.x += (d * x);
	POSITION.y += (d * y);
}

void Seeker::UpdatePath()
{
	float x = m_pPath[m_pathIndex]->GetPosition().x - POSITION.x;
	float y = m_pPath[m_pathIndex]->GetPosition().y - POSITION.y;
	if(x < 0) x *= -1;
	if(y < 0) y *= -1;
	float dis = x + y;
	if(dis < 5)
	{
		m_pathIndex++;
		if(m_pathIndex >= m_pPath.size())
		{
			m_path = true;
			int last = m_targetIndex;

			m_targetIndex++;
			if(m_targetIndex >= m_pWaypoint->Length())
				m_targetIndex = 0;

			Change(m_pWaypoint->GetTarget(last), m_pWaypoint->GetTarget(m_targetIndex), m_color);

			m_pathIndex = 0;
		}

		for(int c = m_pathIndex; c < m_pathIndex + m_foresight; c++)
		{
			if(c >= m_pPath.size())
				break;

			if(!m_pPath[c]->GetNodeState())
				m_path = true;
		}
	}
}

bool Seeker::GetPath()
{
	return m_path;
}