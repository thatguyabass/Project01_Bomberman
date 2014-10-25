//----------------------------------------------
//Temp.h
//----------------------------------------------
//Temp object to test collisions

#ifndef _TEMP_H_
#define _TEMP_H_

#include "base2D.h"
#include "input.h"

class Temp : public Base2D
{
public:
	Temp();
	Temp(const Temp&);
	~Temp();

	bool Initialize(ID3D11Device*, int screenWidth, int screenHeight, int imageWidth, int imageHeight, XMFLOAT2 pos, float rot);
	void Update(float gameTime);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	void Move(float d, XMFLOAT2 dir);
	void Rotate(float d);

	void CollisionWithWall(int side);

private:
	//Methods
	void Shutdown();
	
	//Variables
	float m_xV;
	float m_yV;

};

#endif//_TEMP_H_