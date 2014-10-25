//----------------------------------------------
//bombermanAnimation.cpp
//----------------------------------------------
//Define the Bomberman Animation Class

#include "bombermanAnimation.h"

BombermanAnimation::BombermanAnimation()
{

}

BombermanAnimation::BombermanAnimation(const BombermanAnimation& src)
{

}

BombermanAnimation::~BombermanAnimation()
{
	Shutdown();
}

//Initialize the Animations for Bomberman
//Each of the XMFLOAT2 are coordinantes for the UV from the Sprite Sheet bomberman is using
bool BombermanAnimation::Initialize()
{
	m_pForward = shared_ptr<AnimationSequence>(new AnimationSequence);
	m_pForward->count = 4;
	m_pForward->info = new AnimationInfo[4];
	m_pForward->info[0] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.0f, 0.11f));
	m_pForward->info[1] = AnimationInfo(XMFLOAT2(0.66f, 1.0f), XMFLOAT2(0.0f, 0.11f));
	m_pForward->info[2] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.0f, 0.11f));
	m_pForward->info[3] = AnimationInfo(XMFLOAT2(0.0f, 0.33f), XMFLOAT2(0.0f, 0.11f));

	m_pLeft = shared_ptr<AnimationSequence>(new AnimationSequence);
	m_pLeft->count = 4;
	m_pLeft->info = new AnimationInfo[4];
	m_pLeft->info[0] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.11f, 0.22f));
	m_pLeft->info[1] = AnimationInfo(XMFLOAT2(0.66f, 1.0f), XMFLOAT2(0.11f, 0.22f));
	m_pLeft->info[2] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.11f, 0.22f));
	m_pLeft->info[3] = AnimationInfo(XMFLOAT2(0.0f, 0.33f), XMFLOAT2(0.11f, 0.22f));

	m_pRight = shared_ptr<AnimationSequence>(new AnimationSequence);
	m_pRight->count = 4;
	m_pRight->info = new AnimationInfo[4];
	m_pRight->info[0] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.22f, 0.33f));
	m_pRight->info[1] = AnimationInfo(XMFLOAT2(0.66f, 1.0f), XMFLOAT2(0.22f, 0.33f));
	m_pRight->info[2] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.22f, 0.33f));
	m_pRight->info[3] = AnimationInfo(XMFLOAT2(0.0f, 0.33f), XMFLOAT2(0.22f, 0.33f));

	m_pBack = shared_ptr<AnimationSequence>(new AnimationSequence);
	m_pBack->count = 4;
	m_pBack->info = new AnimationInfo[4];
	m_pBack->info[0] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.33f, 0.44f));
	m_pBack->info[1] = AnimationInfo(XMFLOAT2(0.66f, 1.0f), XMFLOAT2(0.33f, 0.44f));
	m_pBack->info[2] = AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.33, 0.44f));
	m_pBack->info[3] = AnimationInfo(XMFLOAT2(0.0f, 0.33f), XMFLOAT2(0.33f, 0.44f));

	m_pIdle = shared_ptr<AnimationSequence>(new AnimationSequence);
	m_pIdle->count = 1;
	m_pIdle->info = new AnimationInfo(XMFLOAT2(0.33f, 0.66f), XMFLOAT2(0.0f, 0.11f));

	m_pCurrent = m_pIdle;

	return true;
}

//Play the animation after checking to make sure the desired animation isn't already playing
void BombermanAnimation::PlayIdle()
{
	if(m_pCurrent != m_pIdle)
		PlayAnimation(m_pIdle);
}
void BombermanAnimation::PlayForward()
{
	if(m_pCurrent != m_pForward)
		PlayAnimation(m_pForward);
}
void BombermanAnimation::PlayBack()
{
	if(m_pCurrent != m_pBack)
		PlayAnimation(m_pBack);
}
void BombermanAnimation::PlayLeft()
{
	if(m_pCurrent != m_pLeft)
		PlayAnimation(m_pLeft);
}
void BombermanAnimation::PlayRight()
{
	if(m_pCurrent != m_pRight)
		PlayAnimation(m_pRight);
}

//Release the Animation Sequence objects
void BombermanAnimation::Shutdown()
{
	if(m_pForward)
	{
		delete [] m_pForward->info;
		m_pForward.reset();
		m_pForward = 0;
	}

	if(m_pBack)
	{
		delete [] m_pBack->info;
		m_pBack.reset();
		m_pBack = 0;
	}

	if(m_pLeft)
	{
		delete [] m_pLeft->info;
		m_pLeft.reset();
		m_pLeft = 0;
	}

	if(m_pRight)
	{
		delete [] m_pRight->info;
		m_pRight.reset();
		m_pRight = 0;
	}

	if(m_pIdle)
	{
		delete [] m_pIdle->info;
		m_pIdle.reset();
		m_pIdle = 0;
	}

}