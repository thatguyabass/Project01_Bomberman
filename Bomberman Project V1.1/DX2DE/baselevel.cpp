//------------------------------------
//baselevel.cpp
//------------------------------------
//Define the Graphics class responcable for rendering for the application

#include "baselevel.h"

BaseLevel::BaseLevel()
{
	m_pD3D = 0;
	m_pCamera = 0;
	m_pTextureShader = 0;
}

BaseLevel::BaseLevel(const BaseLevel& src)
{

}

BaseLevel::~BaseLevel()
{
	Shutdown();
}

bool BaseLevel::Initialize(shared_ptr<D3D> d3d)
{
	bool result;

	m_pD3D = d3d;

	InitializeCamera();

	//Create a new Texture Shader
	m_pTextureShader = shared_ptr<TextureShader>(new TextureShader);
	if(!m_pTextureShader)
		return false;

	HWND hwnd = Global::GetHWND();

	//Initialize the Texture shader
	result = m_pTextureShader->Initialize(m_pD3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Texture Shader object", L"Texture Shader Error", MB_OK);
		return false;
	}

	return true;
}

void BaseLevel::InitializeCamera()
{
	int width = Global::GetScreenWidth();
	int height = Global::GetScreenHeight();

	m_pCamera = shared_ptr<OrthoCamera>(new OrthoCamera);
	
	m_pCamera->SetPosition(0.0f, 0.0f, -15.0f);
	m_pCamera->SetBounds(width, height);
}

void BaseLevel::Update(float gameTime)
{

}

void BaseLevel::Shutdown()
{
	//Release Direct 3D object Reference
	if(m_pD3D)
	{
		m_pD3D.reset();
		m_pD3D = 0;
	}

	//Release the Texture Shader
	if(m_pTextureShader)
	{
		m_pTextureShader.reset();
		m_pTextureShader = 0;
	}

	//Release the Orthographic
	if(m_pCamera)
	{
		m_pCamera.reset();
		m_pCamera = 0;
	}
}

void BaseLevel::BeginScene()
{
	//Generate the view matrix based on the camera's position
	m_pCamera->Render();

	//Turn of the depth buffer for 2D rendering
	m_pD3D->TurnZBufferOff();

	//Turn on Alpha Blending
	m_pD3D->TurnOnAlphaBlending();
}
void BaseLevel::EndScene()
{
	//Turn off Alpha Blending
	m_pD3D->TurnOffAlphaBlending();

	//Turn Depth back on for 3D rendering
	m_pD3D->TurnZBufferOn();
}

bool BaseLevel::Render()
{
	return true;
}

void BaseLevel::GetViewMatrix(XMFLOAT4X4& view)
{
	m_pCamera->GetViewMatrix(view);
}
XMFLOAT2 BaseLevel::GetCameraOffset()
{
	return m_pCamera->GetPosition2D();
}