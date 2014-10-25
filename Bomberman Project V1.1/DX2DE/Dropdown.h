//-------------------------------------------------------------------------
//Dropdown.h
//-------------------------------------------------------------------------
//Base Dropdown functionality. This class will hold the values that every dropdown object will need and update and render the buttons that are added.

#ifndef _DROPDOWN_H_
#define _DROPDOWN_H_

#include "static2D.h"
#include "Global.h"
#include "Button.h"

class Dropdown
{
public:
	Dropdown();
	~Dropdown();

	//Initialize the Core values
	virtual bool Initialize(ID3D11Device*, int, int, XMFLOAT2, int);
	virtual void Update(float, BoundingBox*);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Get the string of the button pressed in the dropdown
	string GetString(int);
	
	//mark the current selected button with a visual element for the users
	void SetSelected(int, XMFLOAT4);

protected:
	//Methods
	virtual void Shutdown();
	//Override this function if the background needs to be larger then the the base
	virtual bool InitializeBackground(ID3D11Device*);

	//Movement functions for the buttons in the list
	void Arrow(int);
	void Adjust(int);
	void Move(float, XMFLOAT2);

	//Move the buttons down
	int IncreaseTop();
	//Move the buttons up
	void DecreaseTop();
	void SetScrollIcon();

	//Variables
	//GUI Button Specs
	int m_buttonWidth;
	int m_buttonHeight;
	int m_arrowWidth;
	int m_arrowHeight;

	//Base Position - the entire object is based off this position
	XMFLOAT2 m_position;

	//Position offset used in base button placement and Adjustment
	int m_offset;
	//The current index of the button that is on top of the dropdown
	int m_top;
	//Number of buttons displayed
	int m_display;
	int m_baseDisplay;
	//If the number of buttons isn't longer then display this can control weather the slider is shown
	bool m_short;

	//How many to move up or down
	int m_adjust;
	//Which was the last selected button
	int m_lastSelected;

	shared_ptr<Static2D> m_pBackground;
	//Slider used to navigate through the buttons
	shared_ptr<Static2D> m_pScrollIcon;
	shared_ptr<Static2D> m_pScrollBar;
	vector<shared_ptr<Button>> m_pArrows;
	
	//Buttons that are used for the file names
	vector<shared_ptr<Button>> m_pButtons;

};

#endif//_DROPDOWN_H_