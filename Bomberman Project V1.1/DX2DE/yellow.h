//----------------------------------------------
//yellow.h
//----------------------------------------------
//Define the Heli Enemy

#ifndef _YELLOW_H_
#define _YELLOW_H_

#include "Enemy.h"

class Yellow : public Enemy
{
public:
	Yellow();
	Yellow(const Yellow&);
	~Yellow();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_YELLOW_H_