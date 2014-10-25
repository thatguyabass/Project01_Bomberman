//----------------------------------------------
//MadBomb.h
//----------------------------------------------
//Define the MadBomb Enemy

#ifndef _MAD_BOMB_H_
#define _MAD_BOMB_H_

#include "Enemy.h"

class MadBomb : public Enemy
{
public:
	MadBomb();
	MadBomb(const MadBomb&);
	~MadBomb();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_MAD_BOMB_H_