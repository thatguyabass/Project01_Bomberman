//----------------------------------------------
//MadGuy.h
//----------------------------------------------
//Define the Mad Guy Enemy

#ifndef _MAD_GUY_H_
#define _MAD_GUY_H_

#include "Enemy.h"

class MadGuy : public Enemy
{
public:
	MadGuy();
	MadGuy(const MadGuy&);
	~MadGuy();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_MAD_GUY_H_