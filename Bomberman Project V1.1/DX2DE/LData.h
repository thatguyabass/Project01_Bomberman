//--------------------------------------------------------------
//LData.h
//--------------------------------------------------------------
//Store the Level data from a text file to be used in creating new levels

#ifndef _LEVEL_DATA_INFO_H_
#define _LEVEL_DATA_INFO_H_

//------------------------------------------------------------------------------

class LData
{
public:
	LData();
	~LData();

	//Set the level Node count
	int GetX();
	int GetY();

	//Set the level data
	void SetLevel(int X, int Y, char** data);
	char GetChar(int, int);

private:
	//Methods
	void Shutdown();

	//Variables
	int m_X;
	int m_Y;
	char** m_ppLevelData;

};

#endif//_LEVEL_DATA_INFO_H_