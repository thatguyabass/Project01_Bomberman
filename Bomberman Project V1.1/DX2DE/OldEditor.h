//-----------------------------------------------------------------------------
//OldEditor.h
//-----------------------------------------------------------------------------
//Handle the editing of an already existing level and recreate it for the user

#ifndef _OLD_EDITOR_H_
#define _OLD_EDITOR_H_

#include "AutoDropdown.h"
#include "Button.h"
#include "Global.h"
#include "LevelInfo.h"

//---------------------------------------------------------------------------------------------------

class OldEditor
{
public:
	OldEditor();
	~OldEditor();

	bool Initialize(ID3D11Device*, shared_ptr<Font>, function<void(int)> submitCallback);
	void Update(float, BoundingBox*);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Get the information required for creating a level
	void GetLevelData(string&, LevelInfo&);

private:
	//Methods
	void Shutdown();

	//Mark the level ready for editing
	void InitializeLevel(int);
	//Check if the level is ready to be edited
	bool Complete();

	//Read the file to display more information to the user
	bool ReadFile(wstring);

	//Variables
	shared_ptr<AutoDropdown> m_pDrop;
	shared_ptr<Button> m_pButton;
	vector<shared_ptr<Text>> m_pText;

	string m_level;

};

#endif//_OLD_EDITOR_H_