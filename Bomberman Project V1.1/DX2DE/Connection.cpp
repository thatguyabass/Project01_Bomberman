//-------------------------------------------------------------
//Connection.cpp
//-------------------------------------------------------------
//define the Connection Class

#include "Connection.h"

Connection::Connection()
{
	m_render = true;
	m_pN1 = 0;
	m_pN2 = 0;

	m_pED1 = 0;
	m_pED2 = 0;
}

Connection::Connection(const Connection& src)
{

}

Connection::~Connection()
{
	Shutdown();
}

bool Connection::Initialize(ID3D11Device* device, XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color)
{
	bool result;
	m_colorOverride = color;

	//Get Length
	float A = pos2.x - pos1.x;
	float O = pos2.y - pos1.y;
	
	//Get Distance
	float SA = A * A;
	float SO = O * O;

	//Get Distance between points
	float H = sqrt(SA + SO); 

	//Get the angle between the two points
	float AN = acos(A / H);
	AN = XMConvertToDegrees(AN) * -1;

	float height = 5.0f;

	//Average distance between the two points
	XMFLOAT2 pos;
	pos.x = (pos2.x + pos1.x) / 2;
	pos.y = (pos2.y + pos1.y) / 2;

	//Initialize
	result = Base2D::Initialize(device, H, height, pos, AN);
	if(!result)
		return false;

	//Load Texture
	result = Base2D::LoadTexture(device, StringManager::Connection());
	if(!result)
		return false;

	return true;
}

void Connection::Update(float gameTime)
{
	if(m_pN1)
	{
		if(!m_pN1->GetNodeState() || !m_pN2->GetNodeState())
			m_render = false;
		else
			m_render = true;
	}
	else if(m_pED1)
	{
		if(!m_pED1->GetNodeState() || !m_pED2->GetNodeState())
			m_render = false;
		else
			m_render = true;
	}
}

bool Connection::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	if(m_render)
	{
		result = Base2D::Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}
	return true;
}

void Connection::SetRender(bool value)
{
	m_render = value;
}
bool Connection::GetRender()
{
	return m_render;
}

void Connection::SetObjects(Node* N1, Node* N2)
{
	m_pN1 = N1;
	m_pN2 = N2;
}

void Connection::SetObjects(EditorTile* ED1, EditorTile* ED2)
{
	m_pED1 = ED1;
	m_pED2 = ED2;
}

void Connection::Shutdown()
{
	m_pN1 = 0;
	m_pN2 = 0;

	m_pED1 = 0;
	m_pED2 = 0;
}