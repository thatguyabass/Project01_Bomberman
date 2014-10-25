//-----------------------------------------------------
//GUITestLevel.cpp
//-----------------------------------------------------
//


#include "GUITestLevel.h"

Test::Test()
{

}

Test::~Test()
{

}

bool Test::Initialize(shared_ptr<D3D> d3d,wstring level, bool t)
{
	bool result;

	result = BaseLevel::Initialize(d3d);
	if(!result)
		return false;

	result = Global::GetCursor()->Initialize(m_pD3D->GetDevice(), StringManager::Connection(), 10 * ASPECT.x, 10 * ASPECT.y);
	if(!result)
		return false;

	Global::GetCursor()->SetColor(Color::Red());

	Util::ReadFolder(str, StringManager::LevelDirectory());

	m_pFont = shared_ptr<Font>(new Font);
	m_pFont->Initialize(m_pD3D->GetDevice(), StringManager::FontText(), StringManager::FontTexture());

	for(int c = 0; c < str.size(); c++)
	{
		m_pText.push_back(shared_ptr<Text>(new Text));
		result = m_pText[c]->Initialize(m_pD3D->GetDevice(), m_pFont, str[c], 300, XMFLOAT2(0,100 - (c * 50)), Color::White());
	}

	m_pDrop = shared_ptr<DynamicDropdown>(new DynamicDropdown);
	result = m_pDrop->Initialize(m_pD3D->GetDevice(), m_pFont, StringManager::GUIBlank(),
		200, 100, XMFLOAT2(-400 * ASPECT.x, 250 * ASPECT.y), 5, 1, 0.9f);
	if(!result)
		return false;

	m_pTextBox = shared_ptr<TextBox>(new TextBox);
	result = m_pTextBox->Initialize(m_pD3D->GetDevice(), m_pFont, 200,50, XMFLOAT2(-100 * ASPECT.x, 300 * ASPECT.y), true, "Hello", 11, 0.75f);
	if(!result)
		return false;

	m_pTextBox1 = shared_ptr<TextBox>(new TextBox);
	result = m_pTextBox1->Initialize(m_pD3D->GetDevice(), m_pFont, 200,50, XMFLOAT2(150 * ASPECT.x, 300 * ASPECT.y), false, "World", 11, 0.75f);
	if(!result)
		return false;

	m_dropTest = shared_ptr<Text>(new Text);
	result  = m_dropTest->Initialize(m_pD3D->GetDevice(), m_pFont, m_string, 100, XMFLOAT2(300,0), Color::White());
	if(!result)
		return false;

	m_pBundle = shared_ptr<LevelBundle>(new LevelBundle);
	m_pBundle->Initialize(StringManager::CampaignBundle());

	m_pButton = shared_ptr<Button>(new Button);
	result = m_pButton->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200,200, XMFLOAT2(200,0), ONCE);
	if(!result)
		return false;
	m_pButton->SetFunction(std::bind(&Test::TestFunction, this, _1), 0);

	return true;
}

void Test::Update(float gameTime)
{
	m_pDrop->Update(gameTime, Global::GetCursor()->GetBox());
	m_pTextBox->Update(gameTime, Global::GetCursor()->GetBox());
	m_pTextBox1->Update(gameTime, Global::GetCursor()->GetBox());
	m_pButton->Update(gameTime, Global::GetCursor()->GetBox());
	Global::GetCursor()->Update(gameTime);
}

bool Test::Render()
{
	bool result;

	XMFLOAT4X4 world, view, ortho;

	//Get the world, view, and projection Matrixes form the camera amd D3D objects
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	BaseLevel::BeginScene();

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	result = m_pTextBox->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pTextBox1->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pDrop->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	result = Global::GetCursor()->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_dropTest->Render(m_pD3D->GetDeviceContext(),m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	result = m_pButton->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	BaseLevel::EndScene();

	return true;
}

void Test::TestFunction(int value)
{
	m_pDrop->Populate(m_pBundle->GetLevels());
}