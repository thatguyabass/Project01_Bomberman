//---------------------------------------------------------------------
//BundleEditor.h
//---------------------------------------------------------------------
//Handle the Creation and Editing of level bundles
//The bundles are used to play level campaigns in sequence

#ifndef _BUNDLE_EDITOR_H_
#define _BUNDLE_EDITOR_H_

#include "TextBox.h"
#include "Button.h"
#include "DynamicDropdown.h"
#include "LevelBundle.h"
#include "MessagePrompt.h"

#include "text.h"

//----------------------------------------------------------------------------------------------------

class BundleEditor
{
public:
	BundleEditor();
	~BundleEditor();

	bool Initialize(ID3D11Device*, shared_ptr<Font>);
	void Update(float, BoundingBox*);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Edit State - Add is used when adding additional levels to the bundle
	enum EDIT_STATE { NONE, EDIT };

private:
	//Methods
	void Shutdown();

	//Write the bundle to a text file
	void SaveBundle();
	//Message displaying the status of the Write
	void SaveMessage(bool);
	//Edit a existing bundle
	void EditBundle();
	//Internal method to display the selected button in the auto dropdown and populate the dynamic dropdown
	void GetEditIndex(int);

	//Variables
	wstring m_levelPath;

	shared_ptr<TextBox> m_pBox;
	shared_ptr<LevelBundle> m_pBundle;
	//Auto dropdown used for the editing process
	shared_ptr<AutoDropdown> m_pAuto;
	//Use a Dynamic Dropdown to hold the levels before creating the bundle
	shared_ptr<DynamicDropdown> m_pDrop;
	vector<shared_ptr<Button>> m_pButtons;
	shared_ptr<Text> m_pText;
	shared_ptr<MessagePrompt> m_pPrompt;

	EDIT_STATE e_edit;

};

#endif//_BUNDLE_EDITOR_H_