//--------------------------------------
//basecamera.h
//--------------------------------------
//Base camera class
//Handles the variables and methods that every camera may need

#ifndef _BASE_CAMERA_H_
#define _BASE_CAMERA_H_

#include<Windows.h>
#include<xnamath.h>

class BaseCamera
{
public:
	BaseCamera();
	virtual ~BaseCamera();
	
	//Position Access Methods
	void SetPosition(float, float, float);
	void SetPosition(float, float);
	void SetPosition(XMFLOAT3);
	virtual XMFLOAT3 GetPosition();
	//Return the X and Y values of position
	XMFLOAT2 GetPosition2D();

	//Directional Access Methods
	XMFLOAT3 GetRight();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetForward();

	//Movement Methods
	void Move(float, XMFLOAT3);
	void MoveX(float);
	void MoveY(float);
	virtual void MoveZ(float);

	virtual void Render();

	//Access the view Matrix - Passed into the Texture Shader with each object rendered
	void GetViewMatrix(XMFLOAT4X4&);

protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_right;
	XMFLOAT3 m_up;
	XMFLOAT3 m_forward;

	XMFLOAT4X4 m_viewMatrix;
};

#endif//_BASE_CAMERA_H_