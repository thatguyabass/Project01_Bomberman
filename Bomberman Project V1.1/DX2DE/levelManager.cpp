//--------------------------------------------
//levelManager.cpp
//--------------------------------------------
//define the Level Manager class

#include "levelManager.h"

LevelManager::LevelManager()
{
	//base values
	m_pD3D = 0;
	m_pCurrent = 0;

	e_levelType = MENU;
}

LevelManager::~LevelManager()
{
	Shutdown();
}

bool LevelManager::Initialize(shared_ptr<D3D> d3d)
{
	bool result;

	m_pD3D = d3d;

	//Initialize the Global Cursor
	result = Global::GetCursor()->Initialize(m_pD3D->GetDevice(), StringManager::Connection(), 10 * ASPECT.x, 10 * ASPECT.y);
	if(!result)
		return false;

	//Initialize the initial level - Main Menu
	result = GetTypeInitialize();
	if(!result)
		return false;

	//Create and Initialize the Menu Manager
	m_pMenu = shared_ptr<MenuManager>(new MenuManager);
	result = m_pMenu->Initialize(m_pD3D);
	if(!result)
		return false;

	//Create a level bundle
	m_pBundle = shared_ptr<LevelBundle>(new LevelBundle);

	return true;
}

void LevelManager::Update(float gameTime)
{
	//if(Util::e_globalGame != Util::GAME && Util::e_globalMenu == Util::CLOSED)
		m_pMenu->Update(gameTime, m_pCurrent->GetCameraOffset());

	//So long as the start menu isn't open - update the current level
	if(Util::e_globalMenu == Util::CLOSED)
		GetTypeUpdate(gameTime);

	//None menu Updates
	if(e_levelType != MENU)
	{
		if(e_levelType != EDITOR)
		{
			//True = Level Complete | false = Level in progress
			if(dynamic_cast<Level*>(m_pCurrent.get())->LevelStatus())
				SetContinueType();//Check which continue state the user selected
		}

		//Return to the main menu
		if(FLAG_BACK)
		{
			Util::ToggleMenu();
			MenuReturn();
		}
	}
}

bool LevelManager::Render()
{
	bool result;

	//Render the Current level
	result = GetTypeRender();
	if(!result)
		return false;

	//Get the View Matrix
	XMFLOAT4X4 view;
	m_pCurrent->GetViewMatrix(view);

	//Render the Start Menus
	result = m_pMenu->Render(view);
	if(!result)
		return false;

	return true;
}

void LevelManager::Shutdown()
{
	//Release the Current Level
	if(m_pCurrent)
	{
		m_pCurrent.reset();
		m_pCurrent = 0;
	}

	//Release the Direct 3D object Reference
	if(m_pD3D)
	{
		m_pD3D.reset();
		m_pD3D = 0;
	}

	//Release the Menu Manager
	if(m_pMenu)
	{
		m_pMenu.reset();
		m_pMenu = 0;
	}

	//Release the Level Bundle
	if(m_pBundle)
	{
		m_pBundle.reset();
		m_pBundle = 0;
	}
}


bool LevelManager::GetTypeInitialize()
{
	bool result;

	//Release the Current menu
	if(m_pCurrent)
	{
		m_pCurrent.reset();
		m_pCurrent = 0;
	}

	//Initialize the New Level using the Level State
	switch(e_levelType)
	{
	case MENU: result = InitializeMainMenu(); break;
	case EDITOR: result = InitializeEditor(); break;
	case LEVEL: result = InitializeLevel<Level>(m_pBundle->GetLevel(), m_pBundle->GetContinue()); break;
	case CUSTOM: result = InitializeLevel<Level>(m_levelPath); break;
	}
	if(!result)
		return false;

	return true;
}
void LevelManager::GetTypeUpdate(float gameTime)
{
	//Update based on the current level state
	switch(e_levelType)
	{	
	case MENU:
		UpdateLevel<MainMenu>(gameTime);//Update the Main Menu
		if(dynamic_cast<MainMenu*>(m_pCurrent.get())->LevelSet())//If a new level is set
			SetLevel(dynamic_cast<MainMenu*>(m_pCurrent.get())->GetLevel());//Change the Level State and Initialize the New level
		break;
	case EDITOR: UpdateLevel<Editor>(gameTime); break;
	case LEVEL:
	case CUSTOM: UpdateLevel<Level>(gameTime); break;
	}
}
bool LevelManager::GetTypeRender()
{
	bool result;

	//Render based on the Current Level State
	switch(e_levelType)
	{
	case MENU: result = RenderLevel<MainMenu>(); break;
	case EDITOR: result = RenderLevel<Editor>(); break;
	case LEVEL:
	case CUSTOM: result = RenderLevel<Level>(); break;
	}
	if(!result)
		return false;

	return true;
}

void LevelManager::SetContinueType()
{
	int state = dynamic_cast<Level*>(m_pCurrent.get())->GetContinueState();//Get the Continue State
	switch(state)//Call the method based on the continue state
	{
	case Continue::CONTINUE_STATE::NEXT: NextLevel(); break;
	case Continue::CONTINUE_STATE::QUIT: MenuReturn(); break;
	case Continue::CONTINUE_STATE::AGAIN: PlayAgain(); break;
	case Continue::CONTINUE_STATE::NONE:
		MessageBox(Global::GetHWND(), L"MAJOR ERROR I DON'T KNOW HOW YOU GOT HERE!!!!!", L"Error", MB_OK);
		FLAG_EXIT = true;
		break;
	}
}

//Increment the Level count in the Level Bundle and initialize the new level
void LevelManager::NextLevel()
{
	m_pBundle->NextLevel();

	GetTypeInitialize();
}

//Play the current level again
void LevelManager::PlayAgain()
{
	Stats::Reset();
	GetTypeInitialize();
}

//Set the Level State and initialize the level
void LevelManager::SetLevel(int index)
{
	switch(index)
	{
	case 1: e_levelType = EDITOR; break;
	case 2: InitializeBundle(StringManager::CampaignBundle()); break;
	case 3: CustomBundleFlow(); break;
	case 4: CustomLevelFlow(); break;
	}

	GetTypeInitialize();
}

//Return the User to the Main Menu
void LevelManager::MenuReturn()
{
	Stats::Reset();
	e_levelType = MENU;
	FLAG_BACK = false;
	GetTypeInitialize();
}

bool LevelManager::QuitApplication()
{
	return (Util::GetFlags().exit);
}

bool LevelManager::InitializeMainMenu()
{
	bool result;

	//Create Level
	m_pCurrent = shared_ptr<MainMenu>(new MainMenu);
	if(!m_pCurrent)
		return false;

	//Initialize Level
	result = dynamic_cast<MainMenu*>(m_pCurrent.get())->Initialize(m_pD3D);
	if(!result)
		return false;

	return true;
}

bool LevelManager::InitializeEditor()
{
	bool result;

	//Create Level
	m_pCurrent = shared_ptr<Editor>(new Editor);
	if(!m_pCurrent)
		return false;

	//Initialize Level
	result = dynamic_cast<Editor*>(m_pCurrent.get())->Initialize(m_pD3D, L"");
	if(!result)
		return false;

	return true;
}

void LevelManager::CustomLevelFlow()
{
	//Set the Level State
	e_levelType = CUSTOM;

	//Get the Level Name
	string path = dynamic_cast<MainMenu*>(m_pCurrent.get())->GetSelectedLevel();

	path = StringManager::CustomLevelDir() + path;//Create the full file path
	wstring level(path.begin(), path.end());//Convert to wstring
	m_levelPath = level;
}

void LevelManager::CustomBundleFlow()
{
	//Get the level name
	string path = dynamic_cast<MainMenu*>(m_pCurrent.get())->GetSelectedBundle();
	InitializeBundle(path);//Initialize the Level Bundle
}

void LevelManager::InitializeBundle(wstring dir)
{
	e_levelType = LEVEL;
	m_pBundle->Initialize(dir);
}
void LevelManager::InitializeBundle(string dir)
{
	e_levelType = LEVEL;
	m_pBundle->Initialize(dir);
}