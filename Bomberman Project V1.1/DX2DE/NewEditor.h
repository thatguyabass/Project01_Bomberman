//--------------------------------------------------------------
//NewEditor.h
//--------------------------------------------------------------
//Gather data for new level creation

#ifndef _NEW_EDITOR_H_
#define _NEW_EDITOR_H_

#include "TextBox.h"
#include "Button.h"

#include "text.h"

//--------------------------------------------------------------------------------------

class NewEditor
{
public:
	NewEditor();
	~NewEditor();


	bool Initialize(ID3D11Device*, shared_ptr<Font>, function<void(int)> submitCallback);
	void Update(float, BoundingBox*);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	
	//Get the level info - this is where the X - Y values are increased by 2
	void GetLevelInfo(string&, int&, int&);

private:
	//Methods
	void Shutdown();

	//Cleared to create level
	bool GetComplete();

	//Gather the Data to create the level
	void ExtractData();

	//Variables
	vector<shared_ptr<TextBox>> m_pTextBox;
	shared_ptr<Button> m_pSubmit;
	vector<shared_ptr<Text>> m_pText;

	string m_name;
	int m_X;
	int m_Y;

};

#endif//_NEW_EDITOR_H_