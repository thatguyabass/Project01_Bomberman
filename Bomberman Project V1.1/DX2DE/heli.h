//----------------------------------------------
//heli.h
//----------------------------------------------
//Define the Heli Enemy

#ifndef _HELI_H_
#define _HELI_H_

#include "Enemy.h"

class Heli : public Enemy
{
public:
	Heli();
	Heli(const Heli&);
	~Heli();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_HELI_H_