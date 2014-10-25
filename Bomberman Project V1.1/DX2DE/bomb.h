//-----------------------------------------------------
//bomb.h
//-----------------------------------------------------
//Contain all the behavior for Bomberman's bombs
//Bombs are used to clear out level sections and kill enemies

#ifndef _BOMB_H_
#define _BOMB_H_

#include "base2D.h"
#include "LevelData.h"
#include "fireManager.h"

//-----------------------------------------------------------------------------------------------------------------------

const float DETONATION_TIME = 3000;

class Bomb : public Base2D
{
public:
	Bomb();
	~Bomb();

	bool Initialize(ID3D11Device*, XMFLOAT2 imageSize, shared_ptr<LevelData>, XMFLOAT2, int);
	void Update(float gameTime);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho); 

	//Has the bomb explotion timer been reached
	bool IsFinished();
	//Get this bombs location based on the grid index
	XMFLOAT2 GetIndex();

	//Ready to be removed from the total planted count
	bool RemoveFromCount();
	void Boom();
	bool IsPlanted();
	bool IsBoom();

	bool CheckFireBounds(BoundingBox*);

	//States the bomb can enter
	enum BombState { PLANTED, INITIALIZE_BOOM, BOOM };

private:	
	//Methods
	void Shutdown();

	//Variables
	shared_ptr<LevelData> m_pLevelData;
	shared_ptr<FireManager> m_pFire;
	ID3D11Device* m_pDevice;

	//Flags for system completion
	bool m_finished;
	bool m_remove;

	int m_fireSpread;

	//Track the Current state
	float m_timer;
	XMFLOAT2 m_index;
	BombState m_eBombState;

};

#endif//_BOMB_H_