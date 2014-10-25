//----------------------------------------------
//bomberman.h
//----------------------------------------------
//Primary user controlled character
//Responsible for the Initialize/Update and Render of the Bomb and Fire Classes

#ifndef _BOMBERMAN_H_
#define _BOMBERMAN_H_

#include "base2D.h"
#include "input.h"
#include "bombermanAnimation.h"
#include "LevelData.h"
#include "bomb.h"
#include "fireManager.h"
#include "Enemy.h"
#include "stats.h"
#include "Global.h"

#include <vector>
#include <memory>
#include <functional>

//------------------------------------------------------------------------------------------------------------------------------------

#define _GOD_MODE Global::GetGodMode()

//System short cuts
const float BOMB_TIME = 500;
const int GOD_MODE_TIME = 5000;
const int ACTIVE_NODE = 0;
const int INACTIVE_NODE = 1;

class Bomberman : public Base2D
{
public:
	Bomberman();
	~Bomberman();

	bool Initialize(ID3D11Device*, XMFLOAT2 imageSize, XMFLOAT2 index, shared_ptr<LevelData>);

	//When the Character Dies and the camera needs to be repositioned
	void SetCameraCallback(function<void(void)>);
	
	//Update the Movement/Collision
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	void CollisionWithWall(int, BoundingBox*);

	//Get the Current Location based on the level grid
	XMFLOAT2 GetIndex();
	
	//Collision Check Methods
	bool GetHit();
	void CheckEnemyIntersects(Enemy*);

private:
	//Method
	void Shutdown();
	void UpdateTexture(VertexType*);
	void PlaceBomb(const XMFLOAT2&);
	void AddBomb();

	//Movement Methods
	void Move(float d, Node*); //<- Only one used in this project
	void Move(float d, XMFLOAT2);
	void Rotate(float d);

	//Confirmed Collision Methods 
	void Hit();
	void GodMode(float);
	void GodModeFade(float);

	//Variables
	ID3D11Device* m_pDevice;
	shared_ptr<LevelData> m_pLevelData;
	
	//Animation
	float m_nextFrame;
	shared_ptr<BombermanAnimation> m_pAnimation;
	
	//Movement
	float m_xVelocity;
	float m_yVelocity;

	//Grid Positioning
	XMFLOAT2 m_index;
	XMFLOAT2 m_spawnIndex;

	//Confirmed Hit Variables
	bool m_hit;
	bool m_godMode;
	float m_godTimer;
	float m_godAlpha;
	bool m_fade;

	//Bomb
	vector<shared_ptr<Bomb>> m_pBombs;
	float m_bombTimer;
	int m_activeBombs;

	//Call back method
	function<void(void)> m_cameraCallback;

};

#endif//_BOMBERMAN_H_