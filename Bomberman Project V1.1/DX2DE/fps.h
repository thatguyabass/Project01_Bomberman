//------------------------------------------------------
//fps.h
//------------------------------------------------------
//Calculate the Frames per second 

#ifndef _FPS_H_
#define _FPS_H_

#pragma comment(lib, "winmm.lib")

#include<Windows.h>
#include<mmsystem.h>

//----------------------------------------------------------------------------------------

class FPS
{
public:
	FPS();
	FPS(const FPS&);
	~FPS();

	void Initialize();
	void Update();
	static int GetFPS();

private:
	int m_count;
	static int m_FPS;
	unsigned long m_startTime;

};


#endif//_FPS_H_