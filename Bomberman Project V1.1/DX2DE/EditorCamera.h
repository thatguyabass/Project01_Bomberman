//------------------------------------------------------------------
//EditorCamera.h
//------------------------------------------------------------------
//Cameras Soul use is in the Level Editor
//This Camera has a full range of motion in X and Y

#ifndef _EDITOR_CAMERA_H_
#define _EDITOR_CAMERA_H_

#include "orthographiccamera.h"
#include "input.h"

class EditorCamera : public OrthoCamera
{
public:
	EditorCamera();
	~EditorCamera();

	bool Initialize();
	//Process the Input to move the camera
	void Update(float);

	void SetSpeed(float);
	float GetSpeed();

private:
	//Methods
	void Move(float);
	void Shutdown();

	//Variables
	float m_xVelocity;
	float m_yVelocity;

	//Direction the camera will move in when input is provided
	XMFLOAT2 m_direction;
	float m_speed;

};

#endif//_EDITOR_CAMERA_H_