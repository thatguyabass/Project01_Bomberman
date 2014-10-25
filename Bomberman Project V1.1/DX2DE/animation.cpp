//---------------------------------------------
//animation.cpp
//---------------------------------------------
//Define the base animation class

#include "animation.h"

Animation::Animation()
{
	m_currentFrame = 0;
}

Animation::~Animation()
{
	Shutdown();
}

bool Animation::Initialize()
{
	return true;
}

//Play the next image in the animation
void Animation::PlayAnimation(shared_ptr<AnimationSequence> next)
{
	if(m_pCurrent != next)
	{
		m_pCurrent = next;
		m_currentFrame = 0;
	}
}

AnimationSequence* Animation::GetCurrent()
{
	return m_pCurrent.get();
}

//Play the next frame in the animation, looping if the max frame was exceeded
void Animation::NextFrame()
{
	m_currentFrame++;
	if(m_currentFrame >= m_pCurrent->count)
		m_currentFrame = 0;
}
int Animation::GetFrameNumber()
{
	return m_currentFrame;
}

void Animation::Shutdown()
{
	m_currentFrame = 0;
	if(m_pCurrent)
	{
		m_pCurrent.reset();
		m_pCurrent = 0;
	}
}

AnimationInfo::AnimationInfo()
{

}

AnimationInfo::AnimationInfo(XMFLOAT2 _U, XMFLOAT2 _V)
{
	U = _U;
	V = _V;
}