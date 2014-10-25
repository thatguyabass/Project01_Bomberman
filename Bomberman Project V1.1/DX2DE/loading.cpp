//----------------------------------------------------
//loading.cpp
//----------------------------------------------------
//define the loading class

#include "loading.h"

Loading::Loading()
{

}

Loading::Loading(const Loading& src)
{

}

Loading::~Loading()
{
	Shutdown();
}

bool Loading::Initialize(shared_ptr<D3D> d3d, HWND hwnd)
{
	bool result;

	result = BaseLevel::Initialize(d3d);
	if(!result)
		return false;

	m_pImage = shared_ptr<Static2D>(new Static2D);
	if(!m_pImage)
		return false;

	result = m_pImage->Initialize(m_pD3D->GetDevice(), L"../DX2DE/Textures/BombermanLoading.jpg", Global::GetScreenWidth(), Global::GetScreenHeight() + 50, XMFLOAT2(0, 0), 0);
	if(!result)
		return false;

	////Create and Initialize the font and font shader
	//result = InitializeFont("../DX2DE/Font/Font.txt", L"../DX2DE/Font/Font.PNG", hwnd);
	//if(!result)
	//	return false;

	////Text Objects
	//m_pTextFPS = new Text;
	//if(!m_pTextFPS)
	//	return false;
	//result = m_pTextFPS->Initialize(m_pD3D->GetDevice(), hwnd, m_pFont, screenWidth, , "Hello this is not a test", 100, XMFLOAT2(1270, 10), XMFLOAT3(0,0,0));
	//if(!result)
	//	return false;

	return true;
}

bool Loading::Render()
{
	XMFLOAT4X4 world, view, ortho;
	bool result;

	BaseLevel::BeginScene();

	//Fill the Matrixes
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	result = m_pImage->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	BaseLevel::EndScene();

	return true;
}

void Loading::Shutdown()
{
	if(m_pImage)
	{
		//delete m_pImage;
		m_pImage.reset();
		m_pImage = 0;
	}
}
