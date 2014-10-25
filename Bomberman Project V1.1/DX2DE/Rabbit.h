//------------------------------//----------------------------------------------
//Rabbit.h
//----------------------------------------------
//Define the Rabbit Enemy

#ifndef _RABBIT_H_
#define _RABBIT_H_

#include "Enemy.h"

class Rabbit : public Enemy
{
public:
	Rabbit();
	Rabbit(const Rabbit&);
	~Rabbit();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_RABBIT_H_