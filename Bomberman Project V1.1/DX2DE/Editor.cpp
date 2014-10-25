//-----------------------------------------------------------------
//Editor.cpp
//-----------------------------------------------------------------
//

#include "Editor.h"

Editor::Editor()
{
	e_editor = INITIAL;
	m_pLevel = NULL;
}

Editor::~Editor()
{
	Shutdown();
}

bool Editor::Initialize(shared_ptr<D3D> d3d, wstring level)
{
	bool result;

	//Set the Global Game State
	Util::e_globalGame = Util::LEVEL_EDITOR;

	//Base Initialize
	result = BaseLevel::Initialize(d3d);
	if(!result)
		return false;

	//Create and Initialize the Font Objects
	result = InitializeFont();
	if(!result)
		return false;
	
	//Set the Cursor to a new color
	Global::GetCursor()->SetColor(Color::LightBlue());

	//Create and Initialize the Old Editor Flow
	m_pOld = shared_ptr<OldEditor>(new OldEditor);
	result = m_pOld->Initialize(m_pD3D->GetDevice(), m_pFont, std::bind(&Editor::SetState, this, _1));
	if(!result)
		return false;

	//Create and Initialize the New Editor Flow
	m_pNew = shared_ptr<NewEditor>(new NewEditor);
	result = m_pNew->Initialize(m_pD3D->GetDevice(), m_pFont, std::bind(&Editor::SetState, this, _1));
	if(!result)
		return false;

	//Create and Initialize the Bundle Editor
	m_pBundle = shared_ptr<BundleEditor>(new BundleEditor);
	result = m_pBundle->Initialize(m_pD3D->GetDevice(), m_pFont);
	if(!result)
		return false;
	
	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-300 * ASPECT.x, 150 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[0]->InitializeText(m_pD3D->GetDevice(), m_pFont, "New Level", 10, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200,100, XMFLOAT2(-300 * ASPECT.x, 0), ONCE);
	if(!result)
		return false;
	result = m_pButtons[1]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Edit Level", 12, Color::White());
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 200, 100, XMFLOAT2(-300 * ASPECT.x, -150 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pButtons[2]->InitializeText(m_pD3D->GetDevice(), m_pFont, "Bundle Editor", 25, Color::White());
	if(!result)
		return false;

	//Set the Buttons Callback Methods
	m_pButtons[0]->SetFunction(std::bind(&Editor::SetState, this, _1), NEW);
	m_pButtons[1]->SetFunction(std::bind(&Editor::SetState, this, _1), OLD);
	m_pButtons[2]->SetFunction(std::bind(&Editor::SetState, this, _1), BUNDLE);

	//Create and Initialize the Back Button
	m_pBack = shared_ptr<Button>(new Button);
	result = m_pBack->Initialize(m_pD3D->GetDevice(), &StringManager::GUIBlank(), 150,75, XMFLOAT2(-490 * ASPECT.x, -300 * ASPECT.y), ONCE);
	if(!result)
		return false;
	result = m_pBack->InitializeText(m_pD3D->GetDevice(), m_pFont, "Back", 5, Color::Green());
	if(!result)
		return false;

	//Set the back callback
	m_pBack->SetFunction(std::bind(&Editor::BackPressed, this));

	//Create and Initialize the Background
	m_pBackground = shared_ptr<Static2D>(new Static2D);
	result = m_pBackground->Initialize(m_pD3D->GetDevice(), StringManager::Continue(), Global::GetScreenWidth(), Global::GetScreenHeight(), XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	return true;
}

void Editor::Update(float gameTime)
{
	//Get the Camera position
	XMFLOAT2 offset = m_pCamera->GetPosition2D();
	
	//Only update the back button when in a menu state
	if(e_editor != LEVEL)
		m_pBack->Update(gameTime, Global::GetCursor()->GetBox());

	//Update the editor based on the Editor State
	switch(e_editor)
	{
	case INITIAL: Update(gameTime, Global::GetCursor()->GetBox()); break;
	case NEW: m_pNew->Update(gameTime, Global::GetCursor()->GetBox()); break;
	case OLD: m_pOld->Update(gameTime, Global::GetCursor()->GetBox()); break;
	case LEVEL: UpdateLevel(gameTime, offset); break;
	case BUNDLE: m_pBundle->Update(gameTime, Global::GetCursor()->GetBox()); break;
	}
}

bool Editor::Render()
{
	XMFLOAT4X4 world, view, ortho;
	bool result;

	//Get the Matrixs
	m_pCamera->GetViewMatrix(view);
	m_pD3D->GetOrthoMatrix(ortho);
	m_pD3D->GetWorldMatrix(world);

	BaseLevel::BeginScene();

	if(e_editor != LEVEL)
	{
		result = m_pBackground->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	//Render based on the current editor state
	switch(e_editor)
	{
	case INITIAL: Render(world, view, ortho); break;
	case NEW: m_pNew->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho); break;
	case OLD: m_pOld->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho); break;
	case LEVEL: RenderLevel(world, view, ortho); break;
	case BUNDLE: m_pBundle->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho); break;
	}

	if(e_editor != LEVEL)
	{
		result = m_pBack->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	BaseLevel::EndScene();

	return true;
}

void Editor::Shutdown()
{
	//Release Font
	if(m_pFont)
	{
		m_pFont.reset();
		m_pFont = 0;
	}

	//Release and clear the Text Vector
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();

	//Release and Clear the Buttons Vector
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();
	
	//Release the back button
	if(m_pBack)
	{
		m_pBack.reset();
		m_pBack = 0;
	}

	//Release the Old Editor Flow
	if(m_pOld)
	{
		m_pOld.reset();
		m_pOld = 0;
	}

	//Release the New Editor Flow
	if(m_pNew)
	{
		m_pNew.reset();
		m_pNew = 0;
	}
	
	//Release the Level Editor
	if(m_pLevel)
	{
		m_pLevel.reset();
		m_pLevel = 0;
	}

	//Release the Level Bundle Editor
	if(m_pBundle)
	{
		m_pBundle.reset();
		m_pBundle = 0;
	}

	//Release the Background
	if(m_pBackground)
	{
		m_pBackground.reset();
		m_pBackground = 0;
	}
}

//Create and Initialize the Font
bool Editor::InitializeFont()
{
	bool result;

	m_pFont = shared_ptr<Font>(new Font);
	result = m_pFont->Initialize(m_pD3D->GetDevice(), StringManager::FontText(), StringManager::FontTexture());
	if(!result)
		return false;

	return true;
}

//Override the Initialize Camera to create the editor camera
void Editor::InitializeCamera()
{
	int width = Global::GetScreenWidth();
	int height = Global::GetScreenHeight();

	//Create and Initialize the Editor Camera
	m_pCamera = shared_ptr<EditorCamera>(new EditorCamera);
	dynamic_cast<EditorCamera*>(m_pCamera.get())->Initialize();
	m_pCamera->SetPosition(0.0f, 0.0f, -15.0f);
	m_pCamera->SetBounds(width, height);
}

//Create the level using the data collected
void Editor::GetLevelData()
{	
	string name;

	//Based on the current editor state, create the level
	switch(e_editor)
	{
		case NEW://Create a new level
		{
			int x;
			int y;
			m_pNew->GetLevelInfo(name, x, y);
			InitializeLevel(name, x, y);
		}
		break;
	case OLD://Recreate an existing level
		{
			LevelInfo level;
			m_pOld->GetLevelData(name, level);
			InitializeLevel(name, &level);
		}
		break;
	}

	//Set the cameras position to the center of the created level
	XMFLOAT2 d = m_pLevel->GetGridDimensions();
	m_pCamera->SetPosition(d.x / 2, d.y / 2);
}

//Create and initialize a level using the new level flow
void Editor::InitializeLevel(string name, int x, int y)
{
	m_pLevel = shared_ptr<LevelEditor>(new LevelEditor);
	m_pLevel->Initialize(m_pD3D->GetDevice(), name, x, y, m_pFont, std::bind(&Editor::BackPressed, this));
}
//Create and initialize a level using the old level flow
void Editor::InitializeLevel(string name, LevelInfo* level)
{
	m_pLevel = shared_ptr<LevelEditor>(new LevelEditor);
	m_pLevel->Initialize(m_pD3D->GetDevice(), name, level, m_pFont, std::bind(&Editor::BackPressed, this));
}

//Update initialize menu buttons
void Editor::Update(float gameTime, BoundingBox* box)
{
	for(int c = 0; c < m_pButtons.size(); c++)
		m_pButtons[c]->Update(gameTime, box);
}

//Update the editor camera
void Editor::UpdateCamera(float gameTime)
{
	dynamic_cast<EditorCamera*>(m_pCamera.get())->Update(gameTime);
}

//Update the level editor and editor camera
void Editor::UpdateLevel(float gameTime, XMFLOAT2 offset)
{
	if(m_pLevel)
	{
		m_pLevel->Update(gameTime, offset, Global::GetCursor()->GetBox());
		UpdateCamera(gameTime);
	}
}

//Render the Initial Menu
bool Editor::Render(XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Render the level editor
bool Editor::RenderLevel(XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	if(m_pLevel)
	{
		result = m_pLevel->Render(m_pD3D->GetDeviceContext(), m_pTextureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

//Set the Editor State
void Editor::SetState(int value)
{
	switch(value)
	{
	case 0: e_editor = INITIAL; break;
	case 1: e_editor = NEW;		break;
	case 2: e_editor = OLD;		break;
	case 3:
		if(m_pLevel)
			m_pLevel.reset();

		GetLevelData();//Create the level based on the Old or New flow
		e_editor = LEVEL;	
		break;
	case 4: e_editor = BUNDLE; break;
	}
}

//Move back in the menu flow
void Editor::BackPressed()
{
	//If in any state other then the Initial state, return to the initial state and reset the camera position
	//Else return to the Main Menu
	switch(e_editor)
	{
	case INITIAL: ReturnToMenu(); break;
	case NEW:
	case OLD:
	case LEVEL:
	case BUNDLE:
	default:
		SetCameraPosition();
		e_editor = INITIAL;
		break;
	}
}

//Return to the main menu
void Editor::ReturnToMenu()
{
	FLAG_BACK = true;
	Util::ToggleMenu();
}

//Reset the camera position to be used in the menus
void Editor::SetCameraPosition()
{
	m_pCamera->SetPosition(0, 0, -15);
}
