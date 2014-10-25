//---------------------------------------------------------------------------------
//Color.h
//---------------------------------------------------------------------------------
//Handles the Colors

#ifndef _COLOR_H_
#define _COLOR_H_

#include<xnamath.h>

static class Color
{
public:
	Color();
	~Color();

	static XMFLOAT4 Red() { return XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); }
	static XMFLOAT4 Green() { return XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); }
	static XMFLOAT4 Blue() { return XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f); }
	static XMFLOAT4 LightBlue() { return XMFLOAT4(0.67f, 0.85f, 0.9f, 1.0f); }
	static XMFLOAT4 White() { return XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); }
	static XMFLOAT4 Black() { return XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); }
	static XMFLOAT4 Teal() { return XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f); }
	static XMFLOAT4 Yellow() { return XMFLOAT4(1.0f, 1.0f, 0.35f, 1.0f); }
	static XMFLOAT4 Purple() { return XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f); }
	static XMFLOAT4 Gray() { return XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f); }
	static XMFLOAT4 LightGray() { return XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f); }

};

#endif//_COLOR_H_