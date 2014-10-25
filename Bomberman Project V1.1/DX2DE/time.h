//----------------------------------------------------------
//time.h
//----------------------------------------------------------
//handle the Game time
//Using the game time will prevent lag or outside forces from breaking the game

#ifndef _TIME_H_
#define _TIME_H_

#include<Windows.h>

class Time
{
public:
	Time();
	Time(const Time&);
	~Time();

	bool Initialize(HWND);
	void Update();
	void LostFocus();//If the user loses focus with the application

	float DeltaTime();

private:
	HWND m_hwnd;

	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif//_TIME_H_
