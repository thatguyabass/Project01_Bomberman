//--------------------------------
//systemclass.cpp
//--------------------------------
//Define the class SystemClass

#include "systemclass.h"

SystemClass::SystemClass()
{
	m_pD3D = 0;
	m_pLevel = 0;
}

SystemClass::~SystemClass()
{
	Shutdown();
}

bool SystemClass::Initialize()
{
	bool result;
	
	//Read the Configuation file
	INIReader reader = INIReader(StringManager::ConfigPath());
	int screenWidth = reader.GetInt(StringManager::ConfigGraphicSection(), StringManager::ConfigWidth(), 1280);
	int screenHeight = reader.GetInt(StringManager::ConfigGraphicSection(), StringManager::ConfigHeight(), 800);
	m_fullscreen = reader.GetBool(StringManager::ConfigGraphicSection(), StringManager::ConfigFullscreen(), false); 
	m_vSync = reader.GetBool(StringManager::ConfigGraphicSection(), StringManager::ConfigVSync(), false);
	bool godMode = reader.GetBool(StringManager::ConfigGodMode(), StringManager::ConfigGodMode(), false);

	Global::SetScreenWidth(screenWidth);
	Global::SetScreenHeight(screenHeight);
	Global::SetGodMode(godMode);

	//Initialize the windows api
	InitializeWindows();
	
	//Create Time
	m_pTime = shared_ptr<Time>(new Time);
	m_pTime->Initialize(m_hwnd);

	//Create FPS tracker
	m_pFPS = shared_ptr<FPS>(new FPS);
	m_pFPS->Initialize();

	//Create D3D Object
	m_pD3D = shared_ptr<D3D>(new D3D);
	if(!m_pD3D)
		return false;

	//Initialize the D3D object
	result = m_pD3D->Initialize(m_vSync, m_fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(m_hwnd, L"Error Initializing the Direct 3D object", L"Error", MB_OK);
		return false;
	}

	//Initialize the Input
	result = Input::Initialize(Global::GetScreenWidth(), Global::GetScreenHeight(), m_hwnd);
	if(!result)
	{
		MessageBox(m_hwnd, L"Error Initializing Input", L"Error", MB_OK);
		return false;
	}

	//Initialize Level Manager
	m_pLevel = shared_ptr<LevelManager>(new LevelManager);
	result = m_pLevel->Initialize(m_pD3D);
	if(!result)
	{
		MessageBox(m_hwnd, L"Initialize Failed", L"Error", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	ShutdownWindows();

	//Release the Level Manager
	if(m_pLevel)
	{
		m_pLevel.reset();
		m_pLevel = 0;
	}

	//Relase the D3D
	if(m_pD3D)
	{
		m_pD3D.reset();
		m_pD3D = 0;
	}

	//Release Time
	if(m_pTime)
	{
		m_pTime.reset();
		m_pTime = 0;
	}

	//Release FPS
	if(m_pFPS)
	{
		m_pFPS.reset();
		m_pFPS = 0;
	}

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done = false;
	bool result;

	//Initialize Message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message from the window or user
	while(!done)
	{
		//Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signals to end the application
		if(msg.message == WM_QUIT)
			done = true;
		else
		{
			//Update the Application
			result = Update();
			if(!result)
				done = true;

			//Render Application
			result = Render();
			if(!result)
				done = true;
		}	

		//Check to see if the user wants to exit the application
		if(m_pLevel->QuitApplication())
			done = true;
	}
}

bool SystemClass::Update()
{
	bool result;

	//Find which window is in focus
	HWND hwnd = GetForegroundWindow();

	//Compare that the application window
	if(m_hwnd == hwnd)
	{
		//Update Time
		m_pTime->Update();
		//Update FPS
		m_pFPS->Update();

		Input::Update();

		//Update Current Level
		m_pLevel->Update(m_pTime->DeltaTime());
	}
	else//Stop the timer from counting correctly to prevent a time difference between time sensitive objects
		m_pTime->LostFocus();

	return true;
}

bool SystemClass::Render()
{
	bool result;

	//Set the window color and clear the render views
	m_pD3D->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	//Render the Current Level
	result = m_pLevel->Render();
	if(!result)
	{
		MessageBox(m_hwnd, L"A Render Object Has Failed", L"Error", MB_OK);
		return false;
	}

	//Present the rendered scene to the screen
	m_pD3D->EndScene();
	
	return true;
}

void SystemClass::InitializeWindows()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;
	//Initialize the Screen Width and Height
	int width = 0;
	int height = 0;

	//Get an external pointer to this object
	ApplicationHandle = this;

	//Get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//Give the application a name
	m_applicationName = L"Bomberman";

	//Setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = (HICON)LoadImage(m_hinstance, MAKEINTRESOURCE(IDI_BOMB), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop screen.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings depending on whether it is running in full screen or in windowed mode
	if(m_fullscreen)
	{
		//if full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		Util::SetAspectRatio(dmScreenSettings.dmPelsWidth, dmScreenSettings.dmPelsHeight);

		//set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		//if windowed the set it to a supported resolution
		width = Global::GetScreenWidth();
		height = Global::GetScreenHeight();

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
		Util::SetAspectRatio(width, height);
	}

	//Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW, posX,posY, width, height, NULL, NULL, m_hinstance, NULL);

	//Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide the Mouse Cursor
	ShowCursor(false);

	//Store the values for the application to use later
	Global::SetHWND(m_hwnd);

	return;
}

void SystemClass::ShutdownWindows()
{
	//Show mouse cursor
	ShowCursor(true);

	//Fix Display settings if leaving fuillscreen mode
	if(m_fullscreen)
		ChangeDisplaySettings(NULL, 0);

	//Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = 0;

	//Remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = 0;

	//Release the pointer to this class
	ApplicationHandle = 0;

	return;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//Check if window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}