//----------------------------------------------------------------------------
//Power.h
//----------------------------------------------------------------------------
//Class used for the Power ups dropped in the game. The player will be able to collect these to increase their power
//Currently Four power ups are available[Speed, Bomb Count, Explotion Spread, Increase Life]

#ifndef _POWER_UPS_H_
#define _POWER_UPS_H_

#include "base2D.h"
#include "stats.h"

class PowerUp : public Base2D
{
public:
	PowerUp();
	~PowerUp();

	bool Initialize(ID3D11Device*, int, int, XMFLOAT2);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//One of these values will be assigned during initialize
	enum POWER_TYPE { BOMB, SPREAD, SPEED, LIFE } ;
	//Increase the power based on this objects POWER_TYPE value
	void IncreasePower();

private:
	//Methods
	void Shutdown();

	//Variables
	POWER_TYPE e_powerType;
	//Total number of powers - Used in random
	int m_powerCount;

};

#endif//_POWER_UPS_H_
