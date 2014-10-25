//--------------------------------------------------------------------------
//LevelInfo.h
//--------------------------------------------------------------------------
//This class is responsible for reading the level files and storing them into objects that can then be used to generate a level
//Level Data and Enemy Data

#ifndef _LEVEL_INFO_H_
#define _LEVEL_INFO_H_

#include "LData.h"
#include "EData.h"

#include<memory>
#include<fstream>
using namespace std;

//--------------------------------------------------------------------------------------------------------------------------

class LevelInfo
{
public:
	LevelInfo();
	~LevelInfo();

	bool ReadFile(wstring);

	int GetEnemyCount();
	EData* GetEnemyData(int);
	LData* GetLevelData();

private:
	//Methods
	void Shutdown();

	//Variables
	int m_enemyCount;
	vector<shared_ptr<EData>> m_pEData;
	shared_ptr<LData> m_pLData;

};

#endif//_LEVEL_INFO_H_