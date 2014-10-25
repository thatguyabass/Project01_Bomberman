//-----------------------------------------------------
//BlueEye.h
//-----------------------------------------------------
//

#ifndef _BLUE_EYE_H_
#define _BLUE_EYE_H_

#include "Enemy.h"

class BlueEye : public Enemy
{
public:
	BlueEye();
	BlueEye(const BlueEye&);
	~BlueEye();

	bool Initialize(ID3D11Device*, XMFLOAT2, Node*, Waypoint*);

private:

};

#endif//_BLUE_EYE_H_