//------------------------------------------------
//PathfindingGrid.cpp
//------------------------------------------------
//

#include "PathfindingGrid.h"

PathGrid::PathGrid()
{
	m_X = 0;
	m_Y = 0;
	m_connectionCount = 0;

	m_pFont = 0;
}

PathGrid::PathGrid(const PathGrid& src)
{

}

PathGrid::~PathGrid()
{
	Shutdown();
}

bool PathGrid::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, int width, int height, float startX, float startY, LData* levelData)
{
	bool result;

	m_X = levelData->GetX();
	m_Y = levelData->GetY();

	int id = 0;
	m_ppNode = new Node*[m_Y];
	for(int c = 0; c < m_Y; c++)
	{
		m_ppNode[c] = new Node[m_X];
		for(int v = 0; v < m_X; v++)
			m_ppNode[c][v].Initialize(device, screenWidth, screenHeight, width / 5,  height / 5, levelData->GetChar(c, v), XMFLOAT2(startX + (width * v), startY - (height * c)), ++id, XMFLOAT2(v,c), XMFLOAT2(0,0), XMFLOAT2(0,0));
	}

	int counter = 0;
	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
		{
			//Up and Down
			if(c - 1 >= 0)
				AddNode(device, screenWidth, screenHeight, &m_ppNode[c][v], &m_ppNode[c - 1][v]);
			if(c + 1 < m_Y)
				AddNode(device, screenWidth, screenHeight, &m_ppNode[c][v], &m_ppNode[c + 1][v]);

		
			//Left and Right	
			if(v - 1 >= 0)
				AddNode(device, screenWidth, screenHeight, &m_ppNode[c][v], &m_ppNode[c][v - 1]);
			if(v + 1 < m_X)
				AddNode(device, screenWidth, screenHeight, &m_ppNode[c][v], &m_ppNode[c][v + 1]);
		}
	}

	m_pPath = shared_ptr<Pathfinding>(new Pathfinding);

	return true;
}

void PathGrid::Update(float gameTime)
{
	for(int c = 0; c < m_connectionCount; c++)
		m_pConnection[c]->Update(gameTime);
}

bool PathGrid::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pConnection.size(); c++)
		m_pConnection[c]->Render(context, shader, world, view, ortho);

	for(int c = 0; c < m_Y; c++)
	{
		for(int v = 0; v < m_X; v++)
			m_ppNode[c][v].Render(context, shader, world, view, ortho);
	}

	return true;
}

void PathGrid::Shutdown()
{
	for(int c= 0; c < m_Y; c++)
		delete[] m_ppNode[c];
	delete[] m_ppNode;

	for(int c = 0; c < m_pConnection.size(); c++)
		delete m_pConnection[c];

	//Release Pathfinding
	if(m_pPath)
	{
		m_pPath.reset();
		m_pPath = 0;
	}
}

Node* PathGrid::GetNode(int x, int y)
{
	return &m_ppNode[y][x];
}

XMFLOAT2 PathGrid::GetNodePosition(Node* node)
{
	return node->GetPosition();
}

void PathGrid::AddNode(ID3D11Device* device, int width, int height, Node* N1, Node* N2)
{
	if(N1->CheckNode(N2))
	{
		N1->AddNode(N2);
		N2->AddNode(N1);
		MakeConnection(device, width, height, N1, N2);
	}
}
void PathGrid::MakeConnection(ID3D11Device* device, int width, int height, Node* N1, Node* N2)
{
	m_pConnection.push_back(new Connection);
	m_pConnection[m_connectionCount]->Initialize(device, width, height, N1->GetPosition(), N2->GetPosition(), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	m_pConnection[m_connectionCount]->SetObjects(N1, N2);
	m_connectionCount++;
}

void PathGrid::FindPath(Seeker* seeker)
{
	m_pPath->FindPath(seeker);
}

void PathGrid::FindPath(Enemy* enemy)
{
	m_pPath->FindPath(enemy);
}

//------------------------------DEBUG TEXT------------------------------------------
bool PathGrid::InitializeText(ID3D11Device* device, int width, int height, HWND hwnd)
{
	bool result;

	if(!m_pFont)
	{
		//Create Font Object
		m_pFont = new Font;
		if(!m_pFont)
			return false;

		//Initialzie Font
		result = m_pFont->Initialize(device, StringManager::FontText(), StringManager::FontTexture());
		if(!result)
			return false;

		//Create Font Shader Object
		m_pFontShader = new FontShader;
		if(!m_pFontShader)
			return false;

		result = m_pFontShader->Initialize(device, hwnd);
		if(!result)
			return false;
	}

	for(int c = 0; c < m_X; c++)
	{
		for(int v = 0; v < m_Y; v++)
		{
			result = m_ppNode[v][c].InitializeText(device, m_pFont, width, height, hwnd);
			if(!result)
				return false;
		}
	}

	return true;
}

void PathGrid::UpdateText(float gameTime)
{
	for(int c = 0; c < m_X; c++)
	{
		for(int v = 0; v < m_Y; v++)
			m_ppNode[v][c].UpdateText(gameTime);
	}
}

bool PathGrid::RenderText(ID3D11DeviceContext* context, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_X; c++)
	{
		for(int v = 0; v < m_Y; v++)
		{
			result = m_ppNode[v][c].RenderText(context, m_pFontShader, world, view, ortho);
			if(!result)
				return false;
		}
	}

	return true;
}