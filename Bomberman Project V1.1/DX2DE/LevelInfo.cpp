//----------------------------------------------------------------
//LevelInfo.cpp
//----------------------------------------------------------------
//

#include "LevelInfo.h"

LevelInfo::LevelInfo()
{

}

LevelInfo::~LevelInfo()
{
	Shutdown();
}

bool LevelInfo::ReadFile(wstring level)
{
	ifstream in;
	char input;

	char** data;
	int count = 0;
	int x = 0;
	int y = 0;

	in.open(level.c_str());

	if(in.fail())
		return false;

	//Get number of Node across
	in >> x;	

	in.get(input);
	while(input != ':')
		in.get(input);

	//Get number of Node high
	in >> y;

	//Levels are have to be read Y - X for proper placement
	data = new char*[y];
	for(int c = 0; c < y; c++)
	{
		data[c] = new char[x];
		for(int v = 0; v < x; v++)
		{
			in.get(input);
			if(input == '\n')
				in.get(input);

			data[c][v] = input;
		}
	}

	//Set the level data
	m_pLData = shared_ptr<LData>(new LData);
	m_pLData->SetLevel(x, y, data);
	//Release Temp Data
	for(int c = 0; c < y; c++)
		delete [] data[c];
	delete [] data;

	//Loop through till the next section of valid data
	in.get(input);
	while(input != ':')
		in.get(input);

	//Get the Enemy Count
	in >> m_enemyCount;

	//Gather the Enemy Data
	for(int c = 0; c < m_enemyCount; c++)
	{
		//Create the Enemy Data
		m_pEData.push_back(shared_ptr<EData>(new EData));

		//get the enemy type
		in.get(input);
		while(input == '\n')
			in.get(input);
		
		m_pEData[c]->SetType(input);;
		
		//Make sure the next input will not be a spacer ':'
		in.get(input);
		while(input != ':')
			in.get(input);

		//Get the X position
		in >> x;
		
		//Get the Y position
		in.get(input);
		in >> y;

		m_pEData[c]->SetPosition(x, y);

		//Get the Waypoint Count
		in.get(input);
		in >> count;
		m_pEData[c]->SetCount(count);
		//Get the Waypoints positions
		for(int v = 0; v < count; v++)
		{
			//Get the Waypoints X position
			in >> x;

			//Get the Waypoints Y position
			in.get(input);
			in >> y;

			m_pEData[c]->SetWaypoint(x, y);
		}
	}

	in.close();

	return true;
}

void LevelInfo::Shutdown()
{
	//Release Level Data
	if(m_pLData)
	{
		m_pLData.reset();
		m_pLData = 0;
	}

	//Release the Enemy Data
	for(int c = 0; c < m_pEData.size(); c++)
	{
		m_pEData[c].reset();
		m_pEData[c] = 0;
	}
	m_pEData.clear();
}

int LevelInfo::GetEnemyCount()
{
	return m_enemyCount;
}

EData* LevelInfo::GetEnemyData(int index)
{
	return m_pEData[index].get();
}

LData* LevelInfo::GetLevelData()
{
	return m_pLData.get();
}