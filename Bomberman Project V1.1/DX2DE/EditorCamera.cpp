//-------------------------------------------------------------
//EditorCamera.cpp
//-------------------------------------------------------------
//

#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
	m_xVelocity = 0;
	m_yVelocity = 0;
	m_direction = XMFLOAT2(0,0);
	m_speed = 1;
}

EditorCamera::~EditorCamera()
{
	Shutdown();
}

bool EditorCamera::Initialize()
{
	bool result;

	return true;
}

void EditorCamera::Update(float gameTime)
{
	//Get the Current Input values
	int current = Input::GetCurrentKeys();
	int currentS = Input::GetCurrentSpecial();

	//Process the input and translate the camera
	if(current & KEY_W || currentS & KEY_UP)
	{
		m_direction.y = -1;
		Move(gameTime * m_speed);
	}
	else if(current & KEY_S || currentS & KEY_DOWN)
	{
		m_direction.y = 1;
		Move(gameTime * m_speed);
	}
	else
	{
		m_direction.y = 0;
		m_yVelocity = 0;
	}

	if(current & KEY_A || currentS & KEY_LEFT)
	{
		m_direction.x = -1;
		Move(gameTime * m_speed);
	}
	else if(current & KEY_D || currentS & KEY_RIGHT)
	{
		m_direction.x = 1;
		Move(gameTime * m_speed);
	}
	else
	{
		m_direction.x = 0;
		m_xVelocity = 0;
	}

	m_position.x += m_xVelocity;
	m_position.y += m_yVelocity;
}

void EditorCamera::SetSpeed(float speed)
{
	m_speed = speed;
}
float EditorCamera::GetSpeed()
{
	return m_speed;
}

void EditorCamera::Shutdown()
{

}

void EditorCamera::Move(float s)
{
	m_xVelocity = s * m_direction.x;
	m_yVelocity = s * m_direction.y;
}