//-------------------------------
//systemclass.h
//-------------------------------
//System that will do all the heavy lifting

#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_

#define WIN32_LEAN_AND_MEAN

#include<Windows.h>
#include<ctime>
#include<cstdlib>

#include "d3d.h"
#include "time.h"
#include "fps.h"

#include "Input.h"
#include "Util.h"
#include "INIReader.h"
#include "stats.h"
#include "stringManager.h"
#include "levelManager.h"

#include "resource.h"

//--------------------------------------------------------------------------------------------------------------

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//System class updates and renders most of the base systems
class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	bool Initialize();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//Methods
	void Shutdown();	//Shutdown is called by the Deconstructor
	bool Update();
	bool Render();
	void InitializeWindows();
	void ShutdownWindows();

	//Variables
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	bool m_fullscreen;
	bool m_vSync;

	shared_ptr<D3D> m_pD3D;
	shared_ptr<Time> m_pTime;
	shared_ptr<FPS> m_pFPS;

	shared_ptr<LevelManager> m_pLevel;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;


#endif	//_SYSTEM_CLASS_H_