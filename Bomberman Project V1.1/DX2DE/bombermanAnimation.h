//----------------------------------------
//bombermanAnimation.h
//----------------------------------------
//Define Bombermans Animations

#ifndef _BOMBERMAN_ANIMATION_H_
#define _BOMBERMAN_ANIMATION_H_

#include "animation.h"

class BombermanAnimation : public Animation
{
public:
	BombermanAnimation();
	BombermanAnimation(const BombermanAnimation&);
	~BombermanAnimation();

	bool Initialize();

	void PlayIdle();
	void PlayForward();
	void PlayBack();
	void PlayLeft();
	void PlayRight();

private:
	//Methods
	void Shutdown();

	//Variables
	shared_ptr<AnimationSequence> m_pIdle;
	shared_ptr<AnimationSequence> m_pForward;
	shared_ptr<AnimationSequence> m_pBack;
	shared_ptr<AnimationSequence> m_pLeft;
	shared_ptr<AnimationSequence> m_pRight;
};

#endif//_BOMBERMAN_ANIMATION_H_