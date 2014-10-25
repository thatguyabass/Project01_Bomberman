//-----------------------------------------------------------------------
//Cursor.h
//-----------------------------------------------------------------------
//Static texture that will move around the screen using the mouse position

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "base2D.h"
#include "Input.h"

class Cursor : public Base2D
{
public:
	Cursor();
	~Cursor();

	bool Initialize(ID3D11Device*, wstring, int, int);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Used for positioning after the view port moves
	void SetOffset(XMFLOAT2);

private:
	//Methods
	void Shutdown();

	//Variables
	XMFLOAT2 m_offset;

};

#endif//_CURSOR_H_