//----------------------------------------------------
//Button.h
//----------------------------------------------------
//Define the button class to use boundingbox and mouse position
//Can be used as a trigger when no texture is provided

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "base2D.h"
#include "Input.h"

#include "text.h"

//Used for callbacks
#include<functional>
using namespace std;
using namespace std::placeholders;

//////---------------------------------------------------------------------------------------------------------------------------------------------

//0.25 seconds
#define INPUT_DELAY 250

//Press types determine how the button will react to input
enum PRESS_TYPE
{
	HOLD,//User can hold for repeated results after the input delay
	ONCE,//Will only process once for each new input - Holding will only result in one press
	RELEASE//Process when the input key is released
};

class Button : public Base2D
{
public:
	Button();
	~Button();

	//Set the base values for the button
	bool Initialize(ID3D11Device*, wstring*, int, int, XMFLOAT2, PRESS_TYPE, float boxMod = 1.0f);
	void Update(float, BoundingBox* = 0);	//Update will detect if input is provided
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Call this methods to add text to a button
	bool InitializeText(ID3D11Device*, shared_ptr<Font>, string, int, XMFLOAT4, float fontScale = 1);
	void SetInputDelay(int);	//Manually Set the input delay

	//Set the Callback function - this is the methods that will be called when the button is pressed
	void SetFunction(std::function<void(void)>);
	void SetFunction(std::function<void(int)>, int);
	void ResetFunctionValue(int);	//Reset the value the int Function will send

	//Access Methods for text
	void SetText(string);
	string GetText();

	//Movement methods
	void Move(float d, XMFLOAT2 dir);
	void AdjustPos(XMFLOAT2);

	//Set the objects offset - Allows for proper position after movement
	void SetOffset(XMFLOAT2);

private:
	//Methods
	void Shutdown();
	//Press when no bounding box is sent via update - uses mouse base position 
	void Pressed();
	//Press event when a bounding box is provided
	void Pressed(BoundingBox*);

	//Reposition the text to the center of the button - when SetText is called
	void RepositionText();

	//Update the pressed method based on the buttons PRESS_TYPE
	bool GetPressType(int);
	//Call the buttons method
	void ExecuteCallback();
	
	//States
	PRESS_TYPE e_press;

	//Variables
	std::function<void(int)> m_funcInt;
	std::function<void(void)> m_funcVoid;
	int m_funcIntValue;

	shared_ptr<Text> m_pText;

	float m_timer;
	bool m_delay;
	int m_inputDelay;

};

#endif _BUTTON_H_