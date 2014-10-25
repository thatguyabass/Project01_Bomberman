//----------------------------------------------------
//fire.h
//----------------------------------------------------
//This class will handle an individual fire object
//This class is only used as an visual effect

#ifndef _FIRE_H_
#define _FIRE_H_

#include "base2D.h"

class Fire : public Base2D
{
public:
	Fire();
	Fire(const Fire&);
	~Fire();

	bool Initialize(ID3D11Device*, XMFLOAT2 imageSize, XMFLOAT2 position);
	void Update(float gameTime);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

private:
	//Methods
	void Shutdown();
};


#endif//_FIRE_H_