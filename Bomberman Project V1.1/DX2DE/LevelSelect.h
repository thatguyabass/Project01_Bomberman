//------------------------------------------------------------------------
//LevelSelect.h
//------------------------------------------------------------------------
//Handles the selection procees for an individual level

#ifndef _LEVEL_SELECT_H_
#define _LEVEL_SELECT_H_

#include "Button.h"
#include "AutoDropdown.h"

#include "Global.h"

#include "text.h"
#include "font.h"

//------------------------------------------------------------------------------------------------

class LevelSelect
{
public:
	LevelSelect();
	~LevelSelect();
	//Level Callback will set the next level to be loaded when the Confirm button is selected
	virtual bool Initialize(ID3D11Device*, shared_ptr<Font>, wstring, function<void(int)> levelCallback, int callbackValue, function<void(void)> backCallback);
	virtual void Update(float);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	void Confirm();//Confirm selection
	bool LevelStatus();//Return the Confirmation state
	string GetLevel();//Return the selected level name

protected:
	//Methods
	void Shutdown();
	void GetCustomLevel(int);//Select and Gather the Level Name

	//Variables
	string m_level;
	bool m_confirm;

	shared_ptr<Text> m_pText;
	vector<shared_ptr<Button>> m_pButtons;
	shared_ptr<AutoDropdown> m_pDrop;
	shared_ptr<Static2D> m_pBackground;

};

#endif//_LEVEL_SELECT_H_