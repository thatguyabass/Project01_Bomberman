//---------------------------------------------------------------
//LData.cpp
//----------------------------------------------------------------
//

#include "LData.h"

LData::LData()
{

}

LData::~LData()
{
	Shutdown();
}

int LData::GetX()
{
	return m_X;
}

int LData::GetY()
{
	return m_Y;
}

void LData::SetLevel(int x, int y, char** level)
{
	//Set the level Node count
	m_X = x;
	m_Y = y;

	//Store the values to be used later in creating the level 
	m_ppLevelData = new char*[m_Y];
	for(int c = 0; c < m_Y; c++)
	{
		m_ppLevelData[c] = new char[m_X];
		for(int v = 0; v < m_X; v++)
			m_ppLevelData[c][v] = level[c][v];//Store them [Y][X] in the array
	}
}

char LData::GetChar(int x, int y)
{
	return m_ppLevelData[x][y];
}

void LData::Shutdown()
{
	for(int c = 0; c < m_Y; c++)
		delete [] m_ppLevelData[c];
	delete [] m_ppLevelData;
}