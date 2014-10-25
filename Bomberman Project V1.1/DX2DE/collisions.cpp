//----------------------------------------------
//collisions.h
//----------------------------------------------
//

#include "collisions.h"

//Position the bounding box around the offset provided
void BoundingBox::Set(float width, float height, XMFLOAT2 offset)
{
	this->top = offset.y + (height / 2);
	this->bottom = top  - height;
	this->left = offset.x - (width / 2);
	this->right = left + width;
	this->width = width;
	this->height = height;
}

//Position the bounding box between the values provided
void BoundingBox::Set(float top, float right, float bottom, float left)
{
	this->top = top;
	this->bottom = bottom;
	this->right = right;
	this->left = left;
	this->width = right - left;
	this->height = top - bottom;
}

//Move the bounding box around the position value
void BoundingBox::Update(XMFLOAT2 position)
{
	this->top = position.y + (height / 2);
	this->bottom = top - height;
	this->left = position.x - (width / 2);
	this->right = left + width;
}
//Check collision between this box and another box
bool BoundingBox::BoundingBoxIntersect(BoundingBox* box)
{
	if(this->bottom > box->top)
		return false;
	if(this->top < box->bottom)
		return false;
	if(this->left > box->right)
		return false;
	if(this->right < box->left)
		return false;

	return true;
}
//Check Collision between the two povided boxes
bool BoundingBox::BoundingBoxIntersect(BoundingBox* box1, BoundingBox* box2)
{
	if(box1->bottom > box2->top)
		return false;
	if(box1->top < box2->bottom)
		return false;
	if(box1->left > box2->right)
		return false;
	if(box1->right < box2->left)
		return false;

	return true;
}
//Check the collision and return which side the collision occurred
bool BoundingBox::BoundingBoxIntersect(BoundingBox* box1, BoundingBox* box2, int& side)
{
	if(box1->bottom > box2->top)
		return false;
	if(box1->top < box2->bottom)
		return false;
	if(box1->left > box2->right)
		return false;
	if(box1->right < box2->left)
		return false;

	float smallest;

	//BOTTOM
	float x1 = box1->bottom - box2->top;
	if(x1 < 0) 
		x1 *= -1;
	smallest = x1;
	side = 0;
	
	//TOP
	float x2 = box1->top - box2->bottom;
	if(x2 < 0)
		x2 *= -1;
	if(smallest > x2)
	{
		smallest = x2;
		side = 1;
	}

	//LEFT
	float x3 = box1->left - box2->right;
	if(x3 < 0) 
		x3 *= -1;
	if(smallest > x3)
	{
		smallest = x3;
		side = 2;
	}

	//RIGHT
	float x4 = box1->right - box2->left;
	if(x4 < 0) 
		x4 *= -1;
	if(smallest > x4) 
	{
		smallest = x4;
		side = 3;
	}

	return true;
}