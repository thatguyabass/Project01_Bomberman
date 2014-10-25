//--------------------------------------------
//static2D.h
//--------------------------------------------
//Basic Render Class that can be used for static assets

#ifndef _STATIC_2D_H_
#define _STATIC_2D_H_

#include "base2D.h"

class Static2D : public Base2D
{
public:
	Static2D();
	Static2D(const Static2D&);
	~Static2D();

	bool Initialize(ID3D11Device*, wstring texture, int imageWidth, int imageHeight, XMFLOAT2 pos, float rot);
	void Update(float);

private:

};

#endif//_STATIC_2D_H_

