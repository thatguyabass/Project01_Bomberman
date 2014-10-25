//---------------------------------------------
//Animation.h
//---------------------------------------------
//Base class for animation purposes, This class should never be created directly.
//This class is not responsible for the rendering of the object

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include<Windows.h>
#include<xnamath.h>

#include<memory>
#include<vector>
using namespace std;

//---------------------------------------------------------------------------------------------------

//Information on the current animation
struct AnimationInfo
{
	//These are vector2 to get the Min and Max of each value
	XMFLOAT2 U; //Animation Width
	XMFLOAT2 V;	//Animation Height
	AnimationInfo();
	AnimationInfo(XMFLOAT2, XMFLOAT2);
};

//Uses sequences of animation info to display moving images
struct AnimationSequence
{
	int count;
	AnimationInfo* info;//The parent class is responsible for releasing this object
};

class Animation
{
public:
	Animation();
	virtual ~Animation();

	virtual bool Initialize();

	void PlayAnimation(shared_ptr<AnimationSequence>);
	AnimationSequence* GetCurrent();
	void NextFrame();
	int GetFrameNumber();

protected:
	//Methods
	virtual void Shutdown();
	
	//Variables
	shared_ptr<AnimationSequence> m_pCurrent;
	int m_currentFrame;

};


#endif//_ANIMATION_H_
