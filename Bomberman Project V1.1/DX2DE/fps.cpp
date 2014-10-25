//----------------------------------
//fpsclass.cpp
//----------------------------------

#include "fps.h"

int FPS::m_FPS = 0;

FPS::FPS()
{

}

FPS::FPS(const FPS& src)
{

}

FPS::~FPS()
{

}

void FPS::Initialize()
{
	m_count = 0;
	m_startTime = timeGetTime();
}

void FPS::Update()
{
	//Increment Count
	m_count++;

	if(timeGetTime() >= (m_startTime + 1000))//Every Second
	{
		FPS::m_FPS = m_count;//Set FPS
		m_count = 0;//Reset Count
		m_startTime = timeGetTime();//Set the time
	}
}

int FPS::GetFPS()
{
	return m_FPS;
}