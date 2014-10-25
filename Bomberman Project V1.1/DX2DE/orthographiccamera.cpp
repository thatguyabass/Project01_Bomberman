//--------------------------------------------------
//orthographiccamera.cpp
//--------------------------------------------------
//Define the camera Class

#include "orthographiccamera.h"

OrthoCamera::OrthoCamera()
{
	m_X = 0;
	m_Y = 0;
	m_lastPos = XMFLOAT2(0,0);

	m_pBox = 0;
	
	m_eLeft = ON;
	m_eRight = ON;
	m_eUp = ON;
	m_eDown = ON;

	m_pushBack = 1;
}

OrthoCamera::~OrthoCamera()
{
	Shutdown();
}

void OrthoCamera::MoveZ(float d)
{
	//This type of camera should never move in the z axis
}

void OrthoCamera::Render()
{	
	//Get the Directional values
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMVECTOR u = XMLoadFloat3(&m_up);
	XMVECTOR f = XMLoadFloat3(&m_forward);
	XMVECTOR p = XMLoadFloat3(&m_position);

	//Normalize and cross multiply the values
	f = XMVector3Normalize(f);
	u = XMVector3Normalize(XMVector3Cross(f, r));
	r = XMVector3Cross(u, f);

	float x = -XMVectorGetX(XMVector3Dot(p, r));
	float y = XMVectorGetY(XMVector3Dot(p, u));
	float z = -XMVectorGetZ(XMVector3Dot(p, f));

	//Apply the values to the View Matrix to be used for rendering objects
	m_viewMatrix(0,0) = m_right.x;
	m_viewMatrix(1,0) = m_right.y;
	m_viewMatrix(2,0) = m_right.z;
	m_viewMatrix(3,0) = x;

	m_viewMatrix(0,1) = m_up.x;
	m_viewMatrix(1,1) = m_up.y;
	m_viewMatrix(2,1) = m_up.z;
	m_viewMatrix(3,1) = y;
	
	m_viewMatrix(0,2) = m_forward.x;
	m_viewMatrix(1,2) = m_forward.y;
	m_viewMatrix(2,2) = m_forward.z;
	m_viewMatrix(3,2) = z;
	
	m_viewMatrix(0,3) = 0.0f;
	m_viewMatrix(1,3) = 0.0f;
	m_viewMatrix(2,3) = 0.0f;
	m_viewMatrix(3,3) = 1.0f;
}

void OrthoCamera::Shutdown()
{
	//Release Bounding Box
	if(m_pBox)
	{
		delete m_pBox;
		m_pBox = 0;
	}

	//Release the Tracked Object
	if(m_pTrack)
		m_pTrack = 0;

	//Release level bounds
	if(m_pLevelBounds)
		m_pLevelBounds = 0;
}

void OrthoCamera::Update(float gameTime)
{
	//Update the bounding box in case the position has moved
	m_pBox->Update(XMFLOAT2(m_position.x, m_position.y));
	XMFLOAT2 pos = m_pTrack->GetPosition();

	//Get which direction and by how much the target has moved
	float X = 0;
	float Y = 0;

	//Get the poisition of the target to the center of the camera
	float disX = 0;
	float disY = 0;
	
	//Only update if the camera box width is less then the level 
	if(m_pLevelBounds->width > m_pBox->width)
	{
		X = pos.x - m_lastPos.x;
		disX = m_center.x - pos.x;
		if(disX < 0) 
			disX *= -1;

		//Left Bounds Check
		if(m_pBox->left < m_pLevelBounds->left)
			m_eLeft = OFF;
	
		//Right Bounds Check
		if(m_pBox->right > m_pLevelBounds->right)
			m_eRight = OFF;

		//has a collision happened
		if(m_eLeft == OFF || m_eRight == OFF)
		{
			X = 0;

			//Check if the target has returned to center X
			if(disX < 10)
			{
				if(m_eLeft == OFF)
				{
					X = m_pushBack;
					m_eLeft = ON;
				}
				else if(m_eRight == OFF)
				{
					X = -m_pushBack;
					m_eRight = ON;
				}
			}
		}
	}

	//Only update if the camera box height is less then the level 
	if(m_pLevelBounds->height > m_pBox->height)
	{
		Y = pos.y - m_lastPos.y;
		disY = m_center.y - pos.y;
		if(disY < 0) 
			disY *= -1;

		//Top Bounds Check
		if(m_pBox->top < m_pLevelBounds->top)
			m_eUp = OFF;
	
		//Bottom Bounds Check
		if((-m_pBox->bottom) < m_pLevelBounds->bottom)
			m_eDown = OFF;

		//Has a collision happened
		if(m_eUp == OFF || m_eDown == OFF)
		{
			Y = 0;
			//Check to see if the target has returned to center Y
			if(disY < 10)
			{
				if(m_eUp == OFF)
				{
					Y = -m_pushBack;
					m_eUp = ON;
				}
				else if(m_eDown == OFF)
				{
					Y = m_pushBack;
					m_eDown = ON;
				}
			}
		}
	}

	//Add the change to the offset variables
	m_X += X;
	m_Y += Y;

	//Set the position of the camera
	this->SetPosition(m_X, -m_Y, m_position.z);
	
	//Find Center incase the camera has been moved
	m_center.x = m_pBox->right - (m_pBox->width / 2);
	m_center.y = -(m_pBox->bottom - (m_pBox->height / -2));
	
	//Set the last known location of the target
	m_lastPos = pos;
}

void OrthoCamera::Track(Base2D* track, BoundingBox* levelBounds)
{
	//Set the Tracking Object 
	m_pTrack = track;
	//Set the level bounds
	m_pLevelBounds = levelBounds;

	Retarget();
}

void OrthoCamera::Retarget()
{
	if(m_pTrack)
	{
		//Get the position of the tracked object
		XMFLOAT2 pos = m_pTrack->GetPosition();
		//Update the last position with the current position
		m_lastPos = pos;
		//Move the camera to the target is in the center
		m_X = pos.x;
		m_Y = pos.y;
	
		//Set the position based on the new values
		this->SetPosition(m_X, -m_Y, m_position.z);

		//Update the Bounding box position
		m_pBox->Update(XMFLOAT2(m_position.x, m_position.y));
	
		//Check if the Level width is creater then the camera view port
		if(m_pLevelBounds->width > m_pBox->width)
		{
			//Check to see which side of the view port is over the level width
			//Move the camera Right
			if(m_pBox->left < m_pLevelBounds->left)
			{
				m_X = (m_pBox->width / 2);
				m_eLeft = OFF;
			}
			//Move the camera Left
			if(m_pBox->right > m_pLevelBounds->right)
			{
				m_X = m_pLevelBounds->right - (m_pBox->width / 2);
				m_eRight = OFF;
			}
		}
		else//The level width is greater then the cameras view port
			m_X = (m_pLevelBounds->width / 2);
	
		//Check if the level height is greater then the view port height
		if(m_pLevelBounds->height > m_pBox->height)
		{
			//Check to see which side the view port is overthe level height
			//Move the camera Down
			if(m_pBox->top < m_pLevelBounds->top)
			{
				m_Y = (m_pBox->height / 2) + 50;
				m_eUp = OFF;
			}
			//Move the camera up
			if((-m_pBox->bottom) < m_pLevelBounds->bottom)
			{
				m_Y = (m_pLevelBounds->bottom - (m_pBox->height / 2));
				m_eDown = OFF;
			}
		}
		else//The level height is greater then the view port height
			m_Y = (m_pLevelBounds->height / 2);

		//Update the position
		this->SetPosition(m_X, -m_Y, m_position.z);
	}
}

void OrthoCamera::SetBounds(int width, int height)
{
	if(!m_pBox)
	{
		m_pBox = new BoundingBox();
		m_pBox->Set(width, -height, XMFLOAT2(width / 2, height / 2));

		m_center = XMFLOAT2(width / 2, height / 2);
	}
}