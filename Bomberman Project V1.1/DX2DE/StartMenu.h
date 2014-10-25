//------------------------------------------------------------------
//StartMenu.h
//------------------------------------------------------------------
//Base class for this applications start menus

#ifndef _START_MENU_H_
#define _START_MENU_H_

#include "Button.h"
#include "static2D.h"

#include "baselevel.h"
#include "Editor.h"

//--------------------------------------------------------------------------------------

class StartMenu
{
public:
	StartMenu();
	~StartMenu();

	virtual bool Initialize();
	//gametime, bounding box, offset
	void Update(float, BoundingBox*, XMFLOAT2);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	void Quit();//Quit the application
	void Return();//Return to the current game type
	void ToggleMenu();//Return to the main menu

protected:
	//Methods
	virtual void Shutdown();

	void FlagReset();//Hard flag reset

	//Variables
	shared_ptr<Static2D> m_pBackground;
	vector<shared_ptr<Button>> m_pButtons;

};

#endif//_START_MENU_H_