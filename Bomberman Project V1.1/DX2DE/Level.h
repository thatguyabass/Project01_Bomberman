//--------------------------
//Level.h
//--------------------------
//This is the core game Class for the application
//Level handles the initialize,update and render for the games
//Because this class is dynamic it can create a wide variety of different levels

#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "baselevel.h"
#include "LevelData.h"
#include "LevelInfo.h"
#include "HUD.h"

#include "static2D.h"
#include "bomberman.h"
#include "Enemy.h"
#include "Powers.h"

#include "heli.h"
#include "yellow.h"
#include "BlueEye.h"
#include "MadBomb.h"
#include "MadGuy.h"
#include "Rabbit.h"
#include "Waypoint.h"

#include "LevelInfo.h"
#include "Continue.h"

using namespace std;
using namespace std::placeholders;

//---------------------------------------------------------------------------------------------------

class Level : public BaseLevel
{
public:
	Level();
	Level(const Level&);
	~Level();

	bool Initialize(shared_ptr<D3D>, wstring levelDir, bool cont = false);
	void Update(float);
	bool Render();
	
	//Get the Current Status of the level
	bool LevelStatus();
	int GetContinueState();

	//Level State
	enum LEVEL_STATE { PLAYING, CONTINUE };

private:
	//Methods	
	bool InitializeGrid(LData*);//Create and Initialize the Level 
	bool InitializeBomberman(XMFLOAT2 size, XMFLOAT2 index);//Create and initialize Bomberman
	bool InitializeHUD(bool);//Create and Initialize the HUD and Continue Menu

	//Create and Initialize the Enemies
	template<class E>
	bool AddEnemy(E, XMFLOAT2 size, Node* start, Waypoint* waypoint);
	void SetEnemy(LevelInfo*);//Create all the enemies in the level info 
	void InitializeWaypoints(LevelInfo*);//Initialize the Waypoints for the enemies

	void Shutdown();
	void CameraCallback();//Call the camera to reset its position when Bomberman dies

	//Level Complete Check
	void LevelComplete();
	void DropPowerUp(XMFLOAT2);//Calculate if a power up should drop

	//Reset the game states to base values
	void ResetStats();
	
	//Variables
	shared_ptr<LevelData> m_pLevelData;
	shared_ptr<Bomberman> m_pBomber;

	int m_enemyCount;
	vector<shared_ptr<Waypoint>> m_pWaypoints;
	vector<shared_ptr<Enemy>> m_pEnemy;
	vector<Node*> m_pOpen;

	shared_ptr<HUD> m_pHUD;
	shared_ptr<Continue> m_pCont;

	int m_dropChance;
	vector<shared_ptr<PowerUp>> m_pPower;

	LEVEL_STATE e_level;

};

template<class E>
bool Level::AddEnemy(E item, XMFLOAT2 size, Node* start, Waypoint* waypoint)
{
	bool result;

	//Create and Initialize the Enemy
	m_pEnemy.push_back(shared_ptr<Enemy>(item));
	result = m_pEnemy[m_enemyCount]->Initialize(m_pD3D->GetDevice(), size, start,  waypoint);
	if(!result)
		return false;

	m_enemyCount++;

	return true;
}

#endif //_LEVEL_H_