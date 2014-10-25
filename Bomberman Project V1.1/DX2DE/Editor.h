//--------------------------------------------------------
//Editor.h
//--------------------------------------------------------
//This is the initial step in creating a level
//This class will ask the player for the information before starting the creation of the level itself
//this is also the class where the player will be able to choose which level they want to edit or if they wish to create a new one

#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "NewEditor.h"
#include "OldEditor.h"
#include "LevelEditor.h"
#include "BundleEditor.h"
#include "baselevel.h"

#include "EditorCamera.h"
#include "Global.h"

#include "font.h"

//---------------------------------------------------------------------------------------

//Quick camera access
#define EDITOR_CAMERA dynamic_cast<EditorCamera*>(m_pCamera.get())

class Editor : public BaseLevel
{
public:
	Editor();
	~Editor();

	bool Initialize(shared_ptr<D3D>, wstring = NULL);
	void Update(float);
	bool Render();

	//Editor Menu States
	enum EditorState { INITIAL, NEW, OLD , LEVEL, BUNDLE };

private:
	//Shutdown
	void Shutdown();
	bool InitializeFont();//Create the font objects

	void GetLevelData();//Call this method to start the level creation process
	void InitializeLevel(string, int, int);//Create a new level
	void InitializeLevel(string, LevelInfo*);//Create an existing level

	void InitializeCamera();//Initialize the Editor Camera - Override
	void UpdateCamera(float gameTime);//Update the Editor Camera - Override

	void Update(float, BoundingBox*);//Update the Editor
	void UpdateLevel(float, XMFLOAT2);//Method to keep updat neat

	bool Render(XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	bool RenderLevel(XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);//Method to keep render neat

	void SetState(int);//Set the editor state
	void BackPressed();//Back Method callback
	void ReturnToMenu();//Return to the Main Menu of the application

	void SetCameraPosition();//Reset the camera position to be used in the menus

	//States
	EditorState e_editor;

	//Variables
	shared_ptr<Font> m_pFont;

	vector<shared_ptr<Text>> m_pText;
	vector<shared_ptr<Button>> m_pButtons;
	shared_ptr<Button> m_pBack;

	shared_ptr<OldEditor> m_pOld;
	shared_ptr<NewEditor> m_pNew;
	shared_ptr<LevelEditor> m_pLevel;
	shared_ptr<BundleEditor> m_pBundle;
	shared_ptr<Static2D> m_pBackground;

};

#endif//_EDITOR_H_