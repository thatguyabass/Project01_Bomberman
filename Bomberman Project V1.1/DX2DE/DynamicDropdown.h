//-----------------------------------------------------------
//DynamicDropdown.h
//-----------------------------------------------------------
//Dynamicly add buttons to the dropdown in runtime. This class will also return each string to populate a text file with

#ifndef _DYNAMIC_DROPDOWN_H_
#define _DYNAMIC_DROPDOWN_H_

#include "AutoDropdown.h"
#include "Dropdown.h"

#include<vector>

//------------------------------------------------------------------------------------------------------------------------------

class DynamicDropdown : public Dropdown
{
public:
	DynamicDropdown();
	~DynamicDropdown();

	bool Initialize(ID3D11Device*, shared_ptr<Font>, wstring, int, int, XMFLOAT2, int, float BOX_MOD = 1.0f, float SCALE = 1.0f);
	void Update(float, BoundingBox* = NULL);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//States this class can enter - Only will change when adding buttons to the dropdown
	enum DYNAMIC_STATE { DYNAMIC, ADD };
	//Populate this class with buttons using a string vector
	void Populate(vector<wstring>&);

	//Returns the buttons strings based on which order they are in on the dropdown
	void GetStrings(vector<string>&);

	//Return the Dropdowns current state
	bool GetState();

private:
	//Methods
	void Shutdown();
	//Adjust the size of the background to accommodate the added buttons
	bool InitializeBackground(ID3D11Device*);

	//Swap the text between two values
	void Swap(shared_ptr<Button>&, shared_ptr<Button>&);
	//Internal callback to get the currently selected button
	void GetButtonIndex(int);

	//Toggle the state between Dynamic and Add
	void AddState();
	//Start the Add process after selecting a file from the AutoDropdown
	void StartAdd(int);	
	//Add the selected button to the drop down
	void Add();
	//Remove a button from the dropdown
	void Remove();

	//Change the buttons order 
	void MoveUp();
	void MoveDown();

	//Variables
	//Values used in creating dynamic buttons
	int m_buttonCount;
	int m_modWidth;
	int m_modHeight;
	float m_scale;
	//Used to determine if a button in the Dynamic Dropdown is selected
	bool m_selected;

	//Needed to create new buttons
	ID3D11Device* m_pDevice;

	//Current Selected button on the Add Auto Dropdown
	int m_currentSelected;
	//Add Auto Dropdown - this will be used to add new buttons to the Dynamic Dropdown
	shared_ptr<AutoDropdown> m_pDrop;
	shared_ptr<Static2D> m_pDropBack;
	//Buttons used Durring the Add State to add buttons and toggle the state
	vector<shared_ptr<Button>> m_pDropBut;	
	//button Modification buttons
	vector<shared_ptr<Button>> m_pModButtons;
	shared_ptr<Font> m_pFont;

	DYNAMIC_STATE e_dynamic;

};

#endif//_DYNAMIC_DROPDOWN_H_