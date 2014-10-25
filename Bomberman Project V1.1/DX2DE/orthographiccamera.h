//--------------------------------------------
//orthographiccamera.h
//--------------------------------------------
//Represents the users view port to view the rendered objects

#ifndef _ORTHOGRAPHIC_CAMERA_CLASS_H_
#define _ORTHOGRAPHIC_CAMERA_CLASS_H_

#include "basecamera.h"
#include "base2D.h"

class OrthoCamera : public BaseCamera
{
public:
	OrthoCamera();
	~OrthoCamera();

	void MoveZ(float);
	//Update will update the tracking functionality
	virtual void Update(float);
	//Set an object that the camera will track
	void Track(Base2D*, BoundingBox*);
	void Render();

	//reset the cameras position to properly track the target
	void Retarget();
	//Set up the bounding box for the view port
	void SetBounds(int, int);
	
protected:
	//Methods
	void Shutdown();

	//State to track if the side of the view port is on target or off target - Would be marked as off target if box collided with a wall
	enum Offset{ ON, OFF };
	Offset m_eLeft;
	Offset m_eRight;
	Offset m_eUp;
	Offset m_eDown;

	//Variables
	float m_X;
	float m_Y;

	//Tracked object
	Base2D* m_pTrack;
	//Last known position of the tracked object
	XMFLOAT2 m_lastPos;
	BoundingBox* m_pLevelBounds;

	//Center of the view port
	XMFLOAT2 m_center;
	//Bounding box for the view port
	BoundingBox* m_pBox;

	//Amount the box will be pushed off when hitting the level box
	float m_pushBack;

};

#endif //_ORTHOGRAPHIC_CAMERA_CLASS_H_