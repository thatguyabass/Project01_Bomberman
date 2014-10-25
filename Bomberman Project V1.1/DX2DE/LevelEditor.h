//-------------------------------------------------------------------------------------
//LevelEditor.h
//-------------------------------------------------------------------------------------
//This class will be the Main Level Editor class 
//This class will be responsible for Setting up the level environment, Enemy locations and Enemy waypoints


#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "EditorGrid.h"
#include "LevelEditorEnvironment.h"
#include "LevelEditorEnemies.h"
#include "Global.h"

#include "MessagePrompt.h"

//------------------------------------------------------------------------------------------------------------------------

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();
	
	//Create a level from scratch
	//Device, level name, Node Count wide, Noed count hight, font, Back Callback
	bool Initialize(ID3D11Device*, string, int, int, shared_ptr<Font>, function<void(void)>);

	//Create a level from a pre-existing level
	//Device, level name, level info, font, backcallback
	bool Initialize(ID3D11Device*, string, LevelInfo*, shared_ptr<Font>, function<void(void)>);
	void Update(float, XMFLOAT2, BoundingBox*);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Editor States
	enum Editor{ LEVEL, ENEMIES, WAYPOINTS };
	//Submit States
	enum SUBMIT_STATE{ NONE, SUMBIT_INITIALIZE, SUBMIT_SUCCESS, SUBMIT_FAILED };

	//Total Level Dimensions
	XMFLOAT2 GetGridDimensions();

private:
	//Methods
	//Base Initialize
	bool Initialize(ID3D11Device*, shared_ptr<Font>, function<void(void)>);
	void Shutdown();

	//Set the editor State
	void SetState(int);

	//Update the Camera Movement
	void UpdateCamera(float);

	//Submit the level and write it to a text file
	void Submit();

	//States
	Editor e_state;
	SUBMIT_STATE e_submit;

	//Variables
	string m_name;
	int m_X;
	int m_Y;

	//Use to visualize the currently selected button
	shared_ptr<Static2D> m_pToggle;

	//Menu buttons
	vector<shared_ptr<Button>> m_pButtons;

	shared_ptr<Static2D> m_pBackground;
	//Objects that will execute the Spesific editor behaviour
	shared_ptr<LevEdEnvironment> m_pEnviro;	//Modify the level
	shared_ptr<LevEdEnemies> m_pEnemies;	//Modify the Enemy Navigation

	//Level being modified by the user
	shared_ptr<EditorGrid> m_pGrid;
	shared_ptr<BoundingBox> m_pBox;

	shared_ptr<MessagePrompt> m_pPrompt;

};

#endif//_LEVEL_EDITOR_H_