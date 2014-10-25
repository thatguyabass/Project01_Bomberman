//--------------------------------------------------------------------------------------
//TextBox.h
//--------------------------------------------------------------------------------------
//

#ifndef _TEXT_BOX_H_
#define _TEXT_BOX_H_

#include "Button.h"
#include "static2D.h"

#include "text.h"

class TextBox
{
public:
	TextBox();
	~TextBox();

	//device, font, background width, background heigth, position, number input only, base string, max string length, scale
	bool Initialize(ID3D11Device*, shared_ptr<Font>, int imageWidth, int imageHeight, XMFLOAT2 pos, bool num, string text, int length, float scale = 1.0f);
	void Update(float, BoundingBox*);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	string GetText();

private:
	//Methods
	void Shutdown();
	//Focus Toggle
	void SetFocus(int);
	void HandleNumberInput();	//Process number input
	void HandleInput();			//Process input
	void AssignText(char);		//Assign the char value to the string
	void ReturnFocus(BoundingBox*);//Release focus

	//The background will strobe to indicate the player has focus
	void UpdateSign(float);

	//States
	enum FOCUS { IN_FOCUS, OUT_OF_FOCUS };
	FOCUS e_focus;

	//Variables
	shared_ptr<Static2D> m_pBackground;
	shared_ptr<Static2D> m_pTextArea;
	shared_ptr<Button> m_pButton;
	
	//background colors
	XMFLOAT4 m_color;
	XMFLOAT4 m_baseColor;

	//Strobe Values	
	float m_timer;
	const float BLINK_TIME;
	bool m_blink;

	shared_ptr<Text> m_pText;
	string m_text;

	int m_length;
	bool m_num;//Register only number input Flag

};

#endif//_TEXT_BOX_H_
