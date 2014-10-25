//-----------------------------------------------------------------------
//PathfindingTestLevel.cpp
//-----------------------------------------------------------------------
//Define the Pathfinding Test Level

#include "PathfindingTestLevel.h"

PathfindingTestLevel::PathfindingTestLevel()
{
	//m_pPathGrid = 0;
	m_pHUD = 0;
	time = 0;
	M = false;
	test = true;
}

PathfindingTestLevel::PathfindingTestLevel(const PathfindingTestLevel& src)
{

}

PathfindingTestLevel::~PathfindingTestLevel()
{
	Shutdown();
}

bool PathfindingTestLevel::Initialize(shared_ptr<D3D> d3d, shared_ptr<Input> input, int width, int height, HWND hwnd, WCHAR* level)
{
	bool result;

	result = BaseLevel::Initialize(d3d, input, width, height, hwnd);
	if(!result)
		return false;
	
	//Initialize Pathfinding node grid
	m_pData = new LevelData;
	//result = m_pData->Initialize(m_pD3D->GetDevice(), width, height, L"../DX2DE/Levels/PathTest3.txt");
	//if(!result)
	//	return false;

	//m_pPathGrid = m_pData->GetPath();

	//Debug Text
	//result = m_pPathGrid->InitializeText(m_pD3D->GetDevice(), width, height, hwnd);
	//if(!result)
	//	return false;

	m_pHUD = new HUD;
	if(!m_pHUD)
		return false;

	result = m_pHUD->Initialize(m_pD3D->GetDevice(), width, height, hwnd);
	if(!result)
		return false;

	InitializeWaypoints();

	//m_pSeeker.Add(new Seeker);
	//result = m_pSeeker[0]->Initialize(m_pD3D->GetDevice(), width, height, 25, 25, m_pPathGrid->GetNode(12, 10), m_pWaypoint[0], Color::Purple());
	//if(!result)
	//	return false;

	//m_pSeeker.Add(new Seeker);
	//result = m_pSeeker[1]->Initialize(m_pD3D->GetDevice(), width, height, 25, 25, m_pPathGrid->GetNode(5, 10), m_pWaypoint[1], Color::Green());
	//if(!result)
	//	return false;

	//for(int c = 0; c < m_pSeeker.Length(); c++)
	//{
	//	m_pPathGrid->FindPath(m_pSeeker[c]);
	//	m_pSeeker[c]->RenderPath();
	//}

	return true;
}

void PathfindingTestLevel::Update(float gameTime)
{
	m_pData->Update(gameTime);
	//m_pPathGrid->UpdateText(gameTime);

	m_pHUD->Update();

	//for(int c = 0; c < m_pSeeker.Length(); c++)
	//{
	//	m_pSeeker[c]->Update(gameTime);
	//	if(m_pSeeker[c]->GetPath())
	//	{
	//		m_pPathGrid->FindPath(m_pSeeker[c]);
	//		m_pSeeker[c]->RenderPath();
	//	}
	//}

	//if(m_pInput->KeyDown(DIK_L))
	//{
	//	if(test)
	//	{
	//		test = false;
	//		m_pPathGrid->GetNode(15,2)->SetState(1);
	//		m_pPathGrid->GetNode(15,3)->SetState(1);
	//		m_pPathGrid->GetNode(15,2)->SetColor(Color::Red());
	//		m_pPathGrid->GetNode(15,3)->SetColor(Color::Red());
	//	}
	//	else
	//	{
	//		test = true;
	//		m_pPathGrid->GetNode(15,2)->SetState(0);
	//		m_pPathGrid->GetNode(15,3)->SetState(0);
	//		m_pPathGrid->GetNode(15,2)->SetColor(Color::Blue());
	//		m_pPathGrid->GetNode(15,3)->SetColor(Color::Blue());
	//	}
	//}
}

bool PathfindingTestLevel::Render()
{
	XMFLOAT4X4 world, view, ortho;
	bool result;

	BaseLevel::BeginScene();

	//Get the world, view, and projection Matrixes form the camera amd D3D objects
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	result = m_pData->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	//result = m_pPathGrid->RenderText(m_pD3D->GetDeviceContext(), world, view, ortho);
	//if(!result)
	//	return false;

	result = m_pHUD->Render(m_pD3D->GetDeviceContext(), world, view, ortho);
	if(!result)
		return false;

	for(int c = 0; c < m_pSeeker.Length(); c++)
	{
		result = m_pSeeker[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	BaseLevel::EndScene();

	return true;
}

void PathfindingTestLevel::Shutdown()
{
	//Release the Grid
	//if(m_pPathGrid)
	//	delete m_pPathGrid;

	//Release the HUD
	if(m_pHUD)
		delete m_pHUD;

	for(int c = 0; c < m_pWaypoint.Length(); c++)
		m_pWaypoint.RemoveNoMem(c);

	for(int c = 0; c < m_pSeeker.Length(); c++)
		m_pSeeker.Remove(c);
}

void PathfindingTestLevel::InitializeWaypoints()
{
	//m_pWaypoint.Add(new Waypoint);
	//m_pWaypoint[0]->SetWaypoints(4, m_pPathGrid->GetNode(32,10), m_pPathGrid->GetNode(26,24), m_pPathGrid->GetNode(12, 10), m_pPathGrid->GetNode(5,10));
	//m_pWaypoint.Add(new Waypoint);
	//m_pWaypoint[1]->SetWaypoints(3, m_pPathGrid->GetNode(12,10), m_pPathGrid->GetNode(35,24), m_pPathGrid->GetNode(5,10));
}