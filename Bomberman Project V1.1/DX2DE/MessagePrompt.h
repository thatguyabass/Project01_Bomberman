//----------------------------------------------------------------------------------
//MessagePrompt.h
//----------------------------------------------------------------------------------
//Define the Message Prompt Class
//This class will display a message to the user for a short time
//Error message - Confirmation message

#ifndef _MESSAGE_PROMPT_H_
#define _MESSAGE_PROMPT_H_

#include "static2D.h"

#include "text.h"
#include "font.h"

class MessagePrompt
{
public:
	MessagePrompt();
	~MessagePrompt();

	bool Initialize(ID3D11Device*, shared_ptr<Font>);
	void Update(float, XMFLOAT2 = XMFLOAT2(0,0));
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//display time, text, text color, position, background postion, text scale
	void Display(float, string, XMFLOAT4, XMFLOAT2, XMFLOAT2, float = 1.0f);

private:
	//Methods
	void Shutdown();

	//Variables
	//How long the display prompt will be on screen for
	float m_timer;
	float m_displayTime;
	bool m_display;

	shared_ptr<Text> m_pText;
	shared_ptr<Static2D> m_pBackground;

};

#endif//_MESSAGE_PROMPT_H_