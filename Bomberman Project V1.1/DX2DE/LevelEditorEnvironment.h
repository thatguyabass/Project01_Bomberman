//-------------------------------------------------------
//LevelEditorEnvironment.h
//-------------------------------------------------------
//Handle the manipulation of the level gird to create an enrivonment for players

#ifndef _LEVEL_EDITOR_ENVIRONMENT_H_
#define _LEVEL_EDITOR_ENVIRONMENT_H_

#include "Button.h"

#include "static2D.h"
#include "font.h"
#include "text.h"

#include<vector>

//-------------------------------------------------------------------------------------------------------------------------

class LevEdEnvironment
{
public:
	LevEdEnvironment();
	~LevEdEnvironment();

	bool Initialize(ID3D11Device*, shared_ptr<Font>);
	void Update(float, XMFLOAT2);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	int GetState();

	//Level States
	enum LEVEL_STATE{ NONE, SPACE, NO_SPACE, DESTRUCTIBLE, EXIT, SPAWN, HELI, YELLOW, MAD_GUY, MAD_BOMB, RABBIT, BLUE_EYE };

private:
	//Methods
	void Shutdown();
	//Set the Level State
	void SetState(int);

	//States
	LEVEL_STATE e_level;

	//Variables
	vector<shared_ptr<Button>> m_pButtons;
	vector<shared_ptr<Text>> m_pText;
	shared_ptr<Static2D> m_pToggle;

};

#endif//_LEVEL_EDITOR_ENVIRONMENT_H_