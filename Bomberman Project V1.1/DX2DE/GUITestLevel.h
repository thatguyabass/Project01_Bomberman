//---------------------------------------------------------------------------
//GUITestLevel.h
//---------------------------------------------------------------------------
//This level will test the GUI tools

#ifndef _GUI_TEST_LEVEL_H_
#define _GUI_TEST_LEVEL_H_

#include "baselevel.h"

#include "LevelBundle.h"
#include "DynamicDropdown.h"
#include "Button.h"

#include "text.h"
#include "font.h"

#include "Global.h"

#include "TextBox.h"

class Test : public BaseLevel
{
public:
	Test();
	~Test();

	bool Initialize(shared_ptr<D3D>, wstring, bool);
	void Update(float);
	bool Render();

	void TestFunction(int value);

private:
	vector<shared_ptr<Text>> m_pText;
	vector<string> str;

	shared_ptr<Font> m_pFont;

	shared_ptr<DynamicDropdown> m_pDrop;
	shared_ptr<LevelBundle> m_pBundle;
	shared_ptr<Button> m_pButton;
	
	shared_ptr<TextBox> m_pTextBox;
	shared_ptr<TextBox> m_pTextBox1;

	shared_ptr<Text> m_dropTest;
	string m_string;

};

#endif//_GUI_TEST_LEVEL_H_