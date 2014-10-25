//----------------------------------------------------------------
//Continue.h
//----------------------------------------------------------------
//Handle the players input when the a game level has been completed - either by beating the level or losing all their lives

#ifndef _CONTINUE_H_
#define _CONTINUE_H_

#include "Global.h"
#include "stats.h"
#include "static2D.h"
#include "Button.h"

//-----------------------------------------------------------------------------------------------------------

//When no state has been selected
#define NONE_INDEX 3

class Continue
{
public:
	Continue();
	~Continue();

	bool Initialize(ID3D11Device*, bool canContinue);
	void Update(float, XMFLOAT2);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Get the continue state
	int GetState();

	//State
	enum CONTINUE_STATE { NEXT, QUIT, AGAIN, NONE };

private:
	//Methods
	void Shutdown();
	void SetState(int);

	//Variables
	vector<shared_ptr<Button>> m_pButtons;
	shared_ptr<Static2D> m_pBackground;

	//State
	CONTINUE_STATE e_continue;

	//Default to 0 - set to one if the Next Buttons shouldn't apply
	int m_min;

};


#endif//_CONTINUE_H_