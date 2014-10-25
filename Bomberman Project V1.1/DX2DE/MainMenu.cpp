//--------------------------------------------------------------
//MainMenu.cpp
//--------------------------------------------------------------
//define the main menu
//Main menu will be the starting point for the users

#include "MainMenu.h"

MainMenu::MainMenu()
{
	//Base Values
	m_levelIndex = 0;
	m_levelSet = false;
	e_mainMenu = MainMenu::MAIN;
}

MainMenu::~MainMenu()
{
	Shutdown();
}

bool MainMenu::Initialize(shared_ptr<D3D> d3d, wstring level)
{
	bool result;

	//Set the Global Game State
	Util::e_globalGame = Util::MENU;

	//Initialize the level
	result = BaseLevel::Initialize(d3d);
	if(!result)
		return false;

	//Create and Initialize the Font
	m_pFont = shared_ptr<Font>(new Font);
	result = m_pFont->Initialize(m_pD3D->GetDevice(), StringManager::FontText(), StringManager::FontTexture());
	if(!result)
		return false;

	//Create and Initialize the Background
	m_pMainMenu = shared_ptr<Static2D>(new Static2D);
	if(!m_pMainMenu)
		return false;
	result = m_pMainMenu->Initialize(m_pD3D->GetDevice(), StringManager::MainMenu(), Global::GetScreenWidth(), Global::GetScreenHeight(), XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	//Change the Cursor color
	Global::GetCursor()->SetColor(Color::Yellow());
	
	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-480 * ASPECT.x, 250 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Campaign", 10, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-480 * ASPECT.x, 100 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Level Select", 15, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-480 * ASPECT.x, -50 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Level Editor", 20, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[3]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-480 * ASPECT.x, -200 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[3]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Bundle", 10, Color::White());
	if(!result)
		return false;

	//Set the Button Callback Methods
	m_pButtons[0]->SetFunction(std::bind(&MainMenu::SetLevel, this, _1), 2);//Campaign
	m_pButtons[1]->SetFunction(std::bind(&MainMenu::SelectLevel, this, _1), 0);//Level Select
	m_pButtons[2]->SetFunction(std::bind(&MainMenu::SetLevel, this, _1), 1);//Editor
	m_pButtons[3]->SetFunction(std::bind(&MainMenu::SelectLevel, this, _1), 1);//Bundle Select

	//Create and Initialize Level Select
	m_pLevel = shared_ptr<LevelSelect>(new LevelSelect);
	result = m_pLevel->Initialize(m_pD3D->GetDevice(), m_pFont, StringManager::CustomLevelDirectory(), std::bind(&MainMenu::SetLevel, this, _1), 4, std::bind(&MainMenu::Back, this));
	if(!result)
		return false;

	//Create and Initialize Bundle Select
	m_pBundle = shared_ptr<BundleSelect>(new BundleSelect);
	result = m_pBundle->Initialize(m_pD3D->GetDevice(), m_pFont, StringManager::BundleDirectory(), std::bind(&MainMenu::SetLevel, this, _1), 3, std::bind(&MainMenu::Back, this));
	if(!result)
		return false;

	return true;
}

void MainMenu::Update(float gameTime)
{
	//Update base on the Menu State
	switch(e_mainMenu)
	{
	case MAIN:
		for(int c = 0; c < m_pButtons.size(); c++)
			m_pButtons[c]->Update(gameTime, Global::GetCursor()->GetBox());
		break;
	case CUSTOM: m_pLevel->Update(gameTime); break;
	case BUNDLE: m_pBundle->Update(gameTime); break;
	}
}

bool MainMenu::Render()
{
	XMFLOAT4X4 world, view, ortho;
	bool result;

	BaseLevel::BeginScene();

	//Get the world, view, and projection Matrixes form the camera amd D3D objects
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetWorldMatrix(world);
	m_pD3D->GetOrthoMatrix(ortho);

	//Render based on the menu state
	switch(e_mainMenu)
	{
		case MAIN: result = Render(world, view, ortho); break;
		case CUSTOM: result = m_pLevel->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho); break;
		case BUNDLE: result = m_pBundle->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho); break;
	}
	if(!result)
		return false;

	BaseLevel::EndScene();

	return true;
}

bool MainMenu::Render(XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = m_pMainMenu->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
	if(!result)
		return false;

	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);

	return true;
}

void MainMenu::Shutdown()
{
	//Release the Font Object
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}

	//Release the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release the Text 
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();

	//Release the Background
	if(m_pMainMenu)
	{
		m_pMainMenu.reset();
		m_pMainMenu = 0;
	}

	//Release the Bundle Select
	if(m_pBundle)
	{
		m_pBundle.reset();
		m_pBundle = 0;
	}

	//Release the Level Select
	if(m_pLevel)
	{
		m_pLevel.reset();
		m_pLevel = 0;
	}
}

//Set the next level
void MainMenu::SetLevel(int value)
{
	m_levelSet = true;
	m_levelIndex = value;
}

//Is the level set?
bool MainMenu::LevelSet()
{
	return m_levelSet;
}
//Get the level index
int MainMenu::GetLevel()
{
	return m_levelIndex;
}

//Get the Selected Level File Path
string MainMenu::GetSelectedLevel()
{
	return m_pLevel->GetLevel();
}

//Get the Selected Bundle File Path
string MainMenu::GetSelectedBundle()
{
	return m_pBundle->GetLevel();
}

//Check the menu state
bool MainMenu::GetMenuState()
{
	return (e_mainMenu == MAIN);
}

//Set the Next Menu State
void MainMenu::SelectLevel(int value)
{
	switch(value)
	{
	case 0:	e_mainMenu = CUSTOM; break;
	case 1: e_mainMenu = BUNDLE; break;
	}
}

void MainMenu::Back()
{
	e_mainMenu = MAIN;
}