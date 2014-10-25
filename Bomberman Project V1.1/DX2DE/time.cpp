//---------------------------------------------
//time.cpp
//---------------------------------------------
//

#include "time.h"

Time::Time()
{

}

Time::Time(const Time& src)
{

}

Time::~Time()
{

}

bool Time::Initialize(HWND hwnd)
{
	//Check to see if this system can support high performance timers 
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
		return false;

	//Find out how many times the frequency counter ticks each millisecond
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void Time::Update()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//Calculate the time between this frame and the last frame
	timeDifference = (float)(currentTime - m_startTime);

	//Calculate how long this frame took
	m_frameTime = timeDifference / m_ticksPerMs;

	//Reset the start time
	m_startTime = currentTime;
}

void Time::LostFocus()
{
	INT64 currentTime;
	float timeDifference;

	//Get the Current Time
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//Reset the start time
	m_startTime = currentTime;
}

float Time::DeltaTime()
{
	return m_frameTime;
}