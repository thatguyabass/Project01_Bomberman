//----------------------------------------------
//fireManager.h
//----------------------------------------------
//This class will handle the spawning of the fire objects based on a central index and fire spread

#ifndef _FIRE_MANAGER_H_
#define _FIRE_MANAGER_H_

#include "LevelData.h"
#include "fire.h"

//How long the fire objects will exist for
#define EXPLOSION_TIME 1500

class FireManager
{
public:
	FireManager();
	FireManager(const FireManager&);
	~FireManager();

	bool Initialize(ID3D11Device*, XMFLOAT2 index, int fireSpread, shared_ptr<LevelData>);
	bool Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	bool CheckIntersect(BoundingBox*);

private:
	//Methods
	void Shutdown();
	bool SetupFire(ID3D11Device*, int indexMod, bool leftRight);
	void SetupBoundingBoxes();
	bool CheckGrid(XMFLOAT2 index);

	//Variables
	XMFLOAT2 m_index;

	int m_spread;
	int m_fireCount;
	vector<shared_ptr<Fire>> m_pFire;
	shared_ptr<LevelData> m_pLevelData;
	XMFLOAT2 m_fireSize;
	
	int m_blocked; //<- Calculate how much was blocked by the level geometry
	float m_timer;

	float m_boundsMod;
	shared_ptr<BoundingBox> m_pBoxLR;
	shared_ptr<BoundingBox> m_pBoxUD;

};

#endif//_FIRE_MANAGER_H_
