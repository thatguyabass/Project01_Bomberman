//-------------------------------------------------
//collisions.h
//-------------------------------------------------
//this class will handle all the collision calculations for box vs box

#ifndef _COLLISIONS_H_
#define _COLLISIONS_H_

#include<Windows.h>
#include<xnamath.h>

//Base collision object
struct BoundingBox
{
	//Values used during collision checking
	float top;
	float right;
	float bottom;
	float left;
	float width;
	float height;

	//Set the bounding box width/height around a center point
	void Set(float width, float height, XMFLOAT2 offset);

	//Manual set up, Position is based on the average between the Wid/Height X-Y values
	void Set(float top, float right, float bottom, float left);

	//Collision Checks
	bool BoundingBoxIntersect(BoundingBox*);
	bool BoundingBoxIntersect(BoundingBox*, BoundingBox*);

	//return the side the collision occured
	bool BoundingBoxIntersect(BoundingBox*, BoundingBox*, int& side);

	//Update the position of the bounding box to move with the object
	void Update(XMFLOAT2);
};

#endif